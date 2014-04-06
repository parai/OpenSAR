/* require flex 2.5 */

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

exit	{
			yylval.yExit = putVarString(yytext);
			return yExit;	// exit
		}

{STRING}	{
			DEBUG_FLEX("Var:%s\n",yytext);
			yylval.yVar = putVarString(yytext);
			return yVar;
		}

"+"|"-"|"*"|"/"   {
			DEBUG_FLEX("Operator:%s\n",yytext);
			return yytext[0];
			}

"\n"		return '\n';

"{"[\^{}}]*"}"	/* eat up one-line comments */

[ \t]+			/* eat up whitespace */


.           return yytext[0];



%%



