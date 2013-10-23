%{
#include <stddef.h>
#include <stdio.h>
%}

%union {
	char *lhs, *rhs;
}

%token <lhs> LVALUE
%token <rhs> RVALUE

%%

proper:
			LVALUE RVALUE {
				fprintf(stderr, "%s: '%s'\n", $1, $2);
			}

%%

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

#ifndef _
#define _(X) X
#endif /* TODO gettext */
#define TEST_FILE "sample.properties"

int
main(void)
{
	FILE *file = fopen(TEST_FILE, "r");
	if (!file) {
		fprintf(stderr, "[%s] %s (%s)\n",
			_("FATAL"), TEST_FILE, strerror(errno));
		return EXIT_FAILURE;
	}
	/* setup done */
	for (yyin = file; !feof(yyin); yyparse());
	/* start cleanup */
	if (fclose(file)) {
		/* FIXME fclose failed */
		fprintf(stderr, "[%s] %s (%s)\n",
			_("WARNING"), TEST_FILE, strerror(errno));
	}
	return EXIT_SUCCESS;
}

void
yyerror(char *s)
{
	assert(s);
	fprintf(stderr, "[%s] %s (%s)\n",
		_("FATAL"), s, __PRETTY_FUNCTION__);
	exit(EXIT_FAILURE);
}
