#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static unsigned long
bytes_per_page(void)
{
	return (unsigned long) sysconf(_SC_PAGE_SIZE);
}

#include "log.h"

#define MAP_PROT (PROT_READ | PROT_WRITE)
#define MAP_ARGS (MAP_SHARED)
#define PATH_MAX 1024

static struct {
	char *path;
	void *file;
} __state = { NULL, NULL };

static int
on(char *path, off_t size)
{
	int fd;
	size_t len;
	assert(path);
	len = strnlen(path, PATH_MAX);
	__state.path = calloc(len, sizeof(char));
	if (!__state.path) {
		return 1;
	}
	strncpy(__state.path, path, len);
	fd = open(__state.path, O_RDWR | O_LARGEFILE, (mode_t) 0666);
	if (fd < 0) {
		/* FIXME open failed */
		free(__state.path);
		__state.path = NULL;
		return 1;
	}
	__state.file = mmap(NULL, size, MAP_PROT, MAP_ARGS, fd, 0);
	if (!__state.file) {
		free(__state.path);
		__state.path = NULL;
		return 1;
	}
	if (close(fd)) {
		/* FIXME close failed */
	}
	memset(__state.file, 0x01, 100);
	return 0;
}

static int
off(char *path, off_t size)
{
	assert(path && __state.path);
	if (strncmp(path, __state.path, PATH_MAX) == 0) {
		assert(__state.file);
		free(__state.path);
		__state.path = NULL;
		if (munmap(__state.file, size)) {
			/* FIXME munmap failed */
		}
		__state.file = NULL;
	}
	return 0;
}

#define TARGET "disk.db"

int
main(void)
{
	struct stat info;
	if (lstat(TARGET, &info) || !S_ISREG(info.st_mode)) {
		return EXIT_FAILURE;
	}
	if (on(TARGET, info.st_size) || off(TARGET, info.st_size)) {
		return EXIT_FAILURE;
	}
	LOGW("System page size is %lu bytes.", bytes_per_page());
	return EXIT_SUCCESS;
}
