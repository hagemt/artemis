#ifndef __LOG_H__
#define __LOG_H__

/* Log levels */

#define LOG_LEVEL_F FATAL
#define LOG_LEVEL_E ERROR
#define LOG_LEVEL_W WARNING
#define LOG_LEVEL_I INFORMATION
#define LOG_LEVEL_V VERBOSE
#define LOG_LEVEL_D DEBUG

enum log_level_t {
	LOG_LEVEL_F    = 0xFF,
	LOG_LEVEL_E    = 0xEE,
	LOG_LEVEL_W    = 0x80,
	LOG_LEVEL_I    = 0x40,
	LOG_LEVEL_V    = 0x20,
	LOG_LEVEL_D    = 0x10,
};

extern enum log_level_t
log_level_get(char *);

extern void
log_level_set(enum log_level_t);

/* Log properties */

#define LOG_LEVEL_DEFAULT LOG_LEVEL_W
#define LOG_TAG_DEFAULT __PRETTY_FUNCTION__
#define LOG_TARGET_DEFAULT stderr

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEFAULT
#endif /* !LOG_LEVEL */

#ifndef LOG_TAG
#define LOG_TAG LOG_TAG_DEFAULT
#endif /* !LOG_TAG */

#ifndef LOG_TARGET
#define LOG_TARGET LOG_TARGET_DEFAULT
#endif /* !LOG_TARGET */

/* Log aliases */

#define LOG(X) fprintf(LOG_TARGET, "[%s] %s (%s)\n", LOG_TAG, X, __PRETTY_FUNCTION__);

#endif /* __LOG_H__ */
