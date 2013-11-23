#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include "log.h"

#define LOG_UID(CODE, NOTE) LOG3("INFO", itoa(CODE, NULL, 10), NOTE)
#define LOG_ERRNO(X) LOG3("ERROR", X, strerror(errno))

static uid_t __getuid(int);
static void __setuid(uid_t);

#define REAL 0
#define RUID 0

int
main(void)
{
	uid_t uid = __getuid(REAL);
	LOG_UID(uid, "before setuid");
	__setuid(RUID);
	LOG_UID(geteuid(), "after setuid");
	__setuid(uid);
	return EXIT_SUCCESS;
}

/* very simple helpers */

static uid_t
__getuid(int real)
{
	return real ? getuid() : geteuid();
}

static void
__setuid(uid_t uid)
{
	assert(uid >= 0);
	if (setuid(uid)) {
		LOG_ERRNO(uid);
	}
}

