#ifndef __ARTEMIS_H__
#define __ARTEMIS_H__

#include "libartemis.h"

struct node_t {
	struct entry_t *head;
	struct node_t *tail;
};

typedef struct node_t Node;

LART_PRIVATE void
enter_state(Entry *);

LART_PRIVATE size_t
count_state(void);

LART_PRIVATE extern void
dump_state(void *);

#endif /* __ARTEMIS_H__ */
