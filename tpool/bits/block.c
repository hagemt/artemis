#include "block.h"

#include <assert.h>

int TP_INTERNAL
block_create(TpBlock *block, TpFunction func, TpArguments args)
{
	/* FIXME shouldn't assert func/args? */
	assert(block && func && args);
	return 1;
}

int TP_INTERNAL
block_destory(TpBlock *block)
{
	assert(block);
	return 1;
}

int TP_INTERNAL
block_equals(TpBlock *b1, TpBlock *b2)
{
	TpThunk *t1, *t2;
	pthread_t *pt1, *pt2;
	assert(b1 && b2);
	t1 = &b1->thunk;
	t2 = &b2->thunk;
	if (t1->func != t2->func || t1->args != t2->args) {
		return 0;
	}
	/* sigh... just use (lib) UUID? */
	(void) tsctl(b1->context, SPIN_GET, &pt1);
	(void) tsctl(b2->context, SPIN_GET, &pt2);
	return pthread_equal(*pt1, *pt2);
}

/* called to invoke a block */

int TP_INTERNAL
block_enter(TpBlock *block, TpURI uri)
{
	assert(block && uri);
	return 1;
}

/* called to check for block exit */

int TP_INTERNAL
block_exit(TpBlock *block, void **ptr)
{
	assert(block && ptr);
	return 1;
}
