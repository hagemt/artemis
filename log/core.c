#include "log.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

/* public API functions */

LOG_EXTERNAL void
__log_wrapper(LogLevel lvl, const char *info, const char *fmt, ...)
{
	va_list args;
	const char *slug;
	__log_func_t function;
	assert(info && fmt);
	if (lvl < log_level_get(info)) {
		return;
	}

	/* Determine the slug, based upon level */
	switch (lvl) {
	case LOG_LEVEL_F: slug = "FATAL"; break;
	case LOG_LEVEL_E: slug = "ERROR"; break;
	case LOG_LEVEL_W: slug = "WARN"; break;
	case LOG_LEVEL_I: slug = "INFO"; break;
	case LOG_LEVEL_V: slug = "VERBOSE"; break;
	default: slug = info;
	}

	/* Determine the logging function */
	function = log_function(slug);
	if (function) {
		va_start(args, fmt);
		/* FIXME(teh) make this retargetable */
		(*function)(LOG_TARGET, slug, fmt, &args);
		va_end(args);
	} else {
		/* FIXME(teh) what do? */
		LOG_TAGGED(info, slug);
	}

	/* TODO(teh) determine if this is the right decision... */
#ifndef NDEBUG
	fputc(' ', LOG_TARGET);
	fputc('(', LOG_TARGET);
	fputs(info, LOG_TARGET);
	fputc(')', LOG_TARGET);
#endif /* DEBUG */
	/* Always finish with a newline */
	fputc('\n', LOG_TARGET);
	/* FIXME(teh) \r support? */
}
