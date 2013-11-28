/* This file is part of: libslf4c
 *
 * libslf4c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libslf4c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libslf4c. <http://www.gnu.org/licenses/>
 */
#include "log.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif /* BUFFER_SIZE */

LOG_EXTERNAL void
test(void)
{
	(void) fprintf(LOG_TARGET, "[%s] %s (%s)\n",
			LOG_TAG, __FILE__, __PRETTY_FUNCTION__);
}

LOG_EXTERNAL void
libtest0(void)
{
	LOGF("%s", __PRETTY_FUNCTION__);
}

LOG_EXTERNAL void
libtest1(FILE *file, char *slug, char *format, va_list *args)
{
	char buffer[BUFFER_SIZE];
	assert(file && slug && format && args);
	vsnprintf(buffer, BUFFER_SIZE, format, *args);
	fprintf(file, "[%s] %s (%s)\n", LOG_TAG, buffer, slug);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */
