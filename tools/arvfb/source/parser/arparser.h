
#ifndef ARPARSER_H_
#define ARPARSER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>


// ============================== [ MACROS ] ======================================
#define ARPVAR_PDU_TYPE      0
#define ARPVAR_SIGNAL_TYPE   1

#define ARP_VAR(var)  (*((ArpVarType**) &var))

// ============================== [ TYPES  ] ======================================
typedef enum
{
	ARP_E_OK,
	ARP_E_NOT_OK
}ArpResultType;

typedef enum
{
	VAR_TYPE_STRING,
	VAR_TYPE_DOUBLE,
	VAR_TYPE_INTEGER
}ArpVarTypeType;

typedef struct ArpVarType
{
	ArpVarTypeType Type;
	union
	{
		char*    String;   // use malloc/free to process it
		double   Double;
		int      Integer;
	}Var;
	struct ArpVarType* Next;
}ArpVarType;


// ============================== [ FUNCTIONS ] ===================================
int  yylex    (void);
void yyerror  (char const *);


void ArParser(int argc, char* argv[]);

#endif /* ARPARSER_H_ */
