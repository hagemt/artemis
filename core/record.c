#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>
#include <libcalg/set.h>

#include "libartemis/record.h"

#include "artemis_wrap.h"
#include "artemis_util.h"

LART_PUBLIC Record *
malloc_record(size_t n)
{
	if (!(record = malloc(sizeof(Record)))) {
		/* FIXME malloc failed */
		return NULL;
	}
	memset(record, 0x00, sizeof(Record));
	record->filter = bloom_filter_new(__filter_size(n), &__wrap_hash_data, 5);
	record->table = hash_table_new(&__wrap_hash_data, &__wrap_equals_data);
	hash_table_register_free_functions(record->table, NULL, &free_set);
	return record;
}

LART_PUBLIC void
free_record(Record *record)
{
	assert(record);
	bloom_filter_free(record->filter);
	hash_table_free(record->table);
	free(record);
}

/* FIXME: utilize visitor return value */

LART_PUBLIC void
visit_sets(Record *r, Visitor v)
{
	HashTableIterator iter;
	assert(r && v);
	hash_table_iterate(r->table, &iter);
	while (hash_table_iter_has_more(&iter)) {
		(*v)(hash_table_iter_next(&iter));
	}
}

LART_PUBLIC void
visit_each(Record *r, Visitor v)
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

/* TODO(teh) */

LART_PUBLIC int
record_entry(Record *r, Entry *e)
{
	Set *s;
	assert(r && e);
	switch (__lookup(r, e, &s)) {
	default: return 0;
	}
	bloom_filter_insert(r->filter, e);
	/* TODO */
}
