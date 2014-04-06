
#ifndef ARPARSER_H_
#define ARPARSER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>


// ============================== [ MACROS ] ======================================


// ============================== [ TYPES  ] ======================================
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
void ArParser(int argc, char* argv[]);

#endif /* ARPARSER_H_ */
