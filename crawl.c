#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "crawl.h"

static void
__data_entry(struct entry_t *entry)
{
	FILE *file; size_t bytes;
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

inline struct entry_t *
malloc_entry(char_t *path, struct stat *info)
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
	len = strnlen(path, MAX_LEN) + 1;
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
		__data_entry(entry);
	}
	return entry;
}

inline void
free_entry(struct entry_t *entry)
{
	assert(entry && entry->path);
	free(entry->path);
	entry->path = NULL;
	if (entry->data) {
		free(entry->data);
	}
	free(entry);
	/* TODO other fields? */
}

/* non-entry */

static void
__expand(char_t *path, callback_t func)
{
	DIR *dir;
	char_t buffer[MAX_LEN];
	struct dirent *entry;
	if (!(dir = opendir(path))) {
		/* FIXME opendir failed */
		return;
	}
	while ((entry = readdir(dir))) {
		/* FIXME more elegantly? */
		if (entry->d_name[0] != '.') {
			snprintf(buffer, MAX_LEN, "%s/%s", path, entry->d_name);
			buffer[MAX_LEN - 1] = '\0';
			visit(buffer, func);
		}
	}
	if (closedir(dir)) {
		/* FIXME closedir failed */
	}
}

inline void
visit(char_t *path, callback_t func)
{
	/* FIXME this is not safe: */
	static struct stat info;
	if (lstat(path, &info)) {
		/* FIXME lstat failed */
		return;
	}
	if (S_ISREG(info.st_mode)) {
		(*func)(malloc_entry(path, &info));
	} else if (S_ISDIR(info.st_mode)) {
		__expand(path, func);
	}
}

