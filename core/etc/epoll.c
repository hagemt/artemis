#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/epoll.h>
#define MAX_EVENTS 32
#define MAX_LINE 1024

#define _(X) X
#define NAME(X) #X
#define WRAP(X) NAME(X)
#define LOG2(TARGET,BODY) \
	fprintf(TARGET, "[%s:%d] %s (%s)\n", \
			__FILE__, __LINE__, _(BODY), strerror(errno));
#define FIXME(X) LOG2(stderr, X)
#define FD(X,FUNC,...) \
	if ((X = FUNC(__VA_ARGS__)) < 0) { FIXME(WRAP(FUNC)); return X; }
#define TRY(FUNC,...) \
	if (FUNC(__VA_ARGS__)) { FIXME(WRAP(FUNC)); }

struct listener_t {
	int fd, pfd;
	pthread_t tid;
	struct epoll_event event;
	struct epoll_event *events;
};

typedef struct listener_t Listener;

static int
accept_events(FILE *file, Listener *epoll)
{
	assert(file && epoll);
	/* initialize fd */
	FD(epoll->fd, fileno, file);
	/* initialize polled fd */
	FD(epoll->pfd, epoll_create1, EPOLL_CLOEXEC);
	/* initialize epoll proper */
	epoll->event.data.fd = epoll->fd;
	epoll->event.events = EPOLLIN | EPOLLET;
	TRY(epoll_ctl, epoll->pfd, EPOLL_CTL_ADD, epoll->fd, &epoll->event);
	epoll->events = calloc(MAX_EVENTS, sizeof(epoll->event));
	return (epoll->events == NULL);
}

static void *
handle_prompt(void *);

#include <pthread.h>

static int
listen_onfifo(const char *target)
{
	int i, n;
	FILE *file;
	Listener epoll;
	assert(target);
	/* ensure file exists, reads from start, write appends */
	file = fopen(target, "a+");
	if (!file) { return 1; }
	/* create some epoll magic */
	TRY(accept_events, file, &epoll);
	/* send I/O and wait for trigger */
	TRY(pthread_create, &epoll.tid, NULL, &handle_prompt, &epoll);
	while (pthread_tryjoin_np(epoll.tid, NULL)) {
		n = epoll_wait(epoll.pfd, epoll.events, MAX_EVENTS, 1000);
		for (i = 0; i < n; ++i) { /* bang */ fputc('\a', stderr); }
	}
	/* cleanup this awful mess */
	free(epoll.events);
	TRY(fclose, file);
	return 0;
}

#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef EPOLL_FIFO
#define EPOLL_FIFO "fifo"
#endif /* EPOLL_FIFO */

#include <editline/readline.h>

static void *
handle_prompt(void *data)
{
	ssize_t bytes, chars;
	char buffer[MAX_LINE], *line;
	Listener *epoll = (Listener *) data;
	while ((line = readline(EPOLL_FIFO "> "))) {
		chars = snprintf(buffer, MAX_LINE, "[LINE] %s\n", line);
		bytes = write(epoll->fd, buffer, chars);
		free(line);
		assert(bytes == chars);
	}
	return data;
}

int
main(int argc, char *argv[])
{
	struct stat info;
	if (argc > 1) {
		fprintf(stderr, "[USAGE] %s\n", *argv);
		return EXIT_SUCCESS;
	}
	if (lstat(EPOLL_FIFO, &info)) {
		fprintf(stderr, "[FATAL] couldn't stat %s (%s)\n",
				EPOLL_FIFO, strerror(errno));
		return EXIT_FAILURE;
	}
	if (!S_ISFIFO(info.st_mode)) {
		fprintf(stderr, "[FATAL] %s is not a FIFO (%s)\n",
				EPOLL_FIFO, strerror(errno));
		return EXIT_FAILURE;
	}
	TRY(listen_onfifo, EPOLL_FIFO);
	fputc('\n', stderr);
	return EXIT_SUCCESS;
}
