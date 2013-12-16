#ifndef __LART_RECORD_H__
#define __LART_RECORD_H__

#include <libcalg/bloom-filter.h>
#include <libcalg/hash-table.h>

/*
 * Holds references to active entries.
 * The filter holds quick references to the "short" hash. (shash)
 * When short hashes collide, it triggers a full hash of the entry.
 * The table relates full hashes (keyed on entry) to sets of known duplicates.
 * Entries are only hashed, never freed by this interface.
 */
struct record_t {
	BloomFilter *filter;
	HashTable *table;
};

typedef struct record_t Record;

#include <stddef.h>

/* Public API */

#include "libartemis/constants.h"

LART_PUBLIC Record *
record_new(size_t);

LART_PUBLIC void
record_free(Record *);

/* Bulk Operations */

typedef void (*Visitor)(void *);

LART_PUBLIC void
visit_sets(Record *, Visitor);

LART_PUBLIC void
visit_each(Record *, Visitor);

/* TODO(teh): join functionality */
typedef void * (*Joiner)(void *, void *);

/* The whole point: */

#include "libartemis/entry.h"

LART_PUBLIC int
record_entry(Record *, Entry *);

#endif /* __LART_RECORD_H__ */
