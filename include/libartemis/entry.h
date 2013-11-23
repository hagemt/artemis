#ifndef __CRAWL_H__
#define __CRAWL_H__

#include <stddef.h>
#include <time.h>

typedef char char_t;

enum type_t {
	REGULAR   = 0x02,
	DIRECTORY = 0x01,
	OTHER     = 0x00
};

typedef enum type_t TYPE;

struct entry_t {
	char_t *path; /* <-- only required field */
	time_t atime, ctime, mtime;
	size_t size; /* FIXME make off_t */
	TYPE type;
	unsigned char *data, *hash, *sum;
};

typedef struct entry_t ENTRY;

/*
extern ENTRY *
malloc_entry(char_t *);
*/

extern void
free_entry(ENTRY *);

typedef void (*callback_t)(ENTRY *);

extern void
visit(char_t *, callback_t);

#define MAX_LEN 4096
#define DAT_LEN (sizeof(unsigned long) << 8)

struct node_t {
	struct entry_t *head;
	struct node_t *tail;
};

typedef struct node_t NODE;

#endif /* __SET_H__ */
