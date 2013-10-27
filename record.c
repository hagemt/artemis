#include "crawl.h"
#include "record.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* wrappers */

#include <libcalg/compare-string.h>
#include <libcalg/hash-string.h>

static unsigned long
__wrap_hash_path(HashTableKey key)
{
	ENTRY *e;
	assert(key);
	e = (ENTRY *) key;
	/* FIXME nocase? */
	return string_hash(e->path);
}

static unsigned long
__wrap_hash_data(HashTableKey key)
{
	ENTRY *e;
	size_t offset;
	unsigned long result = 0;
	assert(key);
	e = (ENTRY *) key;
	/* TODO better binary hash function */
	for (offset = 0; offset < DAT_LEN; offset += sizeof(unsigned long)) {
		result ^= * (unsigned long *) (e->data + offset);
	}
	return result;
}

static int
__wrap_equals_path(HashTableKey k1, HashTableKey k2)
{
	ENTRY *e1, *e2;
	assert(k1 && k2);
	e1 = (ENTRY *) k1;
	e2 = (ENTRY *) k2;
	return string_equal(e1->path, e2->path);
}

static int
__wrap_equals_data(HashTableKey k1, HashTableKey k2)
{
	ENTRY *e1, *e2;
	assert(k1 && k2);
	e1 = (ENTRY *) k1;
	e2 = (ENTRY *) k2;
	return memcmp(e1->data, e2->data, DAT_LEN);
}

static void
__wrap_free_set(HashTableValue value)
{
	assert(value);
	set_free((Set *) value);
}

/* misc utils */

size_t
__filter_size(size_t numel) {
	/* TODO optimize based on numel */
	return numel;
}

static Set *
__insert_new_set(RECORD *r, ENTRY *e)
{
	Set *s;
	int result;
	assert(r && e);
	s = set_new(&__wrap_hash_path, &__wrap_equals_path);
	assert(s);
	/* set_register_free_function(s, &free_entry); */
	result = set_insert(s, e);
	assert(result);
	result = hash_table_insert(r->table, e, s);
	assert(result);
	return s;
}

enum result_t {
	/*
	FITLER_HIT  = 0xFF, // Perhaps already seen
	TABLE_MISS  = 0xF0, // Absent from table
	TABLE_HIT   = 0x00, // Definitely already seen
	FILTER_MISS = 0x00, // Definitely not already seen
	MASK        = 0xFF
	*/
	INSERTED,
	NOT_INSERTED,
	PRESENT
};

/*
static result_t
__lookup(RECORD *r, ENTRY *e, set **s) {
	assert(r && e && s);
	if (!bloom_filter_query(r->filter, e)) {
		bloom_filter_insert(r->filter, e); // TODO assert?
		*s = __insert_new_set(r, e);
		return FILTER_MISS;
	}
	*s = hash_table_lookup(r->table, e);
	if (*s != HASH_TABLE_NULL) {
		return set_insert(*s, e) ?
			INSERTED : NOT_INSERTED;
	}
	*s = __insert_new_set(r, e);
	return result;
}
*/

/* FIXME FIXME FIXME */

static enum result_t
__lookup(RECORD *r, ENTRY *e, Set **s) {
	assert(r && e && s);
	/* handle a possible hit */
	if (bloom_filter_query(r->filter, e)) {
		*s = hash_table_lookup(r->table, e);
		if (*s != HASH_TABLE_NULL) {
			return set_insert(*s, e) ?
				INSERTED : NOT_INSERTED;
		}
		/* could be a false-positive */
	}
	*s = __insert_new_set(r, e);
	return 0;
}

/* user calls this, true if? */

inline int
record(RECORD *r, ENTRY *e)
{
	Set *s;
	assert(r && e);
	switch (__lookup(r, e, &s)) {
	default: return 0;
	}
	bloom_filter_insert(r->filter, e);
	/* TODO */
}

/* exported */

inline RECORD *
malloc_record(size_t n)
{
	RECORD *record;
	if (!(record = malloc(sizeof(RECORD)))) {
		/* FIXME malloc failed */
		return NULL;
	}
	memset(record, 0x00, sizeof(RECORD));
	record->filter = bloom_filter_new(__filter_size(n), &__wrap_hash_data, 5);
	record->table = hash_table_new(&__wrap_hash_data, &__wrap_equals_data);
	hash_table_register_free_functions(record->table, NULL, &__wrap_free_set);
	return record;
}

inline void
free_record(struct record_t *record)
{
	assert(record);
	bloom_filter_free(record->filter);
	hash_table_free(record->table);
	free(record);
}

/* FIXME: utilize visitor return value */

inline void
visit_sets(RECORD *r, VISITOR v)
{
	HashTableIterator iter;
	assert(r && v);
	hash_table_iterate(r->table, &iter);
	while (hash_table_iter_has_more(&iter)) {
		(*v)(hash_table_iter_next(&iter));
	}
}

extern void
visit_each(RECORD *r, VISITOR v)
{
	SetIterator set_iter;
	HashTableIterator ht_iter;
	assert(r && v);
	hash_table_iterate(r->table, &ht_iter);
	while (hash_table_iter_has_more(&ht_iter)) {
		set_iterate(hash_table_iter_next(&ht_iter), &set_iter);
		while (set_iter_has_more(&set_iter)) {
			(*v)(set_iter_next(&set_iter));
		}
	}
}
