#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include "log.h"

static void __loguid(uid_t, char *);
static uid_t __getuid(int);
static void __setuid(uid_t);

#define REAL 0
#define RUID 0

int
main(void)
{
	uid_t uid = __getuid(REAL);
	__loguid(uid, "before setuid");
	__setuid(RUID);
	__loguid(geteuid(), "after setuid");
	__setuid(uid);
	return EXIT_SUCCESS;
}

/* very simple helpers */

static void
__loguid(uid_t uid, char *comment)
{
	char buffer[32];
	snprintf(buffer, 32, "%lu", (long unsigned) uid);
	LOG3("DEBUG", buffer, comment);
}

static uid_t
__getuid(int real)
{
	return real ? getuid() : geteuid();
}

static void
__setuid(uid_t uid)
{
	if (setuid(uid)) {
		__loguid(uid, strerror(errno));
	}
}

