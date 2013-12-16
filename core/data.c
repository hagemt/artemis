#include "libartemis/entry.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

LART_INLINE static ssize_t
read_data(void *, size_t, size_t, char *);

#define MIN(x,y) ((x < y) ? x : y)

LART_PRIVATE void
fill_data(Entry *entry)
{
	ssize_t result, expected = MIN(LART_ENTRY_DLEN, entry->size);
	assert(entry && entry->path && entry->data);
	if (expected > 0) {
		/* result = read_data(entry->data, expected, 1, entry->path); */
		/* FIXME: assert(result == expected); WTF? */
	}
}

LART_PRIVATE void
hash_data(Entry *entry)
{
	assert(entry);
	/* TODO(teh): full hash */
}

LART_PRIVATE void
free_data(Entry *entry)
{
	assert(entry);
	if (entry->data) {
		free(entry->data);
	}
}

#include <stdio.h>

LART_INLINE static ssize_t
read_data(void *target, size_t bytes, size_t times, char *path)
{
	FILE *file;
	size_t result;
	assert(target && path);
	if (!(file = fopen(path, "r"))) {
		/* FIXME fopen failed */
		return -1;
	}
	result = fread(target, bytes, times, file);
	/* FIXME assert(result == times); WTF? */
	if (fclose(file)) {
		/* FIXME fclose failed */
	}
	return - (ssize_t) (bytes < result) ?
		(result - bytes) : (bytes - result);
}
