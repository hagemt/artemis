#include <assert.h>
#include <stdlib.h>

#include "libartemis/entry.h"

LART_PUBLIC Entry *
entry_new(char *path, void *data)
{
	Entry *entry;
	assert(path && data);
	entry = malloc(sizeof(Entry));
	assert(entry);
	return entry;
}

LART_PUBLIC void
entry_free(Entry *entry)
{
	assert(entry);
	free(entry);
	entry = NULL;
}

#include <stdio.h>

LART_PUBLIC void
entry_crawl(char *path, EntryCrawlback hook)
{
	assert(path && callback);
	printf(stderr, "[%s] %s (%s)\n",
			__PRETTY_FUNCTION__, path, "dummy visitor");
}
