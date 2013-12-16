#ifndef __LART_ENTRY_H__
#define __LART_ENTRY_H__

#include <stddef.h>
#include <time.h>

/* TODO(teh): profile this */
#define LART_ENTRY_DLEN (sizeof(unsigned long) << 6)

enum type_t {
	LART_FTYPE_REG = 0x02,
	LART_FTYPE_DIR = 0x01,
	LART_FTYPE_MAX = 0x00
};

struct entry_t {
	char *path; /* <-- require only this field, TODO doc */
	size_t size; /* FIXME make this off_t, or larger type */
	enum type_t type;
	time_t atime, ctime, mtime;
	/* hold a data sample, possibly its hash and a global checksum */
	unsigned char *data, *hash, *sum;
};

/* Public API */

typedef enum type_t Type;
typedef struct entry_t Entry;
typedef void (*EntryCrawlback)(Entry *);

#include "libartemis/constants.h"

LART_PUBLIC Entry *
entry_new(char *, void *);

LART_PUBLIC void
entry_free(Entry *);

LART_PUBLIC void
entry_crawl(char *, EntryCrawlback);

#endif /* __LART_ENTRY_H__ */
