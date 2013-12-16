#include "artemis.h"

#include <assert.h>

static void *
handle_path(void *data) {
	assert(data);
	/* workers just crawl paths, for now */
	entry_crawl((char *) data, &state_enter);
	return data;
}

#include <stdlib.h>
#include <stdio.h>

#define MAX_THREADS 4

static void
print_usage(char *name)
{
	fprintf(stderr, "[NOTE] MAX_THREADS = %d\n", MAX_THREADS);
	fprintf(stderr, "[USAGE] %s filepath...\n", name);
}

#include <stdarg.h>
#include <time.h>

#define BUFFER_SIZE 1024

static void
print_stamp(FILE *target, const char *format, ...)
{
	time_t time_stamp;
	/* format arguments */
	va_list arguments;
	char buffer[BUFFER_SIZE];
	assert(target && format);
	va_start(arguments, format);
	vsnprintf(buffer, BUFFER_SIZE, format, arguments);
	va_end(arguments);
	buffer[BUFFER_SIZE - 1] = '\0';
	/* dump a time-stamped log event */
	(void) time(&time_stamp);
	fprintf(target, "[ARTEMIS] said '%s' at %s", buffer, ctime(&time_stamp));
	fflush(target);
}

#define LOG_FILE "artemis.log"
static FILE *
stdlog = NULL;

static void *
handle_event(void *data)
{
	assert(data);
	print_stamp(stdlog, (const char *) data);
	return data;
}

static void
hunt(FILE *, char **, int);

int
main(int argc, char *argv[])
{
	/* sanity-check arguments */
	if (argc < 2 || argc - 1 > MAX_THREADS) {
		print_usage(argv[0]);
		return EXIT_SUCCESS;
	}

	/* log diagnostics to file */
	if (!(stdlog = fopen(LOG_FILE, "a+"))) {
		return EXIT_FAILURE;
	}

	/* use a thread pool to manage work */
	if (!tpool(MAX_THREADS, &handle_event)) {
		print_stamp(stdlog, "the hunt is on");
		hunt(stdlog, argv + 1, argc - 1);
		print_stamp(stdlog, "we will bury them");
	}

	/* halt logging */
	if (fclose(stdlog)) {
		/* FIXME(teh) fclose failed */
	}

	/* print debug info */
	state_dump(stdout);
	return EXIT_SUCCESS;
}

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define GLYPHS "/-\\|"

static void
hunt(FILE *log, char *argv[], int argc)
{
	int i, n;
	struct stat info;
	if (!log) log = stderr;
	/* queue everything as a work units, separate threads */
	for (i = n = 0; i < argc; ++i) {
		if (lstat(argv[i], &info)) {
			/* FIXME lstat failed, pass on invalid path */
			fprintf(log, "[WARNING] %s (skipped invalid path)\n", argv[i]);
		} else { /* FIXME can read? */
			n += tpool_queue(&handle_path, argv[i], &tpool_bijection);
			fprintf(stderr, "[DEBUG] queued %d jobs(s)\n", n);
		}
	}
	/* block here until n jobs have finished  */
	while (tpool_flush(n)) {
		fprintf(stderr,
				"[%c] crawl found %lu entries so far (currently using %d jobs)\r",
				GLYPHS[++i % (sizeof(GLYPHS) - 1)],
				(unsigned long) state_count(),
				MAX_THREADS - tpool_slots());
		sleep(100000);
	}
}
