#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline/readline.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <unistd.h>

struct event_t {
	int inotify_code;
	const char *description;
};

typedef struct event_t Event;

static Event event_types[] = {
	{ IN_ACCESS,        "read of the file"                                  },
	{ IN_MODIFY,        "last modification"                                 },
	{ IN_ATTRIB,        "attributes of file change"                         },
	{ IN_OPEN,          "open of file"                                      },
	{ IN_CLOSE_WRITE,   "sent when a file opened for writing is closed"     },
	{ IN_CLOSE_NOWRITE, "sent when a file opened not for writing is closed" },
	{ IN_MOVED_FROM,    "when the file is moved or renamed"                 },
	{ IN_MOVED_TO,      "when the file is moved or renamed"                 },
	{ IN_DELETE,        "a file/directory deleted"                          },
	{ IN_CREATE,        "a file in a watched directory is created"          },
	{ IN_DELETE_SELF,   "file monitored is deleted"                         },
	{ 0,                NULL                                                }
};

struct data_t {
	pthread_t tid;
};

static void *
handle(void *data) {
	char *line;
	struct data_t *stuff;
	assert(data);
	stuff = (struct data_t *) data;
	while ((line = readline("> "))) {
		free(line);
	}
	return data;
}

int
main(void)
{
	Event *e;
	int fd, wd;
	struct data_t data;
	for (e = event_types; e->description; ++e) {
		fprintf(stderr, "[INFO] code %d: %s\n", e->inotify_code, e->description);
	}
	if ((fd = inotify_init()) < 0) {
		/* FIXME init failed */
	}
	if ((wd = inotify_add_watch(fd, ".", IN_CREATE)) < 0) {
		/* FIXME add watch failed */
	}
	if (pthread_create(&data.tid, NULL, &handle, &data)) {
		/* FIXME pthread create failed */
	}
	while (pthread_tryjoin_np(data.tid, NULL)) {
		/* TODO stuff */
	}
	if (inotify_rm_watch(fd, wd)) {
		/* FIXME remove watch failed */
	}
	if (close(fd)) {
		/* FIXME close failed */
	}
	return EXIT_SUCCESS;
}
