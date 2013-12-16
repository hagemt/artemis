#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pulls in everything we need from libartemis */
#include "libartemis/entry.h"

/* pulls in node decl */
#include "artemis.h"

static int
__compare_nodes_by_entry_path(const void *n1, const void *n2)
{
	Entry *e1, *e2;
	assert(n1 && n2);
	e1 = (* (Node **) n1) -> head;
	e2 = (* (Node **) n2) -> head;
	return strncmp(e1->path, e2->path, LART_PATH_MAX);
}

static void __lock_state(void *);
static void __unlock_state(void *);

static struct {
	Node root;
	size_t size;
} __state = {
	{ NULL, NULL }, 0
};

LART_PRIVATE size_t
count_state(void)
{
	return __state.size;
}

LART_PRIVATE void
enter_state(Entry *entry)
{
	Node *node;
	assert(entry);
	/* entry should be ready, just create a node to hold it */
	if (!(node = malloc(sizeof(Node)))) {
		/* FIXME malloc failed, pass */
		return;
	}
	node->head = entry;
	/* and track it, within the global state */
	__lock_state(&__state);
	node->tail = __state.root.tail;
	__state.root.tail = node;
	++__state.size;
	__unlock_state(&__state);
}

LART_PRIVATE void
dump_state(void *where)
{
	FILE *file;
	Node *node, **index;
	size_t i, n, bytes, a, b;
	assert(where);
	file = (FILE *) where;
	__lock_state(&__state);

	/* We'll track total size and individual counts */
	bytes = a = b = i = n = 0;
	for (node = &__state.root; (node = node->tail); ++n);

	/* create an index of entries in path-sorted order */
	if (!(index = malloc(sizeof(node) * n))) {
		/* FIXME malloc failed */
		return;
	}
	for (node = &__state.root; (node = node->tail); ++i) {
		index[i] = node;
	}
	qsort(index, n, sizeof(node), &__compare_nodes_by_entry_path);

	/* Provide some aggregated info */
	for (i = 0; i < n; ++i) {
		bytes += index[i]->head->size;
		switch (index[i]->head->type) {
		case LART_FTYPE_REG: ++a; break;
		case LART_FTYPE_DIR: ++b; break;
		default:
			/* FIXME use log functions! */
			fprintf(file, "[%s] %s (%s)\n",
					("PATH"), index[i]->head->path, ("unknown type"));
		}
	}
	fprintf(file,
			"[INFO] %.2f KB in %lu entities (%lu regular files, %lu others)\n",
			((double) bytes) / 1024, n, a, b);

	/* Destroy children of root */
	while ((node = __state.root.tail)) {
		__state.root.tail = node->tail;
		free_entry(node->head);
		#ifndef NDEBUG
		node->head = NULL;
		node->tail = NULL;
		#endif /* DEBUG */
		free(node);
	}
	/* Cleanup */
	__state.root.head = NULL;
	__state.root.tail = NULL;
	__state.size = 0;
	free(index);
	__unlock_state(&__state);
}

#include <pthread.h>

static pthread_mutex_t
__lock = PTHREAD_MUTEX_INITIALIZER;

static void
__lock_state(void *state)
{
	assert(state);
	pthread_mutex_lock(&__lock);
}

static void
__unlock_state(void *state)
{
	assert(state);
	pthread_mutex_unlock(&__lock);
}
