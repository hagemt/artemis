%{
/* GCC 4.3 warns about input() */
#define YY_NO_INPUT
/*
typedef char *YYSTYPE;
extern YYSTYPE prop_yylval;
*/
#ifndef _
#define _(X) X
#endif /* TODO gettext */

#include <stddef.h>
#include <stdio.h>

static size_t __lines = 0;

static char *
__copy(char *);

#include "y.tab.h"
%}

%option noyywrap

%%

#[^\n]*$ {
	fprintf(stderr, "[%s] %s (%s)\n",
		_("LEX"), yytext, _("COMMENT"));
}

^[^=]*= {
	fprintf(stderr, "[%s] %s (%s)\n",
		_("LEX"), yytext, _("LVALUE"));
	yylval.lhs = __copy(yytext);
	return LVALUE;
}

=[^\n]*\n$ {
	fprintf(stderr, "[%s] %s (%s)\n",
		_("LEX"), yytext, _("RVALUE"));
	yylval.rhs = __copy(yytext);
	return RVALUE;
}

\n { ++__lines; }
[ \t] ;
. ;

%%

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 4096

static char *
__copy(char *src)
{
	char *s;
	size_t n;
	assert(src);
	n = strnlen(src, MAX_LEN) + 1;
	if ((s = calloc(n, sizeof(char)))) {
		strncpy(s, src, n);
		/* s[n] = '\0'; */
	} else {
		fprintf(stderr, "[%s] %s (%s)\n",
			_("LEX"), strerror(errno), __PRETTY_FUNCTION__);
	}
	return s;
}
