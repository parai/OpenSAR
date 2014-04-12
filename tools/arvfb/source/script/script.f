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
 * Sourrce Open At: https://github.com/parai/OpenSAR/
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
/* Start Conditions */
%x      sc_str

DIGIT    [0-9]
HEX      [0][xX][0-9A-Fa-f]+
ID       [_a-zA-Z][_a-zA-Z0-9]*
CHAR     .

%%

	#define MAX_STR_CONST 1024
	static char string_buf[MAX_STR_CONST];
	static char *string_buf_ptr = string_buf;

"#"+[^\n]*

\" 		{ string_buf_ptr = string_buf; yy_push_state(sc_str); }

<sc_str>{
	\" 		{ 	/* saw closing quote - all done */
				yy_pop_state();
				*string_buf_ptr++ = '\0';
				DEBUG_FLEX("Var:%s\n",string_buf);
				yylval.tk_string.u.string = arso_strdup(string_buf);
				return tk_string;
	}

	\n      {
                 /* error - unterminated string constant */
                 /* generate error message */
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

TRUE|true|True	{
			DEBUG_FLEX("True:%s\n",yytext);
			yylval.tk_integer.type = YVAR_INTEGER;
			yylval.tk_integer.u.integer = 1;
			return tk_integer;
		}

FALSE|false|False	{
			DEBUG_FLEX("False:%s\n",yytext);
			yylval.tk_integer.type = YVAR_INTEGER;
			yylval.tk_integer.u.integer = 0;
			return tk_integer;
		}

{DIGIT}+	{
			DEBUG_FLEX("Integer:%s\n",yytext);
			yylval.tk_integer.type = YVAR_INTEGER;
			yylval.tk_integer.u.integer = atoi(yytext);
			return tk_integer;
		}

{HEX}		{
			DEBUG_FLEX("Hex:%s\n",yytext);
			yylval.tk_integer.type = YVAR_INTEGER;
			yylval.tk_integer.u.integer = htoi(yytext);
			return tk_integer;
	
		}

{DIGIT}+"."{DIGIT}*        {
			DEBUG_FLEX("Double:%s\n",yytext);
			yylval.tk_double.type = YVAR_DOUBLE;
			yylval.tk_double.u.dvar = atof(yytext);
			return tk_double;
		}

exit	{	printf("##: %s\n",yytext);
			return tk_exit;	// exit
		}
		
\'{CHAR}\'	{
		DEBUG_FLEX("Var:%s\n",yytext);
		yylval.tk_char.type = YVAR_CHAR;
		yylval.tk_char.u.chr = yytext[0];
		return tk_char;
		}		
{ID}	{
		DEBUG_FLEX("Var:%s\n",yytext);
		yylval.tk_id.type = YVAR_STRING;
		yylval.tk_id.u.string = arso_strdup(yytext);
		return tk_id;
	}		

"+"|"-"|"*"|"/"|"="  {
			DEBUG_FLEX("Operator:%s\n",yytext);
			return yytext[0];
			}

"\n"	    { yylval.tk_char.type = YVAR_CHAR; 
			  yylval.tk_char.u.chr = yytext[0];
			  return tk_eol;
			}

"{"[\^{}}]*"}"	/* eat up one-line comments */

[ \t]+			/* eat up whitespace */

<<EOF>>		{ printf("EOF: %s\n",yytext); 
			  yylval.tk_char.type = YVAR_CHAR; 
			  yylval.tk_char.u.chr = yytext[0];
			  return tk_eof;
		    }

.           return yytext[0];



%%



