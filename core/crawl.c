#include "libartemis/entry.h"

#include <assert.h>

static void
expand_path(char *, EntryCrawlback);

static void
entry_crawlback(Entry *entry)
{
	assert(entry);
	/* TODO(teh): expand stub */
}

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

LART_PUBLIC void
entry_crawl(char *path, EntryCrawlback hook)
{
	Entry *entry;
	struct stat info;
	assert(path);
	if (lstat(path, &info)) {
		/* FIXME lstat failed */
		return;
	}
	/* recursive case: expand on this path */
	if (S_ISDIR(info.st_mode)) {
		expand_path(path, hook);
		return;
	}
	/* leaf case: create an entry and crawlback */
	entry = entry_new(path, &info);
	if (hook) {
		(*hook)(entry);
		return;
	}
	/* FIXME bad idea? */
	entry_crawlback(entry);
	entry_free(entry);
}

#include <stdio.h>

#include <dirent.h>

static void
expand_path(char *path, EntryCrawlback func)
{
	DIR *dir;
	struct dirent *node;
	/* TODO(teh): profile this */
	char buffer[LART_PATH_MAX];
	if (!(dir = opendir(path))) {
		/* FIXME opendir failed */
		return;
	}
	while ((node = readdir(dir))) {
		/* FIXME more elegant check? */
		if (node->d_name[0] != '.') {
			snprintf(buffer, LART_PATH_MAX, "%s/%s", path, node->d_name);
			buffer[LART_PATH_MAX - 1] = '\0';
			entry_crawl(buffer, func);
		}
	}
	if (closedir(dir)) {
		/* FIXME closedir failed */
	}
}
