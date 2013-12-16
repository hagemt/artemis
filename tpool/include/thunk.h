#ifndef __TP_THUNK_H__
#define __TP_THUNK_H__

/* actually a (pretty generic) thunk! */

typedef void * (*TpFunction)(void *);
typedef void * TpArguments;

struct thunk_t {
	TpFunction func;
	TpArguments args;
};

typedef struct thunk_t TpThunk;

#endif /* __TP_THUNK_H__ */
