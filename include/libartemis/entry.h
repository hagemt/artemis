#ifndef __LART_ENTRY_H__
#define __LART_ENTRY_H__

#include <stddef.h>
#include <time.h>

#define LART_ENTRY_DLEN (sizeof(unsigned long) << 6)

enum type_t {
	LART_FTYPE_REG = 0x02,
	LART_FTYPE_DIR = 0x01,
	LART_FTYPE_MAX = 0x00
};

struct entry_t {
	char *path; /* <-- only required field, TODO doc */
	size_t size; /* FIXME make this off_t, or larger type */
	enum type_t type;
	time_t atime, ctime, mtime;
	unsigned char *data, *hash, *sum;
};

/* TODO(teh) remove...?
struct node_t {
	struct entry_t *head;
	struct node_t *tail;
};
typedef struct node_t Node;
*/

typedef enum type_t Type;
typedef struct entry_t Entry;
typedef void (*Callback)(Entry *);

/* Public API */

extern Entry *
malloc_entry(char *);

extern void
free_entry(Entry *);

extern void
visit(char *, Callback);

#endif /* __LART_ENTRY_H__ */
