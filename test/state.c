#include "artemis.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

static struct {
	Node root;
	size_t size;
	Record *store;
} __state = {
	{ NULL, NULL }, 0, NULL
};

LART_INLINE static void
__state_lock(void *);

LART_INLINE static void
__state_unlock(void *);

static int
__compare_nodes_by_entry_path(const void *, const void *);

/* Private API */

LART_PRIVATE size_t
state_count(void)
{
	return __state.size;
}

LART_PRIVATE void
state_enter(Entry *entry)
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
	__state_lock(&__state);
	if (!__state.store) {
		__state.store = record_new(1 << 20);
	}
	node->tail = __state.root.tail;
	__state.root.tail = node;
	++__state.size;
	__state_unlock(&__state);
}

#include <stdio.h>

LART_PRIVATE void
state_dump(void *target)
{
	FILE *file;
	Node *node, **index;
	size_t i, n, bytes, a, b;
	assert(target);
	file = (FILE *) target;
	__state_lock(&__state);

	/* We'll track total size and individual counts */
	bytes = a = b = i = n = 0;
	for (node = &__state.root; (node = node->tail); ++n);
	assert(__state.size == n);

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
			"[DEBUG] %.2f KB in %lu entities (%lu regular files, %lu others)\n",
			((double) bytes) / 1024, n, a, b);

	/* Destroy children of root */
	while ((node = __state.root.tail)) {
		__state.root.tail = node->tail;
		entry_free(node->head);
		free(node);
		--__state.size;
	}
	assert(__state.size == 0);
	assert(__state.root.head == NULL);
	assert(__state.root.tail == NULL);
	if (__state.store) {
		record_free(__state.store);
	}
	__state_unlock(&__state);
	free(index);
}

/* extraneous functions */

#include <string.h>

static int
__compare_nodes_by_entry_path(const void *n1, const void *n2)
{
	Entry *e1, *e2;
	assert(n1 && n2);
	e1 = (* (Node **) n1) -> head;
	e2 = (* (Node **) n2) -> head;
	return strncmp(e1->path, e2->path, LART_PATH_MAX);
}

/* for synchronicity */

#include <pthread.h>

static pthread_mutex_t
__state_mutex = PTHREAD_MUTEX_INITIALIZER;

static void
__state_lock(void *state)
{
	assert(state);
	pthread_mutex_lock(&__state_mutex);
}

static void
__state_unlock(void *state)
{
	assert(state);
	pthread_mutex_unlock(&__state_mutex);
}
