#include "artemis.h"

static void *
handle_path(void *data) {
	assert(data);
	/* workers just crawl paths, for now */
	visit((char *) data, &__handle_entry);
	return data;
}

#define MAX_THREADS 4
/* TODO(teh) use sem_t to enforce */

static void
print_usage(char *name) {
	fprintf(stderr, "[USAGE] %s path ... (max: %d)\n", name, MAX_THREADS);
}

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define GLYPHS "/-\\|"

int
main(int argc, char *argv[])
{
	int i, n, t;
	FILE *stdlog;
	struct stat info;

	sem_t threads_available;
	pthread_attr_t thread_attr;
	pthread_t ids[MAX_THREADS];

	/* sanity-check arguments */
	if (argc < 2 || argc > MAX_THREADS) {
		print_usage(argv[0]);
		return EXIT_SUCCESS;
	}
	if (!(stdlog = fopen("artemis.log", "a+"))) {
		return EXIT_FAILURE;
	}

	pthread_attr_init(&thread_attr);
	sem_init(&threads_available, 0, MAX_THREADS);
	memset(ids, 0x00, sizeof(pthread_t) * MAX_THREADS);
	/* kick off supplied arguments as work units, separate threads */
	for (i = n = t = 0; i < argc; ++i) {
		if (lstat(argv[i], &info)) {
			/* FIXME lstat failed, pass on invalid path */
			continue;
		}
		if (!pthread_create(&ids[i], &thread_attr, &__handle_path, argv[i])) {
			fprintf(stdlog, "[INFO] started processing '%s'\n", argv[i]);
			if (sem_wait(&threads_available)) { /* FIXME sem_wait failed */ }
		}
	}
	sem_getvalue(&threads_available, &t);
	fprintf(stderr, "[DEBUG] created %d threads (%d of %d available)\n",
			n, t, MAX_THREADS);
	/* report, waiting on threads to finish TODO(teh) ...tryjoin... */
	do {
		for (i = 0; i < n; ++i) {
			if (ids[i] && !pthread_tryjoin_np(ids[i], argv)) {
				fprintf(stdlog, "[INFO] finished processing '%s'\n", *argv);
				if (sem_post(&threads_available)) { /* FIXME sem_post failed */ }
				ids[i] = 0;
			}
		}
		fprintf(stderr, "[%c] Crawling ... (processed %lu entries)\r",
				GLYPHS[++t % sizeof(GLYPHS)], (long unsigned) count_state());
		sem_getvalue(&threads_available, &i);
	} while (i < MAX_THREADS);

	pthread_attr_destroy(&thread_attr);
	sem_destroy(&thread_locks);
	/* finally, print a summary */
	dump_records(stdout);
	if (fclose(stdlog)) {
		/* FIXME(teh) fclose failed */
	}
	return EXIT_SUCCESS;
}
