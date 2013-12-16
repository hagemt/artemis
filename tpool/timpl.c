#include "block.h"
#define TPOOL_INTERNAL
#include "timpl.h"

#include <assert.h>
#include <signal.h>
#include <unistd.h>

/* wrappper for pthread_sigmask */
LART_INLINE static int
__trysignal(struct tattr_t *);

/* wrapper for pthread_create */
LART_INLINE static int
__trycreate(struct tattr_t *, Block *);

/* wrapper for pthread_join */
LART_INLINE static int
__tryjoin(struct future_t *);

static void *
__consume(void *data)
{
	struct tattr_t *attr;
	assert(data);
	if (__trysignal((attr = (struct tattr_t *) data))) {
		return NULL;
	}
	return data;
}

static void *
__produce(void *data)
{
	struct tattr_t *attr;
	assert(attr);
	if (__trysignal((attr = (struct tattr_t *) data))) {
		return NULL;
	}

	/* FIXME change to signal caught? how? */
	while (true) {
		pthread_mutex_lock(&__thread_runner.mutex);
		if ((context.block = queue_pop_head(&__thread_runner.queued))) {
			/* if (__trymake_worker()) */
			block->thread = __trymake_worker(block->thunk.func, block->thunk.args);

			result = pthread_equals(primal, block->thread)
				? queue_push_tail(&__thread_runner.queued, block)
				: slist_prepend(&__thread_runner.running, block);
			assert(result);
		}
		pthread_mutex_unlock(&__thread_runner.mutex);
		usleep(TIME_WAIT(TIME_SLICE));
	}

	return data;
}

/* FIXME(teh): I may be using this wrong */

static const struct timespec
__ONE_MILLISECOND = { 0, TIME_WAIT(1000L) };

TPOOL_PRIVATE void *
producer_consumer(void *data)
{
	int prc, crc;
	void *prp, *crp;
	pthread_t ptid, ctid;
	/* attributes for workers and producer/consumer */
	struct tattr_t *wattr;
	pthread_attr_t pcattr;

	/* launch producer and consumer threads */
	assert(data);
	wattr = (struct tattr_t *) data;
	pthread_attr_init(&pcattr);
	if (pthread_attr_setdetachstate(&pcattr, PTHREAD_CREATE_JOINABLE)) {
		/* FIXME pthread_attr_setdetachstate failed */
	}
	prc = pthread_create(&ptid, &pcattr, &__produce, wattr);
	crc = pthread_create(&ctid, &pcattr, &__consume, wattr);
	assert(prc == 0 && crc == 0);

	/* invariant: each of them will end in due course */
	for (prc = crc = !0; prc && crc; usleep(TIME_WAIT(TIME_SLICE))) {
		/* attempt to achieve a zero return-code (joined) */
		if (prc) prc = pthread_tryjoin_np(ptid, &prp);
		if (crc) crc = pthread_tryjoin_np(ctid, &crp);
#ifndef NDEBUG
		if (!prc) fprintf(stderr, "[INFO] producer %p joined (%p)\n", &ptid, prp);
		if (!crc) fprintf(stderr, "[INFO] consumer %p joined (%p)\n", &ctid, crp);
#endif /* DEBUG */
	}

	pthread_attr_destroy(&pcattr);
	return data;
}

/* aux functions */

TPOOL_INLINE static int
__trysignal(struct tattr_t *attr) {
	sigset_t sigmask, osigmask;
	if (sigfillset(sigmask)) {
		return -1;
	}
	if (sigdelset(sigmask, TPOOL_SIGTERM)) {
		return -1;
	}
	/* TODO(teh) actually initialize signals */
	if (pthread_sigmask(SIG_SETMASK, sigmask, &osigmask)) {
		return 1;
	}
	return 0;
}

LART_INLINE static int
__trycreate(struct tattr_t *attr, Block *block)
{
	pthread_t *thread;
	struct thunk_t *thunk;
	assert(attr && context && context->block);
	/* wait at most one millisecond for a slot */
	if (sem_timedwait(&attr->slots, &__ONE_MILLISECOND)) {
		return 0;
	}
	thread = &context->block->
	thunk = &context->block->thunk;
	/* with the slot ours, create a worker */
	if (pthread_create(&block->thread, attr->,
				block->thunk.worker, block->thunk.work)) {
		/* FIXME pthread_create failed */
		return 1;
	}
	return 0;
}

LART_INLINE static int
__tryjoin(struct future_t *future)
{
	/* this is pretty much a wrapper, I guess */
	assert(id && !pthread_equals(*id, pthread_self()));
	/* TODO return value? */
	if (pthread_timedjoin_np(*id, &block->thunk.args, &__ONE_MILLISECOND)) {
		return 1;
	}
	if (sem_post(&__thread_count)) {
		return -1;
	}
	return 0;
}

