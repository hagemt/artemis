#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

typedef unsigned long iptr_t;

static iptr_t
gcd(iptr_t a, iptr_t b)
{
	if (b == 0)
		return a;
	else
		return gcd(b, a % b);
}

int main(void) {
	void *p1 = malloc(1000000);
	void *p2 = malloc(1000000);
	iptr_t p1r = (iptr_t) p1 & 0xfffffff0;
	iptr_t p2r = (iptr_t) p2 & 0xfffffff0;
	printf("page size = %u\n", getpagesize());
	printf("p1 = %p, p2 = %p\n", p1, p2);
	printf("p1r = %p, p2r = %p\n", (void *) p1r, (void *) p2r);
	printf("gcd = %lu\n", gcd(p1r, p2r));
	return 0;
}
