#include "arparser.h"
#include "arvfb.h"

// ================================ [ LOCAL VARIANTS ] ==================================
static ArpVarType* arVarTable=NULL;  // storing the var parse from yylex

// ================================ [ LOCAL FUNCTIONS ] =================================

// Now, for type String only
static ArpVarType* putVarString (char* string)
{
	ArpVarType *ptr = (ArpVarType*) malloc (sizeof (ArpVarType));

	ptr->Var.String=(char*) malloc (strlen(string) + 1);
	strcpy (ptr->Var.String,string);

	ptr->Type = VAR_TYPE_STRING;
	ptr->Next = (ArpVarType*)arVarTable;
	arVarTable = ptr;
	return ptr;
}

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

// ================================ [     IMPORTS    ] ==================================

#define YYERROR_VERBOSE

#define DEBUG_FLEX(...)  //printf(__VA_ARGS__)
#define DEBUG_BISON(...) printf(__VA_ARGS__)

#include "rule.yc"
#include "grammar.yc"

// ================================ [ FUNCTIONS ] ========================================
void yyerror  (char const * errMsg)
{
	assert(errMsg!=NULL);
	puts(errMsg);
}

int yywrap (void )
{
	return 0;
}

void ArParser(int argc, char* argv[])
{
	--argc;++argv;	// skip the Program name
	if(argc)
	{
		yyin = fopen(*argv,"r");
	}
	else
	{
		yyin = fopen("../../app/config/GEN/arvfb.config","r");
		puts("parse on arvfb.config\n");
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
