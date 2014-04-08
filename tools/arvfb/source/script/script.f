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

DIGIT    [0-9]
HEX      [0][xX][0-9A-Fa-f]+
ID       [_a-zA-Z][_a-zA-Z0-9]*
CHAR     .

%%


"//"+[^\n]*			/* comment ignore */
"/*"+[^\n]*+"*/" 
"#"+[^\n]*

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

exit	{
			return tk_exit;	// exit
		}

\".*\"	{
			DEBUG_FLEX("Var:%s\n",yytext);
			yylval.tk_string.type = YVAR_STRING;
			yytext[strlen(yytext)-1] = '\0'; // eat ""
			yylval.tk_string.u.string = arso_strdup(&yytext[1]);
			return tk_string;
		}
		
\'{CHAR}\'	{
		DEBUG_FLEX("Var:%s\n",yytext);
		yylval.tk_obj.type = YVAR_CHAR;
		yylval.tk_obj.u.chr = yytext[0];
		return tk_obj;
		}		
{ID}	{
		DEBUG_FLEX("Var:%s\n",yytext);
		yylval.tk_obj.type = YVAR_STRING;
		yylval.tk_obj.u.string = arso_strdup(yytext);
		return tk_obj;
	}		

"+"|"-"|"*"|"/"|"="  {
			DEBUG_FLEX("Operator:%s\n",yytext);
			return yytext[0];
			}

"\n"		return '\n';

"{"[\^{}}]*"}"	/* eat up one-line comments */

[ \t]+			/* eat up whitespace */


.           return yytext[0];



%%



