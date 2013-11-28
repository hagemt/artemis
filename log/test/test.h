#ifndef __TEST_H__
#define __TEST_H__

#include <assert.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

static size_t
__tests_passed;

static void __attribute__ ((constructor))
__reset(void) { __tests_passed = 0; }

#ifdef __cpluscplus
} /* extern "C" */
#endif /* C++ */

#define TESTS_PASSED ((long unsigned) __tests_passed)
#define ALL_TESTS_PASSED (__tests_passed == TEST_COUNT)

#include "log.h"

/* allow redefinition of the logging alias */
#ifndef LOGT
#define LOGT LOGD
#endif /* !LOGT */

/* internal: run precondition */
#define __TEST_ENSURE(TEST, DESC) \
	LOGT("ensure %s", DESC); TEST; LOGT("ensured %s", DESC);

/* external: assert precondition */
#define TEST_ASSERT(TEST, DESC) \
	__TEST_ENSURE(assert(TEST), DESC) \
	++__tests_passed;

/* external: run with precondition */
#define TEST_FATAL(BEFORE, TEST, DESC) \
	LOGT("survives %s?", DESC); \
	__TEST_ENSURE(assert(BEFORE), "check("DESC")") TEST; \
	LOGT("survived %s", DESC); \
	++__tests_passed;

/* TODO(teh) expand testing suite (break-out or use external tools) */
/* FIXME(teh) how to make test-case IDs hashed and mappable? */

#endif /* __TEST_H__ */
