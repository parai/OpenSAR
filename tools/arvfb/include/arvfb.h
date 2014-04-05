
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


// =============================== [ ARVFB TYPES   ] ================================
typedef uint16 ArPortType;
typedef struct
{
	uint32 Type;
	uint32 Command;
	uint32 Length;
	uint8  Buffer[MAX_MSG_LENGTH];
}ArMsgType;


// =============================== [ ARVFB INCLUDE ] ================================
#include "arcan.h"
#include "ardl.h"
#include "artp.h"
#include "arfl.h"
#include "arcom.h"


// =============================== [ ARVFB FUNCTIONS ] ==============================
extern boolean ArvfbPoll(ArPortType port,ArMsgType* pMsg);
extern void    ArvfbSend(ArPortType port,const ArMsgType* pMsg);

#endif /* ARVFB_H_ */
