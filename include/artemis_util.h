#ifndef __ARTEMIS_UTIL_H__
#define __ARTEMIS_UTIL_H__

#include "libartemis/record.h"

LART_PRIVATE int
__lookup(Record *, Entry *, Set **);

LART_PRIVATE size_t
__filter_size(size_t);

#endif /* __ARTEMIS_UTIL_H__ */
