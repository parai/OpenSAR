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
%{
#include <cstdlib>
#include <cstring>
#include <string>

#include "analizator.h"

#define FLEX_DEBUG(x, ...) printf("FLEX DEBUG %d:", yylineno); printf(x, ##__VA_ARGS__);

int yycolumn = 0;
%}

%option yylineno
%option noyywrap
%option stack
%x SINGLE_LINE_COMMENT
%x MULTI_LINE_COMMENT
%x STR
%x sc_include
%x sc_require
%x sc_require2
     
DIGIT    [0-9]
HEX      [0][xX][0-9A-Fa-f]+
ID       [_a-zA-Z][_a-zA-Z0-9]*

%%

	static unsigned int include_counter = 0;

<*>"//"	yy_push_state(SINGLE_LINE_COMMENT);
<SINGLE_LINE_COMMENT>{
	[^\n]*	;
	"\n"	{
		yy_pop_state(); 
	}
}

<*>"/*"	yy_push_state(MULTI_LINE_COMMENT);
<MULTI_LINE_COMMENT>{
		    [^(*/)]	;
		    "*/"	yy_pop_state();
}

"include" BEGIN(sc_include);

<sc_include>{
	[ \t]* /* eat the whitespace */ 

	[^ \t\n]+   { /* got the include module name */
		yyin = fopen( yytext, "r" );
		if ( ! yyin )
		{	char string[1024];
			sprintf(string,"Include <%s> is not exist",yytext);
			yyerror(string);
		}
		else
		{
			include_counter ++; 
			yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
			BEGIN(INITIAL);
		}
	}
	
}

"require"  BEGIN(sc_require);

<sc_require>{
	[ \t]* /* eat the whitespace */ 

	[^ \t\n]+   { /* got the include module name */
		yyin = fopen( yytext, "r" );
		if ( ! yyin )
		{	char string[1024];
			sprintf(string,"Require <%s> is not exist",yytext);
			yyerror(string);
		}
		else
		{
			printf("##open require <%s>\n",yytext);
			yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
			BEGIN(sc_require2);
		}
	}
}

<sc_require2>{
	
	{ID}".so."{DIGIT}+	{	yylval.str = std::string(yytext); return LIB_NAME; 	}
	
	"INTEGER"|"STRING"|"FLOATPOINT"|"..."|"NONE" {
		yylval.str = std::string(yytext); 
		return TK_TYPE; 	 
	}
	{ID}		{	yylval.str = std::string(yytext); return IDENTIFIER; }		
	
	"("|")"|","|";"|"{"|"}" {
		return yytext[0];
	}
	<<EOF>>	{ yypop_buffer_state(); BEGIN(INITIAL); }
}



"return"  {
	return RETURN;
}

"break" {
    return BREAK;
}

"function"|"def" {
	return FUNCTION;
}

"on" {
	return ON;
}

"while" {
	return WHILE;
}

"true" {
  yylval.str = yytext;
  return TRUE;
}

"false" {
  yylval.str = yytext;
  return FALSE;
}

"unset" {
  yylval.str = yytext;
  return UNSET;
}

"++" {
  return INC;
}

"--" {
  return DEC;
}
  

"$" {
	return yytext[0];
}

"if" {
	return IF;
}

"else" {
	return ELSE;
}

"for" {
	return FOR;
}


"array[]" {
  //TODO: predefined array size
  return ARRAY_DECLARATION;
}


"\""	yy_push_state(STR); yylval.str = std::string();
<STR>\\n	yylval.str += std::string("\n");
<STR>\\t	yylval.str += std::string("\t");
<STR>\\\"	yylval.str += std::string("\"");
<STR>[^\"] 	yylval.str += std::string(yytext);
<STR>"\""	yy_pop_state(); return TK_STRING; 


{ID} {
	yylval.str = std::string(yytext);
	return IDENTIFIER;
}
 

{DIGIT}+ {
	yylval.str = std::string(yytext);
	return TK_INTEGER;
}

{HEX}		{
	yylval.str = std::string(yytext);
	return TK_INTEGER;
}

{DIGIT}+"."{DIGIT}*        {
	yylval.str = std::string(yytext);
	return TK_FLOATPOINT;
}

">=" {
	yylval.str = std::string(yytext);
	return MORE_OR_EQUAL;
}

"<=" {
	yylval.str = std::string(yytext);
	return LESS_OR_EQUAL;
}

"==" {
	yylval.str = std::string(yytext);
	return EQUAL;
}

"!=" {
	yylval.str = std::string(yytext);
	return NOT_EQUAL;
}


";"|","|"+"|"-"|"*"|"/"|"="|">"|"<"|"%"	{
	return yytext[0];
}

"{"|"}"|"("|")"|"["|"]" {
	return yytext[0];
}

"." {	// string concate operater, TODO: I don't like it
	return yytext[0];
}

<*>. {
	yycolumn++;
}

<*>\n {
	yycolumn=1;
}

<<EOF>>	{
	if(include_counter > 0)
	{
		yypop_buffer_state();
		include_counter --;
	}
	else
	{
		yyterminate();
	}
}
%%



