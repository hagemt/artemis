#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "crawl.h"

static int
__compare_entry_nodes(const void *p1, const void *p2)
{
	ENTRY *e1, *e2;
	assert(p1 && p2);
	e1 = (* (NODE **) p1) -> head;
	e2 = (* (NODE **) p2) -> head;
	return strncmp(e1->path, e2->path, MAX_LEN);
}

inline void
dump(FILE *fp, NODE *root)
{
	NODE *node, **index;
	size_t i, n, bytes, files, dirs;
	assert(fp && root);
	/* We'll track total size and entity counts */
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

	/* dump various bits of information */
	for (i = 0; i < n; ++i) {
		bytes += index[i]->head->size;
		switch (index[i]->head->type) {
		case REGULAR: ++files; break;
		case DIRECTORY: ++dirs; break;
		default:
			/* FIXME use log functions! */
			fprintf(fp, "[%s] %s (%s)\n",
					_("PATH"), index[i]->head->path, _("unknown type"));
		}
	}
	fprintf(fp, "[INFO] %.2f KB in %lu entities (%lu files, %lu dirs)\n",
			((double) bytes) / 1024, n, files, dirs);

	/* Cleanup */
	while ((node = root->tail)) {
		root->tail = node->tail;
		free_entry(node->head);
		#ifndef NDEBUG
		node->head = NULL;
		node->tail = NULL;
		#endif /* DEBUG */
		free(node);
	}
	root->head = NULL;
	root->tail = NULL;
	free(index);
}
