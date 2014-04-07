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
STRING   [_a-zA-Z][_a-zA-Z0-9]*

%%

"//"+[^\n]*	
"/*"+[^\n]*+"*/" 

TRUE|true|True	{
			DEBUG_FLEX("True:%s\n",yytext);
			yylval.yInteger = 1;
			return yInteger;
		}

FALSE|false|False	{
			DEBUG_FLEX("False:%s\n",yytext);
			yylval.yInteger = 0;
			return yInteger;
		}

{DIGIT}+	{
			DEBUG_FLEX("Integer:%s\n",yytext);
			yylval.yInteger = atoi(yytext);
			return yInteger;
		}

{HEX}		{
			DEBUG_FLEX("Hex:%s\n",yytext);
			yylval.yInteger = htoi(yytext);
			return yInteger;
	
		}

{DIGIT}+"."{DIGIT}*        {
			DEBUG_FLEX("Double:%s\n",yytext);
			yylval.yDouble = atof(yytext);
			return yDouble;
		}

^Pdu 	{
			DEBUG_FLEX("Pdu:%s\n",yytext);
			yylval.yPdu = 0;	// value is ignored
			return yPdu;
		}

^Signal  {
			DEBUG_FLEX("Signal:%s\n",yytext);
			yylval.ySignal = 0;	// value is ignored
			return ySignal;
		}

exit	{
			yylval.yExit = 0;
			return yExit;	// exit
		}

{STRING}	{
			DEBUG_FLEX("Var:%s\n",yytext);
			yylval.yString = strdup(yytext);
			return yString;
		}

"\n"	{return '\n';}

"+"|"-"|"*"|"/"   DEBUG_FLEX( "An operator: %s\n", yytext );

"{"[\^{}}]*"}"	/* eat up one-line comments */

[ \t]+			/* eat up whitespace */


.           DEBUG_FLEX( "Unrecognized character: %s\n", yytext );



%%


