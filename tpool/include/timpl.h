#ifndef __TIMPL_H__
#define __TIMPL_H__

#include <pthread.h>
#include <semaphore.h>

struct tpool_cond_t {
	pthread_cond_t cond;
	pthread_condattr_t attr;
	/* FIXME necessary? */
	pthread_mutex_t mutex;
};

typedef struct tpool_cond_t TpCondition;

struct tpool_attr_t {
	pthread_attr_t threads;
	pthread_mutexattr_t locks;
	TpCondition conditions;
	sem_t slots, signals;
};

typedef struct tpool_attr_t TpAttributes;

#include "common.h"

/* Runner operates block producer/consumer */

void * TP_INTERNAL
producer_consumer(void *);

#include "uri.h"

/* runners handle block enter/exit */

int TP_INTERNAL
runner_start(pthread_t *, URI uri);

int TP_INTERNAL
runner_stop(pthread_t *, void **);

#endif /* __TIMPL_H__ */
