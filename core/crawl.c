#include "libartemis/entry.h"

#include <assert.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

static void
__expand(char *path, Callback func)
{
	DIR *dir;
	struct dirent *entry;
	char buffer[LART_PATH_MAX];
	if (!(dir = opendir(path))) {
		/* FIXME opendir failed */
		return;
	}
	while ((entry = readdir(dir))) {
		/* FIXME more elegantly? */
		if (entry->d_name[0] != '.') {
			snprintf(buffer, LART_PATH_MAX, "%s/%s", path, entry->d_name);
			buffer[LART_PATH_MAX - 1] = '\0';
			visit(buffer, func);
		}
	}
	if (closedir(dir)) {
		/* FIXME closedir failed */
	}
}

LART_PUBLIC void
visit(char *path, Callback func)
{
	struct stat info;
	assert(path && func);
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
