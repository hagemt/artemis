#ifndef __LOG_H__
#define __LOG_H__

#define LOG_EXTERNAL extern
#define LOG_INLINE inline
#define LOG_INTERNAL

#define LOG_NULL ((LogFunction) 0)

/* Log levels */

#define LOG_LEVEL_F LOG_LEVEL_FATAL
#define LOG_LEVEL_E LOG_LEVEL_ERROR
#define LOG_LEVEL_W LOG_LEVEL_WARNING
#define LOG_LEVEL_I LOG_LEVEL_INFORMATION
#define LOG_LEVEL_V LOG_LEVEL_VERBOSE
#define LOG_LEVEL_D LOG_LEVEL_DEBUG

/* TODO(teh) logcat support */
#ifdef ANDROID
#warning "logcat (Android logging) not yet supported"
#define LOG_CAT
#endif

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

/* native language support */
#ifdef ENABLE_NLS
#define _(X) gettext(X)
#else /* no-op */
#define _(X) X
#endif

#ifdef __cplusplus
LOG_EXTERNAL "C" {
#endif

/* Mutable log-levels */

enum log_lvl_t {
	LOG_LEVEL_FATAL       = 0xF0,
	LOG_LEVEL_ERROR       = 0xE0,
	LOG_LEVEL_WARNING     = 0x80,
	LOG_LEVEL_INFORMATION = 0x40,
	LOG_LEVEL_VERBOSE     = 0x20,
	LOG_LEVEL_DEBUG       = 0x10,
	LOG_LEVEL_MAX         = 0xFF
};

typedef enum log_lvl_t LogLevel;

LOG_EXTERNAL LogLevel
log_level_get(const char *);

LOG_EXTERNAL void
log_level_set(LogLevel);

/*! \brief Wrapper function (non-public API) for invocation of loggers
 *
 * This wrapper provides a simple log-level facility and tagging system.
 *
 * TODO(teh) document parameters -- level, data, format, args...
 */
LOG_EXTERNAL void
__log_wrapper(LogLevel, const char *, const char *, ...);

/* Log functions */

typedef void (*LogFunction)();

LOG_EXTERNAL LogFunction
log_function_load(char *);

LOG_EXTERNAL LogFunction
log_function_unload(char *);

/* ... or ... */

#include <stdarg.h>
#include <stdio.h>

typedef void (*__log_func_t)(FILE *, const char *, const char *, va_list *);

LOG_EXTERNAL __log_func_t
log_function(const char *slug);

#ifdef __cplusplus
} /* LOG_EXTERN "C" */
#endif /* C++ */

/* Log aliases */

/* Simple logging facilities for simple (standard-teh) format */
#define LOG3(X, Y, Z) \
	(void) fprintf(LOG_TARGET, "[%s] %s (%s)\n", _(X), _(Y), _(Z))
#define LOG_PRETTY(X, Y) LOG3(X, Y, __PRETTY_FUNCTION__)
#define LOG_TAGGED(Y, Z) LOG3(LOG_TAG, Y, Z)

/* Simple logging facilities for level-aware (standard) format */
#define LOG(LVL, FMT, ...) __log_wrapper(LVL, LOG_TAG, FMT, __VA_ARGS__)
#define LOGF(FMT, ...) LOG(LOG_LEVEL_F, FMT, __VA_ARGS__)
#define LOGE(FMT, ...) LOG(LOG_LEVEL_E, FMT, __VA_ARGS__)
#define LOGW(FMT, ...) LOG(LOG_LEVEL_W, FMT, __VA_ARGS__)
#define LOGI(FMT, ...) LOG(LOG_LEVEL_I, FMT, __VA_ARGS__)
#define LOGV(FMT, ...) LOG(LOG_LEVEL_V, FMT, __VA_ARGS__)
#define LOGD(FMT, ...) LOG(LOG_LEVEL_D, FMT, __VA_ARGS__)

#define LOG_IS_HANDLE(X) (X != LOG_NULL)

#define LOG_CALL_HANDLE(X, ...) (*X)(__VA_ARGS__)

#endif /* __LOG_H__ */
