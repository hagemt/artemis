#ifndef __LART_WRAP_H__
#define __LART_WRAP_H__

#include <libcalg/hash-table.h>

#include "libartemis/constants.h"

LART_PRIVATE unsigned long
wrap_hash_entry_data(HashTableKey);

LART_PRIVATE unsigned long
wrap_hash_entry_path(HashTableKey);

LART_PRIVATE int
wrap_equals_entry_data(HashTableKey, HashTableKey);

LART_PRIVATE int
wrap_equals_entry_path(HashTableKey, HashTableKey);

#endif /* __LART_WRAP_H__ */
