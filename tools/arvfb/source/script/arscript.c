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
	printf("Script: %s\n",errMsg);
}

int yywrap (void )
{
	return 0;
}

void ArScript(int argc, char* argv[])
{
	--argc;++argv;	// skip the Program name

	puts("Test:Calc\n");
	yyparse();

}



