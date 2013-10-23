#ifndef __RECORD_H__
#define __RECORD_H__

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>
#include <libcalg/set.h>

struct record_t {
	BloomFilter filter;
	Hashtable table;
};

typedef struct record_t RECORD;

extern RECORD *
malloc_record(size_t);

extern void
free_record(RECORD *);

#endif /* __RECORD_H__ */
