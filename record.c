#include "record.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

size_t
__filter_size(size_t numel) {
	/* TODO optimize based on numel */
	return n;
}

RECORD *
malloc_record(size_t n)
{
	struct record_t *record;
	size_t filter_size = __filter_size(n);
	if (!(record = malloc(sizeof(struct record_t)))) {
		/* FIXME malloc failed */
		return NULL;
	}
	memset(record, 0x00, sizeof(struct record_t));
	record->filter = bloom_table_new();
	record->table = hash_table_new();
	return record;
}

void
free_record(struct record_t *record)
{
	assert(record);
	bloom_table_free(record->filter);
	hash_table_free(record->table);
	free(record);
}
