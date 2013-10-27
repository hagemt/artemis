#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#define RUID 0
#ifndef _ /* FIXME */
#define _(X) (X)
#endif /* gettext */

int
main(void)
{
	uid_t uid = getuid();
	assert(uid);
	fprintf(stdout, "[%s] uid '%d' (%s)\n",
			_("INFO"), uid, _("before setuid"));
	if (setuid(RUID)) {
		fprintf(stderr, "[%s] uid '%d' (%s)\n",
				_("ERROR"), RUID, strerror(errno));
	}
	fprintf(stdout, "[%s] uid '%d' (%s)\n",
			_("INFO"), getuid(), _("after setuid"));
	if (setuid(uid)) {
		fprintf(stderr, "[%s] uid '%d' (%s)\n",
				_("WARNING"), getuid(), strerror(errno));
	}
	return EXIT_SUCCESS;
}
