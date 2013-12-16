#include "mapper.h"

ThreadMapper
mapper_default(void)
{
	return tpool_bijection;
}
