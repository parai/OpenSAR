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
// do import name redefinition for the future add script interpret
#define yyparse 					ar_yyparse
#define yylex   					ar_yylex
#define yyin    					ar_yyin
#define yyout   					ar_yyout
#define yychar  					ar_yychar
#define yylval  					ar_yylval
#define yynerrs 					ar_yynerrs
#define yy_switch_to_buffer			ar_yy_switch_to_buffer
#define yy_delete_buffer			ar_yy_delete_buffer
#define yy_flush_buffer				ar_yy_flush_buffer
#define yy_create_buffer			ar_yy_create_buffer
#define yyrestart					ar_yyrestart
#define yypush_buffer_state			ar_yypush_buffer_state
#define yypop_buffer_state			ar_yypop_buffer_state
#define yy_scan_buffer				ar_yy_scan_buffer
#define yy_scan_bytes				ar_yy_scan_bytes
#define yy_scan_string				ar_yy_scan_string
#define yyget_lineno				ar_yyget_lineno
#define yyget_in					ar_yyget_in
#define yyget_out					ar_yyget_out
#define yyget_leng					ar_yyget_leng
#define yyget_text					ar_yyget_text
#define yyset_lineno				ar_yyset_lineno
#define yyset_in					ar_yyset_in
#define yyset_out					ar_yyset_out
#define yyget_debug					ar_yyget_debug
#define yyset_debug					ar_yyset_debug
#define yylex_destroy				ar_yylex_destroy
#define yyalloc						ar_yyalloc
#define yyrealloc					ar_yyrealloc
#define yyfree						ar_yyfree
#define yyerror						ar_yyerror
#define yywrap						ar_yywrap
#define yylineno					ar_yylineno
#define yy_flex_debug				ar_yy_flex_debug


#define YYERROR_VERBOSE
#define DEBUG_FLEX(...)  //printf(__VA_ARGS__)
#define DEBUG_BISON(...) printf(__VA_ARGS__)
int  yylex    (void);
void yyerror  (char const *);
#include "rule.yc"
#include "grammar.fc"

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
