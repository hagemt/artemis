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
#ifndef __LOG_H__
#define __LOG_H__

#define LOG_EXTERNAL extern
#define LOG_INLINE inline

#ifdef __cplusplus
LOG_EXTERNAL "C" {
#endif

/* Log levels */

#define LOG_LEVEL_F LOG_LEVEL_FATAL
#define LOG_LEVEL_E LOG_LEVEL_ERROR
#define LOG_LEVEL_W LOG_LEVEL_WARNING
#define LOG_LEVEL_I LOG_LEVEL_INFORMATION
#define LOG_LEVEL_V LOG_LEVEL_VERBOSE
#define LOG_LEVEL_D LOG_LEVEL_DEBUG

#ifdef ANDROID
#warning "logcat (Android logging) not yet supported"
#endif

enum log_lvl_t {
	LOG_LEVEL_F    = 0xF0,
	LOG_LEVEL_E    = 0xE0,
	LOG_LEVEL_W    = 0x80,
	LOG_LEVEL_I    = 0x40,
	LOG_LEVEL_V    = 0x20,
	LOG_LEVEL_D    = 0x10,
	LOG_LEVEL_MAX  = 0xFF
};

typedef enum log_lvl_t LogLevel;

LOG_EXTERNAL LogLevel
log_level_get(const char *);

LOG_EXTERNAL void
log_level_set(LogLevel);

/* Log properties */

#define LOG_LEVEL_DEFAULT LOG_LEVEL_W
#define LOG_TAG_DEFAULT "slf4c"
#define LOG_TARGET_DEFAULT stderr

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEFAULT
#endif /* !LOG_LEVEL */

#ifdef TAG
#define LOG_STR(X) #X
#define LOG_WRAP(X) LOG_STR(X)
#define LOG_TAG LOG_WRAP(TAG)
#endif

#ifndef LOG_TAG
#define LOG_TAG LOG_TAG_DEFAULT
#endif /* !LOG_TAG */

#ifndef LOG_TARGET
#define LOG_TARGET LOG_TARGET_DEFAULT
#endif /* !LOG_TARGET */

/* Log aliases */

/* native language support */
#ifdef ENABLE_NLS
#define _(X) gettext(X)
#else /* no-op */
#define _(X) X
#endif

/* Simple logging facilities for teh's standard format */
#define LOG3(X, Y, Z) \
	(void) fprintf(LOG_TARGET, "[%s] %s (%s)\n", _(X), _(Y), _(Z))
#define LOG_PRETTY(TAG, X) LOG3(TAG, X, __PRETTY_FUNCTION__)
#define LOG_TAGGED(X, Y) LOG3(LOG_TAG, X, Y)

/*! \brief Wrapper function (non-public API) for invocation of loggers
 *
 * This wrapper provides a simple log-level facility and tagging system.
 *
 * TODO(teh) document parameters -- level, data, format, args...
 */
LOG_EXTERNAL void
__log_wrapper(LogLevel, const char *, const char *, ...);

#define LOG(LVL, FMT, ...) __log_wrapper(LVL, LOG_TAG, FMT, __VA_ARGS__)
#define LOGF(FMT, ...) LOG(LOG_LEVEL_F, FMT, __VA_ARGS__)
#define LOGE(FMT, ...) LOG(LOG_LEVEL_E, FMT, __VA_ARGS__)
#define LOGW(FMT, ...) LOG(LOG_LEVEL_W, FMT, __VA_ARGS__)
#define LOGI(FMT, ...) LOG(LOG_LEVEL_I, FMT, __VA_ARGS__)
#define LOGV(FMT, ...) LOG(LOG_LEVEL_V, FMT, __VA_ARGS__)
#define LOGD(FMT, ...) LOG(LOG_LEVEL_D, FMT, __VA_ARGS__)

/* Log functions */

typedef void (*LogFunction)();

LOG_EXTERNAL LogFunction
log_function_load(char *);

LOG_EXTERNAL LogFunction
log_function_unload(char *);

/* ... or ... */

#include <stdarg.h>
#include <stdio.h>

typedef void (*log_func_t)(FILE *, const char *, const char *, va_list *);

LOG_EXTERNAL log_func_t
log_func_get(const char *slug);

#ifdef __cplusplus
} /* LOG_EXTERN "C" */
#endif /* C++ */

#endif /* __LOG_H__ */
