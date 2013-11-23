#ifndef __RECORD_H__
#define __RECORD_H__

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>
#include <libcalg/set.h>

/*
 * Holds references to active entries.
 * The filter holds quick references to the "short" hash. (shash)
 * When short hashes collide, it triggers a full hash of the entry.
 * The table relates full hashes (keyed on entry) to sets of known duplicates.
 * Entries are only hashed, never freed by this interface.
 * However, the table' duplicate must be freed.
 */
struct record_t {
	BloomFilter *filter;
	HashTable *table;
};

typedef struct record_t RECORD;

extern RECORD *
malloc_record(size_t);

extern void
free_record(RECORD *);

typedef void (*VISITOR)(void *);
/* typedef void *(*combine_t)(void *, void *); */

extern void
visit_sets(RECORD *, VISITOR);

extern void
visit_each(RECORD *, VISITOR);

#endif /* __RECORD_H__ */
