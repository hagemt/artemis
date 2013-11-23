#include <stddef.h>

typedef char type_t;
typedef type_t (*succ)(type_t);
typedef void (*op)(type_t *, size_t);

static int
__range(type_t *p, size_t i, size_t n, succ inc, type_t a, type_t b, op act)
{
	if (i < n) {
		while (*(p + i) <= b) {
			if (__range(p, i + 1, n, inc, a, b, act)) {
				(*act)(p, n);
			}
			*(p + i) = (*inc)(*(p + i));
		}
		*(p + i) = a;
		return 0;
	}
	return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline type_t
__next(type_t t)
{
	return ++t;
}

static void
__print(type_t *p, size_t n)
{
	register size_t i;
	for (i = 0; i < n; ++i) {
		fputc(p[i], stdout);
	}
	fputc('\n', stdout);
}

#define MIN '0'
#define MAX '1'

inline void
range(type_t *p, size_t n)
{
	memset(p, MIN, n);
	(void) __range(p, 0, n, &__next, MIN, MAX, &__print);
}

#ifndef N
#define N 1
#endif /* N */

static type_t __arr[N];

int main(void) {
	range(__arr, N);
	return EXIT_SUCCESS;
}
