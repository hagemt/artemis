#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "set.h"

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

void
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

void
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

/* utility code */

static int
__compare_entry_nodes(const void *p1, const void *p2)
{
	struct entry_t *e1, *e2;
	assert(p1 && p2);
	e1 = (* (struct node_t **) p1) -> head;
	e2 = (* (struct node_t **) p2) -> head;
	return strncmp(e1->path, e2->path, MAX_LEN);
}

static void
__dump(FILE *fp, struct node_t *root)
{
	struct node_t *node, **index;
	size_t i, n, bytes, files, dirs;
	assert(fp && root);
	bytes = files = dirs = i = n = 0;
	/* create an index of entries in path-sorted order */
	for (node = root; (node = node->tail); ++n);
	if (!(index = malloc(sizeof(root) * n))) {
		/* FIXME malloc failed */
		return;
	}
	for (node = root; (node = node->tail); ++i) {
		index[i] = node;
	}
	qsort(index, n, sizeof(node), &__compare_entry_nodes);
	/* destroy everything after dumping path info */
	for (i = 0; i < n; ++i) {
		bytes += index[i]->head->size;
		switch (index[i]->head->type) {
		case REGULAR: ++files; break;
		case DIRECTORY: ++dirs; break;
		default:
			fprintf(fp, "[PATH] %s\n", index[i]->head->path);
		}
	}
	fprintf(fp, "[INFO] %.2f KB in %lu entities (%lu files, %lu dirs)\n",
			((double) bytes) / 1024, n, files, dirs);
	while ((node = root->tail)) {
		root->tail = node->tail;
		free_entry(node->head);
		node->tail = NULL;
		free(node);
	}
	free(index);
}

static int
__uid(void)
{
	uid_t uid = getuid();
	printf("[INFO] uid '%d' (before setuid)\n", uid);
	if (setuid(0)) {
		fprintf(stderr, "[ERROR] uid '%d' (couldn't setuid to root)\n", uid);
	}
	printf("[INFO] uid '%d' (after setuid)\n", getuid());
	if (setuid(uid)) {
		fprintf(stderr, "[WARNING] uid '%d' (couldn't restore uid)\n", getuid());
	}
	return EXIT_SUCCESS;
}

/* program logic */

static struct node_t __root = { NULL, NULL };

static void
__handle(struct entry_t *entry)
{
	struct node_t *node;
	assert(entry);
	if (!(node = malloc(sizeof(struct node_t)))) {
		/* FIXME malloc failed */
		return;
	}
	node->head = entry;
	node->tail = __root.tail;
	__root.tail = node;
}

int
main(int argc, char *argv[])
{
	struct stat info;
	while (--argc) {
		if (lstat(argv[argc], &info)) {
			/* FIXME lstat failed */
			continue;
		}
		__expand(argv[argc], &__handle);
	}
	__dump(stderr, &__root);
	return __uid();
}
