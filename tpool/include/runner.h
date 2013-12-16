#ifndef __TP_RUNNER_H__
#define __TP_RUNNER_H__

enum run_status_t {
	STATUS_QUEUED  = 0x0,
	STATUS_RUNNING = 0x1,
	STATUS_FAILED  = 0x2,
	STATUS_UNKNOWN = 0xF
};

typedef enum run_status_t TpStatus;

#include "common.h"
#include "block.h"
#include "uri.h"

TpStatus TP_INTERNAL
tpool_status(TpURI, TpBlock *);

TpBlock * TP_INTERNAL
tpool_queue(TpURI, TpFunction, TpArguments);

#include <libcalg/queue.h>
#include <libcalg/slist.h>

struct runner_t {
	Queue *queued, *failed;
	SListEntry *running, *joined;
	pthread_mutex_t mutex;
};

typedef struct runner_t TpRunner;

#endif /* __TP_RUNNER_H__ */
