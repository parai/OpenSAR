/* require flex 2.5 */

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
 * Source Open At: https://github.com/parai/OpenSAR/
 */

/* I feel happy, flex is really very powerful,  
 * Compare it with python.re, it is some how just that
 * I like this things, start for fun
 */

%{
	/* need this for the call to atof() below */
	#include <math.h>
%}

%option yylineno
%option noyywrap
%option stack
%option nounput
%option noinput
/* Start Conditions */
%x      sc_str
%x      sc_import

DIGIT    [0-9]
HEX      [0][xX][0-9A-Fa-f]+
ID       [_a-zA-Z][_a-zA-Z0-9]*
CHAR     .

%%

	#define MAX_STR_CONST 1024
	char string_buf[MAX_STR_CONST];
	char *string_buf_ptr = string_buf;
	unsigned int import_counter = 0;

"#"+[^\n]*

\" 		{ string_buf_ptr = string_buf; yy_push_state(sc_str); }

<sc_str>{
	\" 		{ 	/* saw closing quote - all done */
				yy_pop_state();
				*string_buf_ptr++ = '\0';
				DEBUG_FLEX("Var:%s\n",string_buf);
				yylval.var.type     = YVAR_STRING;
				yylval.var.u.string = arso_strdup(string_buf);
				return tk_string;
	}

	\n      {
                 /* error - unterminated string constant */
                 /* generate error message */
				 yyerror("unterminated string constant");
    }

	\\[0-7]{1,3} {
		/* octal escape sequence */
		 int result;
	
		 (void) sscanf( yytext + 1, "%o", &result );
	
		 if ( result > 0xff )
				 /* error, constant is out-of-bounds */
	
		 *string_buf_ptr++ = result;
	 }

	\\[0-9]+ {
		 /* generate error - bad escape sequence; something
		  * like '\48' or '\0777777'
		  */
		yyerror("bad escape sequence;like '\48' or '\0777777'");
	 }

	\\n  *string_buf_ptr++ = '\n';
	\\t  *string_buf_ptr++ = '\t';
	\\r  *string_buf_ptr++ = '\r';
	\\b  *string_buf_ptr++ = '\b';
	\\f  *string_buf_ptr++ = '\f';

	\\(.|\n)  *string_buf_ptr++ = yytext[1];
	
	[^\\\n\"]+   {
		 char *yptr = yytext;

		 while ( *yptr )
				 *string_buf_ptr++ = *yptr++;
	}
}

import             BEGIN(sc_import);

<sc_import>{
	[ \t]* /* eat the whitespace */ 

	[^ \t\n]+   { /* got the import module name */
		yyin = fopen( yytext, "r" );
		if ( ! yyin )
		{	char string[1024];
			sprintf(string,"Module <%s> is not exist",yytext);
			yyerror(string);
		}
		else
		{
			import_counter ++; 
			yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
			BEGIN(INITIAL);
		}
	}
}

TRUE|true|True	{
			DEBUG_FLEX("True:%s\n",yytext);
			yylval.var.type = YVAR_INTEGER;
			yylval.var.u.integer = 1;
			return tk_integer;
		}

FALSE|false|False	{
			DEBUG_FLEX("False:%s\n",yytext);
			yylval.var.type = YVAR_INTEGER;
			yylval.var.u.integer = 0;
			return tk_integer;
		}

{DIGIT}+	{
			DEBUG_FLEX("Integer:%s\n",yytext);
			yylval.var.type = YVAR_INTEGER;
			yylval.var.u.integer = atoi(yytext);
			return tk_integer;
		}

{HEX}		{
			DEBUG_FLEX("Hex:%s\n",yytext);
			yylval.var.type = YVAR_INTEGER;
			yylval.var.u.integer = htoi(yytext);
			return tk_integer;
	
		}

{DIGIT}+"."{DIGIT}*        {
			DEBUG_FLEX("Double:%s\n",yytext);
			yylval.var.type = YVAR_DOUBLE;
			yylval.var.u.dvar = atof(yytext);
			return tk_double;
		}

exit	{	printf("##: %s\n",yytext);
			return tk_exit;	// exit
		}
		
\'{CHAR}\'	{
		DEBUG_FLEX("Var:%s\n",yytext);
		yylval.var.type = YVAR_CHAR;
		yylval.var.u.chr = yytext[0];
		return tk_char;
		}		
{ID}	{
		DEBUG_FLEX("Var:%s\n",yytext);
		yylval.var.type = YVAR_STRING;
		yylval.var.u.string = arso_strdup(yytext);
		return tk_id;
	}		

"+"|"-"|"*"|"/"|"="  {
			DEBUG_FLEX("Operator:%s\n",yytext);
			return yytext[0];
			}
"\n"|";"	{
				DEBUG_FLEX("EOL:%s\n",yytext);
				return yytext[0];
			}

"{"|"}"|"("|")"		{
				DEBUG_FLEX("BRACK:%s\n",yytext);
				return yytext[0];
			}

[ \t]+			/* eat up whitespace */

.           return yytext[0];


<<EOF>>	{
			if(import_counter > 0)
			{
				yypop_buffer_state();
				import_counter --;
				
				if ( !YY_CURRENT_BUFFER )
				{
					yyterminate();
					return tk_eof;
				}
				else
				{
					BEGIN(INITIAL);
				}
			}
			else
			{
				DEBUG_FLEX("End of file.\n");
				yyterminate();
				return tk_eof;
			}
        }

%%



