/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */

%{
	#include <stdio.h>  /* For printf, etc. */
	#include <math.h>   /* For pow, used in the grammar.  */
	#include "arscript.h"
	
#define ARSO_FREE_IF_IS_STRING(ss)					\
	if(YVAR_STRING == (ss).type)					\
	{												\
		arso_strfree((ss).u.string);				\
	}
%}

/* %define api.value.type   union */
%union{
	yvar_t var;
	char*  str;
};
%token <var>   tk_double
%token <var>   tk_integer
%token <var>   tk_exit
%token <var>   tk_eof
%token <var>   tk_eol
%token <var>   tk_char
%token <var>   tk_string
%token <var>   tk_id
%type  <var>   expr


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
/*
%printer { fprintf (yyoutput, "%s", $$); } <char*>;
%printer { fprintf (yyoutput, "%d", $$); } <int>;
%printer { fprintf (yyoutput, "%g", $$); } <double>;
*/
%printer { switch($$.type)
			{
				case YVAR_STRING:
					fprintf (yyoutput, "%s", $$.u.string); 
					break;
				case YVAR_INTEGER:
					fprintf (yyoutput, "%d", $$.u.integer); 
					break;
				case YVAR_DOUBLE:
					fprintf (yyoutput, "%g", $$.u.dvar); 
					break;
				case YVAR_FUNCTION:
					fprintf (yyoutput, "%p", $$.u.function); 
					break;
				default:
					assert(0);
					break;
			}
		} <var>;

%% /* The grammar follows.  */

block:
  %empty
| block stmt
| '{' block '}'
;



stmt:
	';' | '\n'
| expr  ';'  |
  expr  '\n'  { arsc_print(&$1);  ARSO_FREE_IF_IS_STRING($1); }
| '{' stmt '}'
| error      { yyerrok;                }
;



expr:
   tk_id             { 	arsc_read(&$$,&$1);		}
|  tk_string         {  arsc_copy(&$$,&$1);		}	
|  tk_double		 { 	arsc_copy(&$$,&$1);     }
|  tk_integer		 { 	arsc_copy(&$$,&$1);     }
| '(' expr ')'       {  arsc_copy(&$$,&$2);     }
|  tk_id '=' expr	 {  yobj_t* obj = arso_get($1.u.string);
					    if(NULL == obj)
					    {	// New it, 
					    	obj = arso_new($1.u.string,&$3);
					    }
					    else
					    {
					    	arso_write(obj,&$3);  
					    }
					    arso_strfree($1.u.string);
					    ARSO_FREE_IF_IS_STRING($3);
					    arso_read(obj,&$$);
					 }
| tk_id '(' expr ')' {	arsc_eval(&$$,&$1,&$3);
						arso_strfree($1.u.string);
						ARSO_FREE_IF_IS_STRING($3);
					 }
| expr '+' expr      {  arsc_add(&$$,&$1,&$3); 
						ARSO_FREE_IF_IS_STRING($1);
						ARSO_FREE_IF_IS_STRING($3);
					 }
| expr '-' expr      {  arsc_sub(&$$,&$1,&$3);
						ARSO_FREE_IF_IS_STRING($1);
						ARSO_FREE_IF_IS_STRING($3);
					 }
| expr '*' expr      {  arsc_plus(&$$,&$1,&$3); 
						ARSO_FREE_IF_IS_STRING($1);
						ARSO_FREE_IF_IS_STRING($3);
					 }
| expr '/' expr      {  arsc_div(&$$,&$1,&$3);
						ARSO_FREE_IF_IS_STRING($1);
						ARSO_FREE_IF_IS_STRING($3);
					 }
| '-' expr  %prec NEG{  arsc_neg(&$$,&$2); 
						ARSO_FREE_IF_IS_STRING($2);
					 }
| expr '^' expr      {  arsc_pow(&$$,&$1,&$3); 
						ARSO_FREE_IF_IS_STRING($1);
						ARSO_FREE_IF_IS_STRING($3);
					 }
| tk_exit			 { return 0;                        }
| tk_eof             { return 0;                        }
;

/* End of grammar.  */
%%

