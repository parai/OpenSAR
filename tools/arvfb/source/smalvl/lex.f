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
     
DIGIT    [0-9]
HEX      [0][xX][0-9A-Fa-f]+

%%

	static unsigned int include_counter = 0;

"include" BEGIN(sc_include);

<sc_include>{
	[ \t]* /* eat the whitespace */ 

	[^ \t\n]+   { /* got the include module name */
		yyin = fopen( yytext, "r" );
		if ( ! yyin )
		{	char string[1024];
			sprintf(string,"Module <%s> is not exist",yytext);
			yyerror(string);
		}
		else
		{
			printf("##open include <%s>\n",yytext);
			include_counter ++; 
			yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
			BEGIN(INITIAL);
		}
	}
	
	"\n" BEGIN(INITIAL);
}

"require"  {
	return REQUIRE;
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
<STR>"\""	yy_pop_state(); return STRING_DEFINITION; 


[a-zA-Z_][a-zA-Z0-9_]* {
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


"//"	yy_push_state(SINGLE_LINE_COMMENT);
<SINGLE_LINE_COMMENT>{
	[^\n]*	;
	"\n"	{
		yy_pop_state(); 
	}
}

"/*" 				yy_push_state(MULTI_LINE_COMMENT);
<MULTI_LINE_COMMENT>{
		    [^(*/)]	;
		    "*/"	yy_pop_state();
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

. {
	yycolumn++;
}

\n {
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



