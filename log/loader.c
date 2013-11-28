#include "log.h"
#include "module.h"

static Node *
create_detached(char *);

static Node *
find_parent(char *);

static void
attach_node(Node *);

static void
remove_child(Node *, char *);

#include <string.h>

/* public API functions */

LOG_EXTERNAL LogFunction
log_function_load(char *name)
{
	Node *node;
	LogFunction fp = LOG_NULL;
	if ((node = find_parent(name))) {
		/* we found an existing (child) node */
		if (log_module_alpha(node->tail->head)) {
			LOGV("alpha failure on parent '%s'", name);
		} else {
			/* fp = (LogFunction) node->tail->head->value; */
			/* memcpy(&fp, node->tail->head->value, sizeof(fp)); */
			memcpy(&fp, node->tail->head->value, sizeof(fp));
		}
	} else if ((node = create_detached(name))) {
		/* we're creating a new (child) node */
		if (log_module_alpha(node->head)) {
			LOGV("alpha failure on create '%s'", name);
		} else {
			/* fp = (LogFunction) node->head->value; */
			/* memcpy(&fp, node->head->value, sizeof(fp)); */
			memcpy(&fp, node->head->value, sizeof(fp));
			attach_node(node);
		}
	}
	return fp;
}

LOG_EXTERNAL LogFunction
log_function_unload(char *name)
{
	Node *node;
	LogFunction fp = LOG_NULL;
	if ((node = find_parent(name))) {
		if (log_module_omega(node->tail->head)) {
			LOGV("omega failure on '%s'", name);
		} else {
			/* fp = (LogFunction) node->tail->head->value; */
			/* memcpy(&fp, node->tail->head->value, sizeof(fp)); */
			memcpy(&fp, node->tail->head->value, sizeof(fp));
			remove_child(node, name);
		}
	}
	return fp;
}

/* log module internals */

static Node
__module_root = NODE_EMPTY;

#include <assert.h>
#include <stdlib.h>

static Node *
find_parent(char *name)
{
	Node *node;
	assert(name);
	/* FIXME(teh) doesn't use module lock/unlock */
	for (node = &__module_root; node->tail; node = node->tail) {
		assert(node->tail->head && node->tail->head->name);
		if (strncmp(name, node->tail->head->name, MODULE_NAME_MAX) == 0) {
			return node;
		}
	}
	return NULL;
}

static Node *
create_detached(char *name)
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
	return node;
}

static void
__lock_modules(void *);

static void
__unlock_modules(void *);

static void
attach_node(Node *node)
{
	assert(node);
	/* Attach it to the module chain */
	__lock_modules(&__module_root);
	node->tail = __module_root.tail;
	__module_root.tail = node;
	__unlock_modules(&__module_root);
}

static void
remove_child(Node *parent, char *name)
{
	Node *node;
	assert(parent && parent->tail && parent->tail->head);
	assert(name && parent->tail->head->name);
	assert(strncmp(name, parent->tail->head->name, MODULE_NAME_MAX) == 0);
	/* Clip this node off */
	__lock_modules(&__module_root);
	node = parent->tail;
	parent->tail = node->tail;
	__unlock_modules(&__module_root);
	node->tail = NULL;
	/* Free the node */
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
