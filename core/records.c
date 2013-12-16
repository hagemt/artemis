#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>

#include "libartemis/entry.h"
#include "libartemis/record.h"

static void
wrap_entry_set_free(HashTableValue value);

#include "artemis_wrap.h"

LART_PUBLIC Record *
record_new(size_t n)
{
	Record *record;
	if (!(record = malloc(sizeof(Record)))) {
		/* FIXME malloc failed */
		return NULL;
	}
	memset(record, 0x00, sizeof(Record));
	/* TODO(teh) pick/write f(n) instead of n for filter size: */
	record->filter =
		bloom_filter_new(n, &wrap_hash_entry_data, LART_HASH_DIM);
	record->table =
		hash_table_new(&wrap_hash_entry_data, &wrap_equals_entry_data);
	hash_table_register_free_functions(record->table,
			NULL, /* entry-keys are not freed, but sets are: */
			&wrap_entry_set_free);
	return record;
}

LART_PUBLIC void
record_free(Record *record)
{
	assert(record && record->filter && record->table);
	bloom_filter_free(record->filter);
	hash_table_free(record->table);
	free(record);
}

#include <libcalg/set.h>

static void
wrap_entry_set_free(HashTableValue value) {
	assert(value);
	set_free((Set *) value);
}
