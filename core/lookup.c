#include "libartemis/record.h"

__lookup(Record *r, Entry *e, Set **s) {
LART_PRIVATE size_t
__filter_size(size_t numel) {
	/* TODO optimize based on numel */
	return numel;
}

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>
#include <libcalg/set.h>

#include "artemis_wrap.h"

static Set *
__insert_new_set(Record *r, Entry *e)
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
	TABLE_HIT   = 0x0F, // Definitely already seen
	FILTER_MISS = 0x00, // Definitely not already seen
	MASK        = 0xFF
	*/
	INSERTED,
	NOT_INSERTED,
	PRESENT
};

/*
static result_t
__lookup(Record *r, Entry *e, set **s) {
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

LART_PRIVATE enum result_t
__lookup(Record *r, Entry *e, Set **s) {
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
