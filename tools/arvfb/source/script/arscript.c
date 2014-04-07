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
#define DEBUG_BISON(...) //printf(__VA_ARGS__)
int  yylex    (void);
void yyerror  (char const *);
#include "script.yc"
#include "script.fc"

// ================================ [ FUNCTIONS ] ========================================
void yyerror  (char const * errMsg)
{
	assert(errMsg!=NULL);
	printf("Script: %s\n",errMsg);
}

int yywrap (void )
{
	return 0;
}

void ArScript_Init(void)
{
	ArsValueType arsValue;

	arso_init();

	arsValue.Type = ARS_FUNCTION;

	arsValue.Var.Function = (ArsFpType)sin;
	arso_add("sin",	&arsValue);
	arsValue.Var.Function = (ArsFpType)cos;
	arso_add("cos",	&arsValue);
	arsValue.Var.Function = (ArsFpType)atan;
	arso_add("atan",	&arsValue);
	arsValue.Var.Function = (ArsFpType)exp;
	arso_add("exp",	&arsValue);
	arsValue.Var.Function = (ArsFpType)log;
	arso_add("ln",		&arsValue);
	arsValue.Var.Function = (ArsFpType)sqrt;
	arso_add("sqrt",	&arsValue);
}
void ArScript(int argc, char* argv[])
{
	--argc;++argv;	// skip the Program name

	puts("Test:Calc\n");
	yyparse();

}



