#include "log.h"

static void
__noop(void)
{
	(void) 0;
}

#include <stdio.h>

#define HD_LINE_STRIDE 16
#define HD_TEST_STRING "this is a test of the emergency response system"

void
hd(FILE *file, const char *label, void *data, size_t bytes)
{
	size_t goffset, loffset; char c;
	fprintf(file, "%s\n", label);
	for (goffset = 0; goffset < bytes; goffset += HD_LINE_STRIDE) {
		fprintf(file, "%08X  ", (unsigned int) goffset);
		for (loffset = 0; loffset < HD_LINE_STRIDE; ++loffset) {
			if (goffset + loffset < bytes) {
				c = * (char *) (data + goffset + loffset);
				fprintf(file, "%X%X", (c & 0xF0) >> 4, (c & 0x0F) >> 0);
				fputc(' ', file);
			} else {
				fprintf(file, "   ");
			}
			if (loffset % 8 == 7) fputc(' ', file);
		}
		fputc('|', file);
		for (loffset = 0; loffset < HD_LINE_STRIDE; ++loffset) {
			if (goffset + loffset < bytes) {
				c = * (char *) (data + goffset + loffset);
				fputc((c < ' ' || c > '~') ? '.' : c, file);
			}
		}
		fputc('|', file);
		fputc('\n', file);
	}
}

#include <string.h>

int
main(void)
{
	void *data_ptr;
	LogFunction func_ptr;
	log_level_set(LOG_LEVEL_DEBUG);
	LOGI("\n%lu (%s)\n%lu (%s)\n2",
			sizeof(func_ptr), "size of function (void (*)(void)) pointer",
			sizeof(data_ptr), "size of data (void *) pointer");
	memset(&data_ptr, 0x00, sizeof(void *));
	func_ptr = &__noop;
	memcpy(&data_ptr, &func_ptr, 8);
	hd(stderr, "data_ptr", &data_ptr, sizeof(data_ptr));
	hd(stderr, "func_ptr", &func_ptr, sizeof(func_ptr));
	hd(stderr, "char_ptr", HD_TEST_STRING, sizeof(HD_TEST_STRING));
	return 0;
}
