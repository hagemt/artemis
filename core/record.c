#include "artemis_util.h"

#include <assert.h>
/* TODO(teh): replace with logging */
#include <stdio.h>

LART_PUBLIC int
record_entry(Record *r, Entry *e)
{
	Set *s;
	int result = 0;
	assert(r && e);
	switch (query(r, e, &s)) {
	case QUERY_TABLE_HITS:
		result = set_insert(s, e);
		assert(result == 0);
		break;
	case QUERY_FILTER_HIT:
		fprintf(stderr, "[] %s\n", "cache hit!");
	default:
		assert(s == SET_NULL);
		s = singleton(r, e);
		assert(s != SET_NULL);
	}
	return result;
}
