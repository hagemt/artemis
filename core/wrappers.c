#include <assert.h>

#include "libartemis/entry.h"

LART_INLINE static Entry *
as_entry(void *ptr)
{
	assert(ptr);
	return (Entry *) ptr;
}

#include <libcalg/compare-string.h>
#include <libcalg/hash-string.h>
#include <libcalg/hash-table.h>

LART_PRIVATE unsigned long
wrap_hash_entry_data(HashTableKey key)
{
	void *data;
	size_t bytes;
	unsigned long result = 0;
	data = as_entry(key)->data;
	/* TODO(teh) better hash function for binary data */
	for (bytes = 0; bytes < LART_ENTRY_DLEN; bytes += sizeof(unsigned long)) {
		result ^= * (unsigned long *) data;
		data += sizeof(unsigned long);
	}
	return result;
}

LART_PRIVATE unsigned long
wrap_hash_entry_path(HashTableKey key)
{
	/* TODO(teh) consider using nocase */
	return string_hash(as_entry(key)->path);
}

#include <string.h>

LART_PRIVATE int
wrap_equals_entry_data(HashTableKey k1, HashTableKey k2)
{
	return memcmp(as_entry(k1)->data, as_entry(k2)->data, LART_ENTRY_DLEN);
}

LART_PRIVATE int
wrap_equals_entry_path(HashTableKey k1, HashTableKey k2)
{
	return string_equal(as_entry(k1)->path, as_entry(k2)->path);
}
