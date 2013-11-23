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
