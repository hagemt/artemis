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

static void
__noop_log(FILE *file, const char *tag, const char *format, va_list *args)
{
	assert(file && tag && format && args);
}

static void
__simple_log(FILE *file, const char *tag, const char *format, va_list *args)
{
	assert(file && tag && format && args);
	fputc('[', file);
	fputs(tag, file);
	fputc(']', file);
	fputc(' ', file);
	vfprintf(file, format, *args);
}

static log_func_t
__log_func = &__simple_log;

/* PUBLIC API */

LOG_EXTERNAL log_func_t
log_func_get(const char *slug)
{
	assert(slug);
	return (*slug == '\0') ? &__noop_log : __log_func;
}
