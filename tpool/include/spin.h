#ifndef __TP_SPIN_H__
#define __TP_SPIN_H__

/* tells spinner what to do (requires three bits) */
enum spin_t {
	SPIN_STATUS = 0,
	SPIN_START  = 1,
	SPIN_STOP   = 2,
	SPIN_GET    = 3,
	SPIN_SET    = 4,
	SPIN_LOCK   = 6,
	SPIN_UNLOCK = 7,
	SPIN_COUNT  = 5
};

typedef enum spin_t TpSpin;

#include <pthread.h>

struct spinner_t {
	pthread_t thread, *tid;
	pthread_spinlock_t mutex;
};

typedef struct spinner_t TpSpinner;

#include "common.h"

/* manage all operations on spinners */

int TP_INTERNAL
tsctl(TpSpinner *, TpSpin, pthread_t **);

#endif /* __TP_SPIN_H__ */
