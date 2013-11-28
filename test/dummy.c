#include <assert.h>

#include "libartemis/entry.h"

#define LART_PUBLIC

LART_PUBLIC Entry *
malloc_entry(char *path, void *data) {
	assert(path && data);
	return NULL;
}

LART_PUBLIC void
free_entry(Entry *entry) {
	assert(entry);
}

LART_PUBLIC void
visit(char *path, Callback callback) {
	assert(path && callback);
}
