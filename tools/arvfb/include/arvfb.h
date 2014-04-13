/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */
#ifndef ARVFB_H_
#define ARVFB_H_
// =============================== [ SYSTEM INCLUDE ] ================================
#include <glib.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <Std_Types.h>
#include <math.h>
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
#include "arparser.h"



// =============================== [ ARVFB FUNCTIONS ] ==============================
extern boolean ArvfbPoll(ArPortType port,ArMsgType* pMsg);
extern void    ArvfbSend(ArPortType port,const ArMsgType* pMsg);

void Arch_Trace(const char* format,...);

#endif /* ARVFB_H_ */
