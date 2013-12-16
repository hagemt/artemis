#include "spin.h"

TP_INLINE static int
__makelock(TpSpinner *, TpSpin);

TP_INLINE static int
__withlock(TpSpinner *, TpSpin);

#include <assert.h>

/* Private API */

int TP_INTERNAL
tsctl(TpSpinner *spinner, TpSpin spin, pthread_t **id)
{
	int result = 0;
	assert(spinner);

	/* the following tpctl options are grouped logically */
	switch (spin) {

	case SPIN_START:
	case SPIN_STOP:
		result = __makelock(spinner, spin);
		break;

	/* require the mutex to exist: */

	case SPIN_GET:
		assert(id);
		(void) __withlock(spinner, SPIN_LOCK);
		*id = spinner->tid;
		(void) __withlock(spinner, SPIN_UNLOCK);
		break;

	case SPIN_SET:
		assert(id);
		(void) __withlock(spinner, SPIN_LOCK);
		spinner->tid = *id;
		(void) __withlock(spinner, SPIN_UNLOCK);
		break;

	/* don't touch the tid: */

	default:
		assert(spin == SPIN_STATUS);
	case SPIN_LOCK:
	case SPIN_UNLOCK:
		result = __withlock(spinner, spin);
	}

	/* FIXME log this info? */
	return result;
}

/* helper functions */

TP_INLINE static int
__withlock(TpSpinner *spinner, TpSpin spin)
{
	register int result;
	assert(spinner);
	switch (spin) {
	/* wrapper for pthread_spin_lock */
	case SPIN_LOCK:
		result = pthread_spin_lock(&spinner->mutex);
		break;
	/* wrapper for pthread_spin_unlock */
	case SPIN_UNLOCK:
		result = pthread_spin_unlock(&spinner->mutex);
		break;
	/* wrapper for quick trylock / unlock if necessary */
	case SPIN_STATUS:
		result = pthread_spin_trylock(&spinner->mutex);
		if (!result) {
			if (pthread_spin_unlock(&spinner->mutex)) {
				/* FIXME serious problem: recovery failed */
			}
		}
	/* TODO find proper error-code */
	default:
		result = -1;
	}
	return result;
}

TP_INLINE static int
__makelock(TpSpinner *spinner, TpSpin spin)
{
	register int result;
	assert(spinner);
	switch (spin) {
	/* wrapper for pthread_spin_init */
	case SPIN_START:
		spinner->tid = &spinner->thread;
		/* TODO consider non-zero */
		result = pthread_spin_init(&spinner->mutex, 0);
		break;
	/* wrapper for pthread_spin_destroy */
	case SPIN_STOP:
		spinner->tid = NULL;
		result = pthread_spin_destroy(&spinner->mutex);
		break;
	/* TODO find proper error-code */
	default:
		result = -1;
	}
	return result;
}
