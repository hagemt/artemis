#ifndef __ARTEMIS_DATA_H__
#define __ARTEMIS_DATA_H__

#include "libartemis/entry.h"

LART_PRIVATE void
fill_data(Entry *);

LART_PRIVATE void
hash_data(Entry *);

LART_PRIVATE void
free_data(Entry *);

#endif /* __ARTEMIS_DATA_H__ */
