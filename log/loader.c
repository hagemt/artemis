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
#include "log.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64

struct __module_data_t {
	char *name;
	void *key, *value;
	size_t count;
};

static struct __module_list_t {
	struct __module_data_t *head;
	struct __module_list_t *tail;
} __modules = { NULL, NULL };

typedef struct __module_data_t Module;
typedef struct __module_list_t Node;

static int __load(Module *);
static int __free(Module *);

inline static Node *
__find_parent(char *name)
{
	Node *node;
	assert(name);
	for (node = &__modules; node->tail; node = node->tail) {
		if (strncmp(node->tail->head->name, name, MAX_NAME) == 0) {
			return node;
		}
	}
	return NULL;
}

static Node *
__find_parent_or_create_child(char *name)
{
	Node *node = __find_parent(name);
	if (node) {
		return node;
	}
	/* Create a new node, empty except the name */
	node = malloc(sizeof(Node));
	if (!node) {
		return node;
	}
	node->tail = NULL;
	node->head = malloc(sizeof(Module));
	if (!node->head) {
		free(node);
		node = NULL;
		return node;
	}
	memset(node->head, 0x00, sizeof(Module));
	node->head->count = strnlen(name, MAX_NAME);
	node->head->name = malloc(node->head->count * sizeof(char));
	if (!node->head->name) {
		free(node->head);
		free(node);
		node = NULL;
		return node;
	}
	strncpy(node->head->name, name, node->head->count);
	node->head->count = 0;
	/* Attach it */
	node->tail = __modules.tail;
	__modules.tail = node;
	return &__modules;
}

LOG_EXTERNAL LogFunction
log_function_load(char *name)
{
	Node *node;
	LogFunction fp = NULL;
	node = __find_parent_or_create_child(name);
	if (node && __load(node->tail->head) == 0) {
		fp = (LogFunction) node->tail->head->value;
		/* memcpy(&fp, node->tail->head->value, sizeof(fp)); */
	}
	return fp;
}

static void
__remove_child(Node *parent, char *name)
{
	Node *node;
	assert(name && parent && parent->tail && parent->tail->head->name);
	assert(strncmp(parent->tail->head->name, name, MAX_NAME) == 0);
	/* Clip this node */
	node = parent->tail;
	parent->tail = node->tail;
	/* FIXME(teh) EXTRA: */
	node->tail = NULL;
	free(node->head->name);
	free(node->head);
	free(node);
}

LOG_EXTERNAL LogFunction
log_function_unload(char *name)
{
	Node *node;
	LogFunction fp = NULL;
	node = __find_parent(name);
	if (node && __free(node->tail->head) == 0) {
		fp = (LogFunction) node->tail->head->value;
		/* memcpy(&fp, node->tail->head->value, sizeof(fp)); */
		__remove_child(node, name);
	}
	return fp;
}

#ifdef IS_NOT_POSIX /* FIXME(teh) works? */

#include <windows.h>

static int
__load(Module *module)
{
	assert(module);
	if (!module->key) {
		assert(module->count == 0);
		module->key = LoadLibrary(module->name);
		if (!module->key) return 1;
	}
	module->value = GetProcAddress(module->key, module->name);
	if (!module->value) return 1;
	++module->count;
	return 0;
}

static int
__free(Module *module)
{
	assert(module);
	if (--module->count == 0) {
		return FreeLibrary(name);
	}
	return 0;
}

#else /* POSIX */

#include <dlfcn.h>

static int
__load(Module *module)
{
	assert(module);
	if (!module->key) {
		assert(module->count == 0);
		module->key = dlopen(module->name, RTLD_LAZY | RTLD_LOCAL);
		/* FIXME(teh) ugh... too late... dlerror() */
		if (!module->key) {
			fprintf(stderr, "dlerror: %s\n", dlerror());
			return 1;
		}
	}
	/* Load the symbol */
	module->value = dlsym(module->key, module->name);
	if (!module->value) {
		fprintf(stderr, "dlerror: %s\n", dlerror());
		return 1;
	}
	++module->count;
	return 0;
}

static int
__free(Module *module)
{
	assert(module);
	if (--module->count == 0) {
		return dlclose(module->key);
	}
	return 0;
}

#endif /* POSIX */

static void
__expected(int signum)
{
	fprintf(stderr, "Caught expected signal %d\n", signum);
}

#include <signal.h>

int main(void) {
	LogFunction f, g;
	f = log_function_load("test");
	assert(f);
	(*f)();
	g = log_function_unload("test");
	assert(g);
	assert(f == g);
	signal(SIGSEGV, &__expected);
	(*g)();
	return EXIT_SUCCESS;
}
