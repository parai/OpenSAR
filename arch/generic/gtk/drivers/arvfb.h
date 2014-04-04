
#ifndef ARVFB_H_
#define ARVFB_H_
// =============================== [ SYSTEM INCLUDE ] ================================
#include <glib.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <Std_Types.h>

// =============================== [ ARVFB MACROS  ] ================================
// -- MACROS FOR ArMsg Type
#define MSG_ON_CAN   ((uint32)0)
#define MSG_ON_LIN   ((uint32)1)
#define MSG_ON_DIO   ((uint32)2)
#define MSG_ON_PWM   ((uint32)3)
#define MSG_ON_ADC   ((uint32)4)
#define MSG_ON_GPT   ((uint32)5)
#define MSG_ON_SPI   ((uint32)6)

#define MSG_CMD_POLL     ((uint32)0x00000000)
#define MSG_CMD_SEND     ((uint32)0x00000001)
#define MSG_CMD_ACK      ((uint32)0x80000000) // Set the ACK bit


#define MSG_MIN_LENGTH   (12)
#define MAX_MSG_LENGTH   (256)

#define MSG_CAN_LENGTH   (14)


// =============================== [ ARVFB TYPES   ] ================================
typedef uint16 ArPortType;
typedef struct
{
	uint32 Type;
	uint32 Command;
	uint32 Length;
	uint8  Buffer[MAX_MSG_LENGTH];
}ArMsgType;

typedef 	struct
{
	uint32 Type;
	uint32 Command;
	uint32 Length; // inherit from ArMsg
	struct{
		uint32 Identifier;
		uint8  Data[8];
		uint8  DataLengthCode;
		uint8  BusID;
	}Msg;
}ArCanMsgType;


// =============================== [ ARVFB FUNCTIONS ] ==============================
extern void ArvfbInit(void);
extern boolean ArvfbCanProcess(const ArCanMsgType *ArMsgIn,ArCanMsgType *ArMsgOut);

void Arch_Trace(const char* format,...); // Import from Arch
#endif /* ARVFB_H_ */
