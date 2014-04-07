#line 2407 "./doc/bison.texi"

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
					fprintf (yyoutput, "%s = %s", $$->Name, $$->Value.Var.String); 
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
						printf ("\tStr = %s\n", $1.Var.String); 
						free($1.Var.String);
						break;
					case ARS_INTEGER:
						printf ("\tInteger = %d\n", $1.Var.Integer); 
						break;
					case ARS_DOUBLE:
						printf ("\tDouble = %g\n", $1.Var.Double); 
						break;
					case ARS_FUNCTION:
						printf ("\tFunction = %p\n", $1.Var.Function); 
						break;
					default:
						assert(0);
						break;
				} }
| error '\n' { yyerrok;                }
;



exp:
   yString           { ArsObjType* obj = GetObject($1.Var.String);
   	   	   	   	   	   if(NULL != obj)
   	   	   	   	   	   {
   	   	   	   	   		   $$.Type = obj->Value.Type;
   	   	   	   	   		   switch($$.Type)
						   {
								case ARS_STRING:
									$$.Var.String = strdup(obj->Value.Var.String);
									break;
								case ARS_INTEGER:
									$$.Var.Integer = obj->Value.Var.Integer; 
									break;
								case ARS_DOUBLE:
									$$.Var.Double = obj->Value.Var.Double; 
									break;
								case ARS_FUNCTION:
									$$.Var.Function = obj->Value.Var.Function; 
									break;
								default:
									puts("##: unknown type.\n");
									break;
						   }
   	   	   	   	   	   }
   	   	   	   	   	   else
   	   	   	   	   	   {
   	   	   	   	   		   $$.Type = ARS_STRING;
   	   	   	   	   	   	   $$.Var.String = $1.Var.String;
   	   	   	   	   	   }
	   	   	   	   	   
   	   	   	   	   	 }
|  yDouble			 { $$.Type = ARS_DOUBLE;
					   $$.Var.Double = $1.Var.Double;           			
					 }
|  yInteger			 { $$.Type = ARS_INTEGER;
					   $$.Var.Integer = $1.Var.Integer;           			
					 }
|  yString '=' exp	 {  ArsObjType* obj = GetObject($1.Var.String);
					    if(NULL == obj)
					    {
					    	obj = AddObject($1.Var.String,&$3);
					    }
					    else
					    {
					    	SetObjectValue(obj,&$3);  
					    }
					    $$.Type = obj->Value.Type;
					    switch($$.Type)
					    {
							case ARS_STRING:
								$$.Var.String = strdup(obj->Value.Var.String);
								break;
							case ARS_INTEGER:
								$$.Var.Integer = obj->Value.Var.Integer; 
								break;
							case ARS_DOUBLE:
								$$.Var.Double = obj->Value.Var.Double; 
								break;
							case ARS_FUNCTION:
								$$.Var.Function = obj->Value.Var.Function; 
								break;
							default:
								puts("##: unknown type.\n");
								break;
					   }
					 }
| yString '(' exp ')'{	 Execute(&$$,&$1,&$3);           }
| exp '+' exp        {   Calc_Add(&$$,&$1,&$3);          }
| exp '-' exp        {   Calc_Sub(&$$,&$1,&$3);          }
| exp '*' exp        {   Calc_Plus(&$$,&$1,&$3);         }
| exp '/' exp        {   Calc_Div(&$$,&$1,&$3);          }
| '-' exp  %prec NEG {   Calc_Neg(&$$,&$2);              }
| exp '^' exp        {   Calc_Pow(&$$,&$1,&$3);          }
| '(' exp ')'        {   memcpy(&$$,&$2,sizeof(ArsValueType));           }
| yExit              { return 0;                         }
;

/* End of grammar.  */
%%

