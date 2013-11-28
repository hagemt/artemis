#include "log.h"

static LogLevel
__log_level = LOG_LEVEL;

#include <assert.h>

LOG_EXTERNAL LogLevel
log_level_get(const char *tag)
{
	assert(tag);
	return (*tag == '\0') ? LOG_LEVEL_DEFAULT : __log_level;
}

LOG_EXTERNAL void
log_level_set(LogLevel level)
{
	assert(!(level < 0 || LOG_LEVEL_MAX < level));
	__log_level = level;
}
