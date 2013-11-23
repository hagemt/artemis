{%
#define YYSTYPE double
#include <math.h>
%}

%token NUM
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

%%

input:
	| input line
;

line: '\n'
	| exp '\n' { printf("\t%.10g\n", $1); }
;

expr:
	NUM { $$ = $1; }
|
	expr '+' expr { $$ = $1 + $3; }
|
	expr '-' expr { $$ = $1 - $3; }
|
	expr '*' expr { $$ = $1 * $3; }
|
	expr '/' expr { $$ = $1 / $3; }
|
	'-' exp %prec NEG { $$ = -$2; }
|
	exp '^' exp { $$ = pow($1, $3); }
|
	'(' exp ')' { $$ = $2; }
;

%%

#include <ctype.h>
#include <stdio.h>

int
yylex(void)
{
	int c;
	/* skip white space  */
	do { c = getchar(); }
	while (c == ' ' || c == '\t');
	/* process numbers */
	if (c == '.' || isdigit(c)) {
		ungetc(c, stdin);
		scanf("%lf", &yylval);
		return NUM;
	}
	/* return end-of-file  */
	if (c == EOF) return 0;
	/* return single chars */
	return c;
}

void
yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}

int
main(void)
{
	yyparse();
}
