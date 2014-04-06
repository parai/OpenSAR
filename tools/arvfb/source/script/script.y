#line 2407 "./doc/bison.texi"

%{
	#include <stdio.h>  /* For printf, etc. */
	#include <math.h>   /* For pow, used in the grammar.  */
	#include "arparser.h"
%}

%define api.value.type union  
%token <double>      yDouble
%token <int>         yInteger
%token <ArpVarType*> yExit
%token <ArpVarType*> yVar
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
%printer { fprintf (yyoutput, "%s", $$->Var.String); } yExit;
%printer { fprintf (yyoutput, "%s", $$->Var.String); } yVar;
%printer { fprintf (yyoutput, "%g", $$); } <double>;

%% /* The grammar follows.  */

input:
  %empty
| input line
;



line:
  '\n'
| exp '\n'   { printf ("%.10g\n", $1); }
| error '\n' { yyerrok;                }
;



exp:
|  yDouble			 { DEBUG_BISON(" D ");$$ = $1;           			}
|  yInteger			 { DEBUG_BISON(" I ");$$ = $1;           			}
| exp '+' exp        { DEBUG_BISON(" + ");$$ = $1 + $3;                 }
| exp '-' exp        { DEBUG_BISON(" - ");$$ = $1 - $3;                 }
| exp '*' exp        { DEBUG_BISON(" * ");$$ = $1 * $3;                 }
| exp '/' exp        { DEBUG_BISON(" / ");$$ = $1 / $3;                 }
| '-' exp  %prec NEG { DEBUG_BISON(" _ ");$$ = -$2;                     }
| exp '^' exp        { DEBUG_BISON(" ^ ");$$ = pow ($1, $3);            }
| '(' exp ')'        { DEBUG_BISON(" () ");$$ = $2;                     }
| yExit              { return 0;                                   		}
;

/* End of grammar.  */
%%

