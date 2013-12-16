#ifndef __ARTEMIS_UTIL_H__
#define __ARTEMIS_UTIL_H__

#include "libartemis/constants.h"

#include <stddef.h>

LART_PRIVATE size_t
identity(size_t);

LART_PRIVATE size_t
square(size_t);

/* TODO(teh): write real recommenders ^ */

enum query_result_t {
	/*
	FILTER_MISS = 0x00, // Definitely not already seen (will record in table)
	FITLER_HIT  = 0xF0, // Perhaps already seen (might be false-positive)
	TABLE_HIT   = 0x0F, // Definitely already seen (it's a collision)
	*/
	QUERY_TOTAL_MISS = 0x00,
	QUERY_FILTER_HIT = 0x01,
	QUERY_TABLE_HITS = 0x02,
	QUERY_VALUES_MAX = 0x03
};

typedef enum query_result_t QueryResult;

#include <libcalg/set.h>

#include "libartemis/entry.h"
#include "libartemis/record.h"

LART_PRIVATE QueryResult
query(Record *, Entry *, Set **);

LART_PRIVATE Set *
singleton(Record *, Entry *);

#endif /* __ARTEMIS_UTIL_H__ */
