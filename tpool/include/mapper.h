#ifndef __TP_MAPPER_H__
#define __TP_MAPPER_H__

typedef void * (*ThreadMapper)(void *);

#include "common.h"

ThreadMapper TP_EXTERNAL
mapper_default(void);

extern ThreadMapper
tpool_bijection;

#endif /* __TP_MAPPER_H__ */
