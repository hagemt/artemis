#include "mapper.h"

static void *
__placeholder(void *);

ThreadMapper
tpool_bijection = &__placeholder;

#include <assert.h>

static void *
__placeholder(void *arg)
{
	assert(arg);
	return arg;
}
