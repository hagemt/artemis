#include <assert.h>
#include <stdlib.h>

#include "artemis.h"
#include "block.h"
#define TPOOL_INTERNAL
#include "timpl.h"

static struct {
	pthread_t runner;
	struct tattr_t *state;
} __thread_pool = { 0, NULL };
/* PTHREAD_..._INITIALIZER? */

/* FIXME both of these: */

static ThreadWorker
__thread_event = NULL;

/* these aux. are fine for now */

LART_INLINE static int
__tpool_create(int size) {
	assert(size > 0 && __thread_pool.state == NULL);
	/* FIXME pshared has wrong value? */
	if (sem_init(&__thread_count, 0, size)) {
		return -1;
	}
	if (runner_start(&__thread_pool.runner, TPOOL_DEFAULT)) {
		/* FIXME runner failed to start */
		return 1;
	}
	/* now all that's left is to set up this */
	__thread_pool.state = calloc(sizeof(struct tattr_t), 1);
	/* TODO(teh): initialize state */
	assert(__thread_pool.state);
	if (__thread_event) {
		__thread_event("thread pool created");
	}
	return 0;
}

LART_INLINE static int
__tpool_destroy(void *pool)
{
	void *retval;
	assert(pool && __thread_pool.state);
	/* kill off the runner */
	if (runner_stop(&__thread_pool.runner, &retval)) {
		/* FIXME runner failed to stop */
		return 1;
	}
	/* Clean up state */
	if (sem_destroy(&__thread_count)) {
		/* FIXME sem_destroy failed */
		return -1;
	}
	/* TODO(teh): tear-down state */
	free(__thread_pool.state);
	__thread_pool.state = NULL;
	if (__thread_event) {
		__thread_event("thread pool destroyed");
	}
	return 0;
}

/* really simple routines */

LART_PRIVATE int
tpool(int n, ThreadWorker handler)
{
	__thread_event = handler;
	/* is everything chill? */
	if (!__tpool_create(n)) {
		return n;
	}
	/* ...I guess not... */
	if (sem_destroy(&__thread_count)) {
		/* FIXME sem_destory failed */
	}
	return 0;
}

LART_PRIVATE int
tpool_slots(void)
{
	int available;
	if (sem_getvalue(&__thread_count, &available)) {
		/* FIXME sem_getvalue failed */
		return -1;
	}
	return available;
}

LART_PRIVATE int
tpool_queue(ThreadWorker worker, void *work, ThreadMapper mapper)
{
	Block *block = NULL;
	assert(worker && work && mapper);
	/* TODO utilize mapper to spawn multiple blocks? */
	if (!(block = calloc(1, sizeof(Block)))) {
		return 0;
	}
	block->thunk.func = worker;
	block->thunk.args = work;
	if (block_enter(block, TPOOL_DEFAULT)) {
		free(block);
		return 0;
	}
	if (__thread_event) {
		__thread_event(work);
	}
	return 1;
}

LART_PRIVATE int
tpool_flush(int n)
{
	Block *block;
	void *retval;
	static int i = 0;
	/* only "listen" for up to n blocks */
	if (!(i < n)) {
		__tpool_destroy(&__thread_pool);
		return (i = 0);
	}
	block = retval = NULL;
	/* check if a block is ready */
	if (block_exit(block, &retval)) {
		assert(block);
		free(block);
		/* report about this */
		if (__thread_event) {
			__thread_event(retval);
		}
		return ++i;
	}
	return -1;
}
