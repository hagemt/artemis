#include "log.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

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

static __log_func_t
__log_func = &__simple_log;

static void
__noop_log(FILE *file, const char *tag, const char *format, va_list *args)
{
	assert(file && tag && format && args);
}

/* PUBLIC API */

LOG_EXTERNAL __log_func_t
log_function(const char *slug)
{
	assert(slug);
	return (*slug == '\0') ? &__noop_log : __log_func;
}
