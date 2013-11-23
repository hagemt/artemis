#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_MAX     (0xFF)
#define LOAD_FAILURE (-1)
#define LOAD_FLAGS   (RTLD_NOW | RTLD_GLOBAL)

struct lib_t {
	char name[LOAD_MAX];
	void *handle;
};

struct dot_t {
	struct lib_t *head;
	struct dot_t *tail;
} __root = { NULL, NULL }, *__next = &__root;

extern int
use(char *name)
{
	struct dot_t *node;
	assert(name);
	node = malloc(sizeof(struct dot_t));
	if (!node) {
		return LOAD_FAILURE;
	}
	/* FIXME helper func */
	node->head = malloc(sizeof(struct lib_t));
	if (!node->head) {
		free(node);
		return LOAD_FAILURE;
	}
	node->tail = NULL;
	/* populate */
	node->head->handle = dlopen(name, LOAD_FLAGS);
	if (node->head->handle) {
		strncpy(node->head->name, name, LOAD_MAX);
		__next->tail = node;
		__next = __next->tail;
		return 0;
	}
	/* FIXME? */
	#ifndef NDEBUG
	dlerror();
	#endif /* DEBUG */
	free(node->head);
	free(node);
	return LOAD_FAILURE;
}

static void
__unload(struct dot_t *prev)
{
	struct dot_t *node;
	assert(prev);
	/* clip node */
	node = prev->tail;
	prev->tail = node->tail;
	/* unload handle */
	if (dlclose(node->head->handle)) {
		/* FIXME dlclose failed */
	}
	free(node->head);
	free(node);
}

extern int
drop(char *name)
{
	struct dot_t *prev, *next;
	assert(name);
	for (prev = &__root; next = prev->tail; prev = next) {
		if (strncmp(next->head->name, name, LOAD_MAX) == 0) {
			__unload(prev);
			return 0;
		}
	}
	return LOAD_FAILURE;
}

#include <stdio.h>

#ifndef _ /* FIXME */
#define _(X) X
#endif /* gettext */

static void
__dump(FILE *fp)
{
	register size_t len;
	struct dot_t *next = &__root;
	assert(fp);
	for (len = 0; (next = next->tail); ++len) {
		fprintf(fp, "[%s] %p (%s)\n",
				_("DEBUG"), next->head->handle, _(next->head->name));
	}
	fprintf(fp, "[%s] %lu (%s)\n",
			_("DEBUG"), (unsigned long) len, _("libs checked"));
}

static void
__check(char *name)
{
	struct dot_t *next = &__root;
	assert(name);
	while ((next = next->tail)) {
		fprintf(stderr, "[%s] checking %s (for %s)\n",
				_("DEBUG"), next->head->name, name);
		if (dlsym(next->head->handle, name)) {
			fprintf(stderr, "[%s] lib %s (has %s)\n",
					_("INFO"), next->head->name, name);
		}
	}
}

/*
#include <readline/readline.h>
#include <readline/history.h>
*/

#include <editline/readline.h>

/*
static char *
__prompt(EditLine *el)
{
	assert(el);
	return "> ";
}

static int
__process(char *line)
{
	assert(line);
	printf("line: %s\n", line);
	if (*line == '\n') {
		return 0;
	}
	__check(line);
	return 1;
}

int
main(int argc, char *argv[])
{
	char *line; int count;
	EditLine *el = el_init(*argv, stdin, stdout, stderr);
	el_set(el, EL_PROMPT, &__prompt);
	el_set(el, EL_EDITOR, "vi");

	use(LIBRARY);

	do { el_line(el, &count); }
	while (count && __process(line));
	el_end(el);

	__dump(stderr);
	drop(LIBRARY);
	__dump(stderr);
	return 0;
}
*/

#ifndef LIBRARY
#define LIBRARY "/usr/lib/libm.so"
#endif /* LIBRARY */

static int
__handle(char *line)
{
	if (!line || *line == '\0' || *line == '\n') {
		return 0;
	}
	__check(line);
	__dump(stderr);
	return 1;
}

int main(void) {
	char *line = NULL;
	use(LIBRARY);
	do { line = readline("> "); }
	while (__handle(line));
	fputc('\n', stderr);
	__dump(stderr);
	drop(LIBRARY);
	__dump(stderr);
	return EXIT_FAILURE;
}
