#define TPOOL_INTERNAL
#include "runner.h"

struct runner_t
__thread_runner = {
	QUEUE_NULL, QUEUE_NULL,
	PTHREAD_MUTEX_INIT,
	SLIST_NULL, SLIST_NULL,
};

/* runners have some actions privately accessible via URI */

static int
runner_empty(const char *);

static int
runner_kill(const char *, Block *);

static void
runner_setup(const char *);

static void
runner_cleanup(const char *);

static void
runner_handle(int);

#include <assert.h>

TPOOL_PRIVATE int
runner_ping(const char *uri, Block *block)
{
	int result;
	struct runner_t *runner = &__thread_runner;
	assert(uri && block);
	if (runner && runner->queued != QUEUE_NULL) {
		pthread_mutex_lock(&runner->mutex);
		result = queue_push_tail(runner->queued, block);
		if (!result) { runner_kill(uri, block); }
		pthread_mutex_unlock(&runner->mutex);
		return result;
	}
	return -1;
}

#include <signal.h>

TPOOL_PRIVATE int
runner_start(pthread_t *id, const char *uri)
{
	sigset_t masked_signals;
	struct runner_t *runner = &__thread_runner;
	assert(id && uri && runner_empty(runner));
	/* TODO set handler &runner_cleanup */
	if (sigfillset(&masked_signals)) {
		/* FIXME sigfillset failed */
		return -1;
	}
	if (sigdelset(&masked_signals, TPOOL_SIGTERM)) {
		return -1;
	}
	runner = runner_setup(uri);
	/* TODO(teh) attributes for runner? */
	if (pthread_create(id, NULL, &producer_consumer, &masked_signals)) {
		runner_cleanup(uri);
		return 1;
	}
	return 0;
}

TPOOL_PRIVATE int
runner_stop(pthread_t *id, void **retval)
{
	struct runner_t *runner = &__thread_runner;
	assert(id && retval && !runner_empty(runner));
	/* send a signal we know isn't masked */
	if (pthread_kill(*id, TPOOL_SIGTERM)) {
		return -1;
	}
	/* still need to join w/ thread */
	if (pthread_join(*id, retval)) {
		return 1;
	}
	runner_handle(0);
	return 0;
}

/* aux functions */

TPOOL_INLINE static struct runner_t *
runner_setup(const char *uri)
{
	struct runner_t *runner = &__thread_runner;
	assert(uri && runner && runner_empty(uri));
	pthread_mutex_lock(&runner->mutex);
	runner->queued = queue_new();
	runner->failed = queue_new();
	runner->running = runner->joined = SLIST_NULL;
	pthread_mutex_unlock(&runner->mutex);
	return runner;
}

TPOOL_INLINE static int
runner_empty(const char *uri)
{
	struct runner_t *runner = &__thread_runner;
	assert(uri && runner);
	return
		(runner->queued == QUEUE_NULL) &&
		(runner->failed == QUEUE_NULL) &&
		(runner->joined == SLIST_NULL) &&
		(runner->running == SLIST_NULL);
}

#include <stdio.h>

TPOOL_INLINE static Block *
__dump_block(Block *block, const char *slug)
{
	assert(block && slug);
	fprintf(stderr, "[BLOCK] %s: %0X (%p %p)\n", slug,
			(unsigned int) block->thread, block->thunk.func, block->thunk.args);
	return block;
}

static int
runner_kill(const char *uri, Block *block)
{
	int failures;
	slist_iterator iterator;
	struct runner_t *runner = &__thread_runner;
	assert(uri && block && !runner_empty(runner));
	/* TODO find and pull block out of running */
	pthread_mutex_lock(&runner->mutex);
	slist_iterate(runner->running, &iterator);
	for (failures = 0; slist_iter_has_next(&iterator);) {
		if (block != slist_iter_next(&iterator)) {
			continue;
		}
		if (pthread_kill(block->thread, TPOOL_SIGTERM)) {
			++failures;
			continue;
		}
		if (pthread_join(block->thread, NULL)) {
			++failures;
			continue;
		}
		/* mark any force-killed job failed */
		slist_iter_remove(&iterator);
		if (queue_push_tail(manager->failed, block)) {
			/* FIXME queue_push_tail failed */
		}
	}
	pthread_mutex_unlock(&runner->mutex);
	return failures;
}

static void
runner_cleanup(const char *uri)
{
	slist_iterator iterator;
	struct runner_t *runner = &__thread_runner;
	assert(uri && !runner_empty(uri));
	pthread_mutex_lock(&runner->mutex);
#ifndef NDEBUG
	while (!queue_is_empty(runner->queued)) {
		__dump_block(queue_pop_head(runner->queued), "queued");
	}
	while (!queue_is_empty(runner->failed)) {
		__dump_block(queue_pop_head(runner->failed), "failed");
	}
	slist_iterate(&runner->joined, &iterator);
	while (slist_iter_has_more(&iterator)) {
		__dump_block(slist_iter_next(iterator), "joined");
	}
	/* handle the running set differently */
	slist_iterate(&runner->running, &iterator);
	while (slist_iter_has_more(runner->running)) {
		runner_kill(uri, __dump_block(slist_iter_next(iterator), "running"));
	}
#endif /* DEBUG */
	queue_free(runner->queued);
	queue_free(runner->failed);
	slist_free(runner->joined);
	slist_free(runner->running);
	/* TODO: didn't we just lose references to blocks? */
	pthread_mutex_unlock(&runner->mutex);
}

TPOOL_INLINE static void
__die_in_a_fire(const char *uri)
{
	sigset_t empty, old;
	struct manager_t *manager = &__thread_pool;
	assert(uri && manager);
	if (sigemptyset(&empty)) {
		/* FIXME sigemptyset failed */
	}
	if (pthread_sigmask(SIG_SETMASK, &empty, &old)) {
		/* FIXME pthread_sigmask failed */
	}
	/* FIXME don't re-raise, right? */
	if (pthread_kill(pthread_self(), SIGUSR2)) {
		/* FIXME pthread_kill failed */
	}
}

/* TODO(teh): register this as runners' signal handler */

static void
runner_handle(int signum)
{
	if (!runner_empty(TPOOL_DEFAULT)) {
		runner_cleanup(TPOOL_DEFAULT);
	}
	/* ... oh noes ... */
	if (signum) {
		fprintf(stderr, "\n[FATAL] caught signal %d (%s)\n",
				signum, strsignal(signum));
		__die_in_a_fire(TPOOL_DEFAULT);
	}
}

/* TODO(teh): implement lookups? */

TPOOL_PRIVATE Trie *
__thread_runners = NULL;
