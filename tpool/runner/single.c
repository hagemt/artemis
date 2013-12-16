#include "runners.h"

/* here we have only one tprunner, it handles every queue */

TpRunner TP_INTERNAL
__thread_runner = {
	QUEUE_NULL, QUEUE_NULL,
	SLIST_NULL, SLIST_NULL,
#ifdef NDEBUG
	PTHREAD_MUTEX_INITIALIZER
#else /* DEBUG */
	PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP
#endif /* use proper mutex type */
};

Trie * TP_INTERNAL
__thread_runners = TRIE_NULL;

#include <assert.h>

int TP_INTERNAL
tpool_runner(TpURI uri, TpRunner *runner)
{
	assert(uri && runner);
	runner = &__thread_runner;
	return 0;
}
