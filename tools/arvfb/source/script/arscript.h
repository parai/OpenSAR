
#ifndef ARSCRIPT_H_
#define ARSCRIPT_H_
#include <sys/queue.h>
// ========================== [ TYPEs ]======================================
typedef enum
{
	ARS_STRING,
	ARS_INTEGER,
	ARS_DOUBLE,
	ARS_FUNCTION,
	ARS_UNDEFINE
}ArsObjTypeType;

typedef void (*ArsFpType)(void); // Function Pointer Type


typedef struct
{
	ArsObjTypeType  Type;
	union
	{
		char*   	String;
		int     	Integer;
		double  	Double;
		ArsFpType   Function;
	}Var;
}ArsValueType;

typedef struct ArsObj_t
{
	char* 			Name;
	ArsValueType    Value;
	TAILQ_ENTRY(ArsObj_t) Entry;
}ArsObjType;

void ArScript_Init(void);
void ArScript(int argc, char* argv[]);

#endif /* ARSCRIPT_H_ */
