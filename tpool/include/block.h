#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "spin.h"
#include "thunk.h"
#include "uri.h"

/* however, these we do; blocks (opaque) encapsulate computation */

struct block_t {
	TpSpinner context;
	TpThunk thunk;
};

typedef struct block_t TpBlock;

#include "common.h"

/* blocks require some setup, but can probably just be free'd (no) */

int TP_INTERNAL
block_create(TpBlock *, TpFunction, TpArguments);

int TP_INTERNAL
block_destroy(TpBlock *);

int TP_INTERNAL
block_equals(TpBlock *, TpBlock *);

/* called to invoke a block */

int TP_INTERNAL
block_enter(TpBlock *, TpURI);

/* called to check for block exit */

int TP_INTERNAL
block_exit(TpBlock *, void **);

/* TODO:
 * block_yield
 * block_resume
 */

#endif /* __BLOCK_H__ */
