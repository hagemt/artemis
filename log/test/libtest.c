#include "log.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

LOG_EXTERNAL void
libtest_args_none(void)
{
	(void) fprintf(LOG_TARGET, "[%s] %s (%s)\n",
			LOG_TAG, __FILE__, __PRETTY_FUNCTION__);
}

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif /* BUFFER_SIZE */

LOG_EXTERNAL void
libtest_args_real(FILE *file, char *slug, char *format, va_list *args)
{
	char buffer[BUFFER_SIZE];
	assert(file && slug && format && args);
	vsnprintf(buffer, BUFFER_SIZE, format, *args);
	fprintf(file, "[%s] %s (%s)\n", LOG_TAG, buffer, slug);
}

/* simply invoke all library functions: */

LOG_EXTERNAL void
test(void)
{
	libtest_args_none();
	libtest_args_real(stderr, "slug", "", NULL);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */
