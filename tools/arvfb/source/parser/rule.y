/* require bison 3.0.2 */

%{
  #include <stdio.h>  /* For printf, etc. */
  #include <math.h>   /* For pow, used in the grammar.  */
  #include "arparser.h"
%}


%define api.value.type union /* Generate YYSTYPE from these types:  */
%token <double>  NUM         /* Simple double precision number.  */
%token <ArpVarType*> VAR FNCT/* Symbol table pointer: variable and function.  */
%type  <double>  exp


%precedence '='
%left '-' '+'
%left '*' '/'
%precedence NEG /* negation--unary minus */
%right '^'      /* exponentiation */

/* Generate the parser description file.  */
%verbose
/* Enable run-time traces (yydebug).  */
%define parse.trace

/* Formatting semantic values.  */
%printer { fprintf (yyoutput, "%s", $$->Name); } VAR;
%printer { fprintf (yyoutput, "%s()", $$->Name); } FNCT;
%printer { fprintf (yyoutput, "%g", $$); } <double>;

%% /* The grammar follows.  */

input:
  %empty
| input line
;



line:
  '\n'
| exp '\n'
| error '\n' { yyerrok;                }
;



exp:
  VAR                
	{ 
		if(ARP_E_NOT_OK==ArpProcess($1))
		{	
			yyerror(ArpErrorMsg());
		}              
	}
;

/* End of grammar.  */
%%