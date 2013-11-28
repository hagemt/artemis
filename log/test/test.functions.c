#include "test.h"

#define SYMBOL_NULL "null"
#define SYMBOL_TEST "test"

#define SYMBOL_CALL SYMBOL_TEST
#define SYMBOL_LOAD SYMBOL_TEST

static void
test_log_function_load_unload(void)
{
	LogFunction f, g;

	/* 10 */
	f = log_function_load(SYMBOL_NULL);
	TEST_ASSERT(!LOG_IS_HANDLE(f),
			"invalid handle results from loading non-existent symbol");

	/* 20 */
	f = log_function_load(SYMBOL_LOAD);
	TEST_ASSERT(LOG_IS_HANDLE(f),
			"valid handle results from loading existent symbol");

	/* 30 */
	g = log_function_load(SYMBOL_LOAD);
	TEST_ASSERT(LOG_IS_HANDLE(g) && f == g,
			"valid and identical handle results from loading same symbol again");

	/* 50 */
	g = log_function_unload(SYMBOL_NULL);
	TEST_ASSERT(!LOG_IS_HANDLE(g),
			"invalid handle results from unloading non-existent symbol");

	/* 60 */
	g = log_function_unload(SYMBOL_LOAD);
	TEST_ASSERT(LOG_IS_HANDLE(g),
			"valid handle results from unloading existent symbol");

	/* 70 */
	TEST_ASSERT(f == g,
			"same handle results from loading and unloading the same symbol");

	/* 80 */
	g = log_function_unload(SYMBOL_TEST);
	TEST_ASSERT(!LOG_IS_HANDLE(f),
			"invalid handle results from unloading same symbol twice");
}

static void
__catch_expected(int signum)
{
	LOGV("%s: %d", "caught expected signal", signum);
}

static void
test_log_function_calls(void)
{
	LogFunction f, g;

	/* 40 */
	f = log_function_load(SYMBOL_CALL);
	TEST_FATAL(LOG_IS_HANDLE(f), LOG_CALL_HANDLE(f, NULL),
			"call to valid handle"); /* FIXME(teh) allow survival? */
	(void) log_function_unload(SYMBOL_CALL);

	/* 100 */
	g = log_function_load(SYMBOL_NULL);
	TEST_FATAL(!LOG_IS_HANDLE(g), LOG_CALL_HANDLE(g, NULL),
			"call to invalid handle"); /* FIXME(teh) allow survival? */
}

#include <signal.h>
#include <stdlib.h>

#define TEST_COUNT 10

int
main(void)
{
	log_level_set(LOG_LEVEL_VERBOSE);
	/* call each test-suite */
	test_log_function_load_unload();

	/* catching segfault required for negative test */
	if (signal(SIGSEGV, &__catch_expected) != SIG_DFL) {
		LOGF("%s", "couldn't set up signal handler ");
		return EXIT_FAILURE;
	}
	test_log_function_calls();

	/* provide a final report */
	LOGI("%lu of %lu tests passed", TESTS_PASSED, TEST_COUNT);
	return ALL_TESTS_PASSED ? EXIT_SUCCESS : EXIT_FAILURE;
}
