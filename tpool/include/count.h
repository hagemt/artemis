#ifndef __TP_COUNT_H__
#define __TP_COUNT_H__

#include <semaphore.h>

#include "common.h"

extern sem_t TP_INTERNAL
__thread_count;

#endif /* __TP_COUNT_H__ */
