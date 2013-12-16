#ifndef __TPOOL_H__
#define __TPOOL_H__

typedef void * (*Function)(void *);

/* thread pool */

LART_PRIVATE int
tpool(int, Function);

LART_PRIVATE int
tpool_slots(void);

LART_PRIVATE int
tpool_flush(int);

#include "tpool/mapper.h"

LART_PRIVATE int
tpool_queue(Function, void *, ThreadMapper);

#endif /* __TPOOL_H__ */
