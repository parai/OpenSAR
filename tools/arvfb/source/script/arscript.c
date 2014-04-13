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
#include "arvfb.h"

// ============================= [ LOCAL TYPES  ] =======================================

// ================================ [ LOCAL VARIANTS ] ==================================

// ================================ [ LOCAL FUNCTIONS ] =================================
// change hex ascii to integer
static int htoi(char* string)
{
	int rv=0;
	++string;++string; // skip the prefix 0x or 0X
	while(*string != '\0')
	{
		if(isdigit(*string))
		{
			rv = rv*16 + (*string) - '0';
		}
		else if(isalnum(*string))
		{
			if(isupper(*string))
			{
				rv = rv*16 + (*string) - 'A' + 10;
			}
			else
			{
				rv = rv*16 + (*string) - 'a' + 10;
			}
		}
		else
		{
			assert(0);
		}

		++string;

	}

	return rv;
}
// ========================== [ IMPORTS ] ====================================
#define YYERROR_VERBOSE
#define DEBUG_FLEX(...)  //printf(__VA_ARGS__)
#define DEBUG_BISON(...) printf(__VA_ARGS__)
int  yylex    (void);
void yyerror  (char const *);
#include "script.yc"
#include "script.fc"

// ================================ [ FUNCTIONS ] ========================================
void yyerror  (char const * errMsg)
{
	assert(errMsg!=NULL);
	printf("Script Error: %s at line %d\n",errMsg,yylineno);
}

//int yywrap (void )
//{
//	return 0;
//}

void ArScript_Init(void)
{
	yvar_t var;

	arso_init();

	var.type = YVAR_FUNCTION;

	var.u.function = (yfp_t)sin;
	arso_new("sin",	&var);
	var.u.function = (yfp_t)cos;
	arso_new("cos",	&var);
	var.u.function = (yfp_t)atan;
	arso_new("atan",	&var);
	var.u.function = (yfp_t)exp;
	arso_new("exp",	&var);
	var.u.function = (yfp_t)log;
	arso_new("ln",		&var);
	var.u.function = (yfp_t)sqrt;
	arso_new("sqrt",	&var);
}
void ArScript(int argc, char* argv[])
{
	--argc;++argv;	// skip the Program name

	puts("Script:\n");
	if(argc==2)
	{
		yyin = fopen(argv[1],"r");
	}
	else
	{
		yyin = fopen("../../app/config/GEN/arvfb.spy","r");
		puts("script on arvfb.spy\n");
	}
	if(NULL==yyin)
	{
		yyin = stdin;
	}
	yyout = stdout;

	yyparse();

	if(yyin != stdin)
	{
		fclose(yyin);
	}

}



