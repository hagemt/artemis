#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline/readline.h>

int
main(void)
{
	char *line;
	while ((line = readline("> "))) {
		if (strlen(line)) {
			fprintf(stdout, "%s\n", line);
		}
		free(line);
	}
	fputc('\n', stdout);
	return EXIT_SUCCESS;
}
