#include "libartemis/entry.h"

#include "artemis_data.h"

#include <assert.h>
#include <stdlib.h>

LART_PUBLIC void
entry_free(Entry *entry)
{
	assert(entry && entry->path);
	/* TODO(teh) log this call */
	free_data(entry);
	entry->data = NULL;
	free(entry->path);
	entry->path = NULL;
	free(entry);
	/* TODO(teh) other fields? */
}

#include <string.h>

/* FIXME(teh) make 100% portable? */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

LART_PUBLIC Entry *
entry_new(char *path, void *data)
{
	size_t len;
	Entry *entry;
	struct stat *info = (struct stat *) data;
	assert(path && info);
	if (!(entry = malloc(sizeof(Entry)))) {
		/* FIXME malloc failed */
		return NULL;
	}
	memset(entry, 0x00, sizeof(Entry));
	/* copy the path over */
	len = strnlen(path, LART_PATH_MAX) + 1;
	if ((entry->path = malloc(len * sizeof(char)))) {
		strncpy(entry->path, path, len);
		entry->path[len - 1] = '\0';
	}
	/* set up addtional fields */
	entry->atime = info->st_atime;
	entry->ctime = info->st_ctime;
	entry->mtime = info->st_mtime;
	entry->size = info->st_size;
	if (S_ISREG(info->st_mode)) {
		entry->type = LART_FTYPE_REG;
	} else if (S_ISDIR(info->st_mode)) {
		entry->type = LART_FTYPE_DIR;
	} else {
		entry->type = LART_FTYPE_MAX;
	}
	/* delegate to handle data */
	if ((entry->data = malloc(LART_ENTRY_DLEN))) {
		fill_data(entry);
	}
	return entry;
}
