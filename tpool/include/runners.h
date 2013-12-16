#ifndef __TP_RUNNERS_H__
#define __TP_RUNNERS_H__

#include "runner.h"
#include "uri.h"

int TP_INTERNAL
tpool_runner(TpURI, TpRunner *);

/* FIXME these should go away */

extern TpRunner TP_INTERNAL
__thread_runner;

#include <libcalg/trie.h>

extern Trie * TP_INTERNAL
__thread_runners;

#endif /* __TP_RUNNERS_H__ */
