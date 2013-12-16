#include "artemis_util.h"

#include <assert.h>

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>

/* FIXME can some things be constant? */
LART_PRIVATE QueryResult
query(Record *r, Entry *e, Set **s) {
	QueryResult result = QUERY_TOTAL_MISS;
	assert(r && e && s);
	/* do a quick (non-deterministic) check */
	if (bloom_filter_query(r->filter, e)) {
		/* the filter says we may know this */
		result = QUERY_FILTER_HIT;
		/* so now check the table (authoritative) */
		*s = hash_table_lookup(r->table, e);
		/* check for false-positive (null) */
		if (*s != HASH_TABLE_NULL) {
			/* this isn't a false-positive */
			result = QUERY_TABLE_HITS;
		}
	}
	/* notice r is not updated */
	return result;
	/* TODO(teh) add metrics */
}
