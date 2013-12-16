#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline/readline.h>

typedef char *String;

int
main(void)
{
	String line;
	/* so long as we have input */
	while ((line = readline("> "))) {
		/* print non-empty lines */
		if (strlen(line)) {
			fprintf(stdout, "%s\n", line);
		}
		free(line);
	}
	/* add an extra break */
	fputc('\n', stdout);
	return EXIT_SUCCESS;
}
