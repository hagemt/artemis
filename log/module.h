#ifndef __MODULE_H__
#define __MODULE_H__

#include <stddef.h>

struct __module_data_t {
	char *name;
	void *key, *value;
	size_t count;
};

#define MODULE_NAME_MAX 64

struct __module_list_t {
	struct __module_data_t *head;
	struct __module_list_t *tail;
};

#define NODE_EMPTY ((Node) { NULL, NULL })

typedef struct __module_data_t Module;
typedef struct __module_list_t Node;

/* for private API */

LOG_INTERNAL int
log_module_alpha(Module *);

LOG_INTERNAL int
log_module_omega(Module *);

#endif /* __MODULE_H__ */
