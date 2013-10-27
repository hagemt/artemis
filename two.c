#include <errno.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include "num.h"

static num_t __x = 0;

static void
__handler(int signum)
{
	fputc('\n', stderr);
	if (signum) {
		fprintf(stderr, "[%s] %s (%s)\n",
				_("INFO"), "caught signal", strsignal(signum));
	}
	fprintf(stderr, "x = %llu\n",
			(long long unsigned) __x);
	fprintb(stderr, __x);
	if (signum) {
		signal(signum, NULL);
		raise(signum);
	}
}

int main(void) {
	signal(SIGINT, &__handler);
	for (__x = 1; __x < N; ++__x) {
		fprintb(stdout, __x);
		/*
		if (__x & (__x - 1) == 0) {
			fprintf(stdout, "x = %llu\n",
					(long long unsigned) __x);
		}
		*/
	}
	__handler(0);
	return EXIT_SUCCESS;
}
