
#ifndef ARPARSER_H_
#define ARPARSER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


// ============================== [ MACROS ] ======================================
#define ARPVAR_PDU_TYPE      0
#define ARPVAR_SIGNAL_TYPE   1

// ============================== [ TYPES  ] ======================================
typedef enum
{
	ARP_E_OK,
	ARP_E_NOT_OK
}ArpResultType;

typedef struct
{	// see arcom.c ArComPduType
	uint32_t Identifier;
	uint8_t  BusID;
	uint8_t  IsTxEnabled;
	uint32_t Period;
}ArpPduType;

typedef struct
{
	uint8_t  StartBit;
	uint8_t  BitSize;
}ArpSignalType;

typedef struct
{
	uint32_t type;
	char*    Name;		      // use malloc/free to process it
	union
	{
		ArpPduType    Pdu;
		ArpSignalType Signal;
	}Var;
}ArpVarType;


// ============================== [ FUNCTIONS ] ===================================
int  yylex    (void);
void yyerror  (char const *);
ArpResultType ArpProcess(ArpVarType* Var);
const char*  ArpErrorMsg(void);

void ArParser(int argc, char* argv[]);

#endif /* ARPARSER_H_ */
