#ifndef __TP_FUTURE_H__
#define __TP_FUTURE_H__

#include "block.h"

struct future_t {
	TpBlock block;
	void *result;
};

typedef struct future_t TpFuture;

#endif /* __TP_FUTURE_H__ */
