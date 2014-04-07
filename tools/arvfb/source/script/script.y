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
%}

%define api.value.type   union  
%token <ArsValueType> 	yDouble
%token <ArsValueType>   yInteger
%token <ArsValueType>   yExit
%token <ArsValueType>   yString
%type  <ArsValueType>   exp


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
%printer {  switch($$->Value.Type)
			{
				case ARS_STRING:
					fprintf (yyoutput, "%s%s", $$->Name, $$->Value.Var.String); 
					break;
				case ARS_INTEGER:
					fprintf (yyoutput, "%s = %d", $$->Name, $$->Value.Var.Integer); 
					break;
				case ARS_DOUBLE:
					fprintf (yyoutput, "%s = %g", $$->Name, $$->Value.Var.Double); 
					break;
				case ARS_FUNCTION:
					fprintf (yyoutput, "%s = %p", $$->Name, $$->Value.Var.Function); 
					break;
				default:
					assert(0);
					break;
			}
		} <ArsObjType*>;
%printer { switch($$.Type)
			{
				case ARS_STRING:
					fprintf (yyoutput, "%s", $$.Var.String); 
					break;
				case ARS_INTEGER:
					fprintf (yyoutput, "%d", $$.Var.Integer); 
					break;
				case ARS_DOUBLE:
					fprintf (yyoutput, "%g", $$.Var.Double); 
					break;
				case ARS_FUNCTION:
					fprintf (yyoutput, "%p", $$.Var.Function); 
					break;
				default:
					assert(0);
					break;
			}
		} <ArsValueType>;

%% /* The grammar follows.  */

input:
  %empty
| input line
;



line:
  '\n'
| exp '\n'   { switch($1.Type)
				{
					case ARS_STRING:
						printf ("\t(String)%s\n", $1.Var.String); 
						// as result has already been output, free
						arso_strfree($1.Var.String);
						break;
					case ARS_INTEGER:
						printf ("\t(Integer)%d\n", $1.Var.Integer); 
						break;
					case ARS_DOUBLE:
						printf ("\t(Double)%g\n", $1.Var.Double); 
						break;
					case ARS_FUNCTION:
						printf ("\t(Function)%p\n", $1.Var.Function); 
						break;
					default:
						assert(0);
						break;
				} }
| error '\n' { yyerrok;                }
;



exp:
   yString           { ArsObjType* obj = arso_get($1.Var.String);
   	   	   	   	   	   if(NULL != obj)
   	   	   	   	   	   {
   	   	   	   	   		   arso_read(obj,&$$);
   	   	   	   	   		   arso_strfree($1.Var.String);  // $1 is not needed any more 
   	   	   	   	   	   }
   	   	   	   	   	   else
   	   	   	   	   	   {
   	   	   	   	   		   arsc_copy(&$$,&$1);		
   	   	   	   	   		   // as $$ is a copy of $1, no need to free $1 string as still in use
   	   	   	   	   	   }
   	   	   	   	   	 }
|  yDouble			 { arsc_copy(&$$,&$1);        			
					 }
|  yInteger			 { arsc_copy(&$$,&$1);        			
					 }
|  yString '=' exp	 {  ArsObjType* obj = arso_get($1.Var.String);
					    if(NULL == obj)
					    {	// New it, 
					    	obj = arso_add($1.Var.String,&$3);
					    	arso_strfree($1.Var.String);
					    	if(ARS_STRING == $3.Type)
					    	{	// free it if $3 is string
					    		arso_strfree($3.Var.String);
					    	}
					    }
					    else
					    {
					    	arso_write(obj,&$3);  
					    }
					    arso_read(obj,&$$);
					 }
| yString '(' exp ')'{	 arsc_eval(&$$,&$1,&$3);         }
| exp '+' exp        {   arsc_add(&$$,&$1,&$3);          }
| exp '-' exp        {   arsc_sub(&$$,&$1,&$3);          }
| exp '*' exp        {   arsc_plus(&$$,&$1,&$3);         }
| exp '/' exp        {   arsc_div(&$$,&$1,&$3);          }
| '-' exp  %prec NEG {   arsc_neg(&$$,&$2);              }
| exp '^' exp        {   arsc_pow(&$$,&$1,&$3);          }
| '(' exp ')'        {   arsc_copy(&$$,&$2);             }
| yExit              { return 0;                        }
;

/* End of grammar.  */
%%

