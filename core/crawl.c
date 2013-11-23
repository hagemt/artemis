#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LART_PUBLIC void
free_entry(Entry *entry)
{
	assert(entry && entry->path);
	free(entry->path);
	entry->path = NULL;
	if (entry->data) {
		free(entry->data);
	}
	free(entry);
	/* TODO(teh) other fields? */
}

static void
__ensure_data(Entry *entry)
{
	FILE *file;
	size_t bytes;
	assert(entry && entry->data);
	if (!(file = fopen(entry->path, "r"))) {
		/* FIXME fopen failed */
		return;
	}
	bytes = fread(entry->data, DAT_LEN, 1, file);
	if (bytes != DAT_LEN) {
		/* FIXME bytes lost */
	}
	if (fclose(file)) {
		/* FIXME fclose failed */
	}
}

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

/* FIXME(teh) fix portability for header */
LART_PUBLIC Entry *
malloc_entry(char *path, struct stat *info)
{
	size_t len;
	struct entry_t *entry;
	assert(path && info);
	if (!(entry = malloc(sizeof(struct entry_t)))) {
		/* FIXME malloc failed */
		return NULL;
	}
	memset(entry, 0x00, sizeof(struct entry_t));
	/* copy the path over */
	len = strnlen(path, LART_PATH_MAX) + 1;
	if ((entry->path = malloc(len * sizeof(char_t)))) {
		strncpy(entry->path, path, len);
		entry->path[len - 1] = '\0';
	}
	/* set up addtional fields */
	entry->atime = info->st_atime;
	entry->ctime = info->st_ctime;
	entry->mtime = info->st_mtime;
	entry->size = info->st_size;
	if (S_ISREG(info->st_mode)) {
		entry->type = REGULAR;
	} else if (S_ISDIR(info->st_mode)) {
		entry->type = DIRECTORY;
	} else {
		entry->type = OTHER;
	}
	if ((entry->data = malloc(DAT_LEN))) {
		__ensure_data(entry);
	}
	return entry;
}
