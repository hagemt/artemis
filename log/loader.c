#include "log.h"
#include "module.h"

static Node *
create_attached(char *);

static Node *
find_parent(char *);

static void
remove_child(Node *, char *);

/* public API functions */

LOG_EXTERNAL LogFunction
log_function_load(char *name)
{
	Node *node;
	LogFunction fp = LOG_NULL;
	if (!(node = find_parent(name))) {
		node = create_attached(name);
	}
	if (node && !log_module_alpha(node->tail->head)) {
		fp = (LogFunction) node->tail->head->value;
		/* memcpy(&fp, node->tail->head->value, sizeof(fp)); */
	}
	return fp;
}

LOG_EXTERNAL LogFunction
log_function_unload(char *name)
{
	Node *node;
	LogFunction fp = LOG_NULL;
	node = find_parent(name);
	if (node && !log_module_omega(node->tail->head)) {
		fp = (LogFunction) node->tail->head->value;
		/* memcpy(&fp, node->tail->head->value, sizeof(fp)); */
		remove_child(node, name);
	}
	return fp;
}

/* log module internals */

static Node
__modules = NODE_EMPTY;

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static Node *
find_parent(char *name)
{
	Node *node;
	assert(name);
	/* FIXME(teh) doesn't use module lock/unlock */
	for (node = &__modules; node->tail; node = node->tail) {
		assert(node->tail->head && node->tail->head->name);
		if (strncmp(name, node->tail->head->name, MODULE_NAME_MAX) == 0) {
			return node;
		}
	}
	return NULL;
}

static void
__lock_modules(void *);

static void
__unlock_modules(void *);

static Node *
create_attached(char *name)
{
	Node *node;
	assert(name);
	/* Create a new node, empty except the name */
	node = malloc(sizeof(Node));
	if (!node) {
		return NULL;
	}
	node->tail = NULL;
	node->head = malloc(sizeof(Module));
	if (!node->head) {
		free(node);
		return NULL;
	}
	memset(node->head, 0x00, sizeof(Module));
	node->head->count = strnlen(name, MODULE_NAME_MAX);
	node->head->name = malloc(node->head->count * sizeof(char));
	if (!node->head->name) {
		free(node->head);
		free(node);
		return NULL;
	}
	strncpy(node->head->name, name, node->head->count);
	node->head->count = 0;
	/* Attach it to the module chain */
	__lock_modules(&__modules);
	node->tail = __modules.tail;
	__modules.tail = node;
	__unlock_modules(&__modules);
	return &__modules;
}

static void
remove_child(Node *parent, char *name)
{
	Node *node;
	assert(parent && parent->tail && parent->tail->head);
	assert(name && parent->tail->head->name);
	assert(strncmp(name, parent->tail->head->name, MODULE_NAME_MAX) == 0);
	/* Clip this node off */
	__lock_modules(&__modules);
	node = parent->tail;
	parent->tail = node->tail;
	__unlock_modules(&__modules);
	/* Free the node */
	node->tail = NULL;
	free(node->head->name);
	free(node->head);
	free(node);
}

#ifdef MODULE_THREADS

#include <pthread.h>

static pthread_mutex_t
__module_lock = PTHREAD_MUTEX_INITIALIZER;

#endif /* MODULE_THREADS */

static void
__lock_modules(void *state)
{
	assert(state);
#ifdef MODULE_THREADS
	pthread_mutex_lock(&__module_lock);
#endif /* MODULE_THREADS*/
}

static void
__unlock_modules(void *state)
{
	assert(state);
#ifdef MODULE_THREADS
	pthread_mutex_unlock(&__module_lock);
#endif /* MODULE_THREADS */
}
