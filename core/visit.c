#include <assert.h>

#include <libcalg/hash-table.h>

#include "libartemis/record.h"

LART_INLINE static void
iterate_table(HashTable *, Visitor, int);

/* Public API functions */

LART_PUBLIC void
visit_sets(Record *r, Visitor v)
{
	assert(r && v);
	iterate_table(r->table, v, 1);
}

LART_PUBLIC void
visit_each(Record *r, Visitor v)
{
	assert(r && v);
	iterate_table(r->table, v, 0);
}

/* TODO(teh): utilize visitor return value? */

#include <libcalg/set.h>

LART_INLINE static void
iterate_set(Set *s, Visitor v)
{
	SetIterator iter;
	set_iterate(s, &iter);
	while (set_iter_has_more(&iter)) {
		(*v)(set_iter_next(&iter));
	}
}

LART_INLINE static void
iterate_table(HashTable *table, Visitor v, int once)
{
	HashTableIterator iter;
	assert(table);
	hash_table_iterate(table, &iter);
	while (hash_table_iter_has_more(&iter)) {
		if (once) {
			(*v)(hash_table_iter_next(&iter));
		} else {
			iterate_set(hash_table_iter_next(&iter), v);
		}
	}
}
