#include "arparser.h"
#include "arvfb.h"
// ================================ [ IMPORTS  ] ========================================
extern FILE *yyin, *yyout;

extern int yylex (void);

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

ArpResultType ArpProcess(ArpVarType* Var)
{
	return ARP_E_OK;
}

const char*  ArpErrorMsg(void)
{
	return "Hello World!\n";
}


void ArParser(int argc, char* argv[])
{
	--argc;++argv;	// skip the Program name

	if(argc)
	{
		yyin = fopen(*argv,"r");
	}
	if(NULL==yyin)
	{
		yyin = stdin;
	}
	yyout = stdout;

	yylex();

}
