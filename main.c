#include <assert.h>

#include "crawl.h"

static NODE
__root = { NULL, NULL };

static void
__handle_entry(ENTRY *entry)
{
	NODE *node;
	assert(entry);
	if (!(node = malloc(sizeof(NODE)))) {
		/* FIXME malloc failed */
		return;
	}
	node->head = entry;
	node->tail = __root.tail;
	__root.tail = node;
	++__root.head;
	/* FIXME may be a terrible idea */
}

static void *
__handle_thread(void *data) {
	visit((char *) data, &__handle_entry);
	return data;
}

#include <stdlib.h>
#include <stdio.h>

extern void
dump(FILE *, NODE *);

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

static const char *
__glyphs = "/-\\|";

int
main(int argc, char *argv[])
{
	int i;
	pthread_t tid;
	/* TODO pthread_attr? */
	struct stat info;
	while (--argc) {
		if (lstat(argv[argc], &info)) {
			/* FIXME lstat failed */
			continue;
		}
		if (pthread_create(&tid, NULL, &__handle_thread, argv[argc])) {
			/* FIXME pthread_create failed */
			continue;
		}
		while (pthread_tryjoin_np(tid, NULL)) {
			i = ++i % sizeof(__glyphs);
			fprintf(stderr, "[%c] Crawling '%s' ...\r", __glyphs[i], argv[argc]);
		}
		dump(stderr, &__root);
	}
	return EXIT_SUCCESS;
}
