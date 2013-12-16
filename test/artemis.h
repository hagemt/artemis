#ifndef __ARTEMIS_H__
#define __ARTEMIS_H__

#include "libartemis.h"

/* persistent state */

LART_PRIVATE void
state_enter(Entry *);

LART_PRIVATE size_t
state_count(void);

LART_PRIVATE extern void
state_dump(void *);

/* nodes in state */

struct node_t {
	struct entry_t *head;
	struct node_t *tail;
};

typedef struct node_t Node;

#include "tpool.h"

#endif /* __ARTEMIS_H__ */
