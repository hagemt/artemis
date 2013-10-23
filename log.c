#include "log.h"

static enum log_level_t
__log_level = LOG_LEVEL;

inline enum log_level_t
log_level_get(char *tag)
{
	return tag ? LOG_LEVEL_DEFAULT : __log_level;
}

inline void
log_level_set(enum log_level_t level)
{
	assert(!(LOG_LEVEL_F < level));
	__log_level = level;
}

#include <stdarg.h>
#include <stdio.h>

static void
__standard_log(FILE *dst, const char *tag, const char *fmt, va_list *args)
{
	assert(dst && tag && fmt && args);
	fputc('[', dst);
	fputs(tag, dst);
	fputc(']', dst);
	fputc(' ', dst);
	vfprintf(dst, fmt, *args);
	fputc(' ', dst);
	fputc('(', dst);
	fputs(__PRETTY_FUNCTION__, dst);
	fputc(')', dst);
	fputc('\n', dst);
}

inline void
__log(enum log_level_t lvl, const char *info, const char *fmt, ...)
{
	va_list args;
	const char *slug;
	assert(fmt);
	if (lvl < __log_level) {
		return;
	}
	switch (lvl) {
	case LOG_LEVEL_F: slug = "FATAL"; break;
	case LOG_LEVEL_E: slug = "ERROR"; break;
	default: slug = info;
	}
	va_begin(args, fmt);
	__standard_log(LOG_TARGET, slug, fmt, &args);
	va_end(args);
}
