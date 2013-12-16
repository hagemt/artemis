#include "block.h"
#include "runner.h"
#include "runners.h"

#include <assert.h>

TpStatus TP_INTERNAL
tpool_status(TpURI uri, TpBlock *block)
{
	TpStatus status;
	TpRunner runner;
	assert(uri && block);

	status = STATUS_UNKNOWN;
	runner = tpool_runner(uri);
	assert(runner);

	(void) pthread_mutex_lock(runner->mutex);
	if (slist_find_data(runner->running, &block_equals, block) != SLIST_NULL) {
		status = STATUS_RUNNING;
	}
	(void) pthread_mutex_unlock(runner->mutex);

	return status;
}
