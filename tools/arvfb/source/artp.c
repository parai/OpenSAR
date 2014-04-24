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
 * Source Open At: https://github.com/parai/OpenSAR/
 */

#include "arvfb.h"

// ======================== [ LOCAL TYPES  ] ==============================================
typedef enum
{
	CANTP_ST_IDLE = 0,
	CANTP_ST_START_TO_SEND,
	CANTP_ST_SENDING,
	CANTP_ST_WAIT_FC,
	CANTP_ST_WAIT_CF,
	CANTP_ST_SEND_CF,
	CANTP_ST_SEND_FC
}CanTpState_Type;

typedef struct
{
	uint8   rxBuffer[1024*64];
	uint16  rxLength;
	uint16  rxIndex;
	uint32  rxId;

	uint8  txBuffer[1024*64];
	uint16  txLength;
	uint16  txIndex;
	uint32  txId;

	uint8  BS;
	uint8  SN;	// sequence number
	uint8  STMin;

	CanTpState_Type state;

}CanTpArch_Type;
// ======================== [ LOCAL MACROS  ] ==============================================
#define ISO15765_TPCI_MASK      0x30
#define ISO15765_TPCI_SF        0x00  /* Single Frame */
#define ISO15765_TPCI_FF        0x10  /* First Frame */
#define ISO15765_TPCI_CF        0x20  /* Consecutive Frame */
#define ISO15765_TPCI_FC        0x30  /* Flow Control */
#define ISO15765_TPCI_DL        0x7   /* Single frame data length mask */
#define ISO15765_TPCI_FS_MASK   0x0F  /* Flow control status mask */


#define ISO15765_FLOW_CONTROL_STATUS_CTS        0
#define ISO15765_FLOW_CONTROL_STATUS_WAIT       1
#define ISO15765_FLOW_CONTROL_STATUS_OVFLW      2

#define cfgSTmin  10
#define cfgBS     8

// ======================== [ LOCAL VARIANTS  ] ==============================================
static CanTpArch_Type sArchTp;
static GTimer* pSysTimer;
static gboolean isSysTimerStarted;

// ======================== [ LOCAL FUNCTIONS  ] ==============================================
static void     StartTimer(void);
static void     StopTimer(void);
static gboolean IsTimerElapsed(gulong microseconds);

static void SendSF(void);
static void SendFF(void);
static void SendFC(void);
static void SendCF(void);
static void SendStart(void);
static void HandleSF(const uint8* data,uint16 size);
static void HandleFF(const uint8* data,uint16 size);
static void HandleCF(const uint8* data,uint16 size);
static void HandleFC(const uint8* data,uint16 size);

static void StartTimer(void)
{
	g_timer_start(pSysTimer);
	isSysTimerStarted = TRUE;
}

static void StopTimer(void)
{
	g_timer_stop(pSysTimer);
	isSysTimerStarted = FALSE;
}
static gboolean IsTimerElapsed(gulong microseconds)
{
	gboolean rv;
	if(isSysTimerStarted)
	{
		gdouble elapsed;
		gulong  elapsed_microseconds;

		elapsed = g_timer_elapsed(pSysTimer,NULL);
		elapsed_microseconds = (elapsed*1000);

		if(elapsed_microseconds>microseconds)
		{
			rv = TRUE;
		}
		else
		{
			rv = FALSE;
		}
	}
	else
	{
		rv = TRUE;
	}
	return rv;
}
static void SendSF(void)
{
	ArMsgType armsg;
	ArCanMsgType* pMsg = (ArCanMsgType*)&armsg;
	pMsg->Type    = MSG_ON_CAN;
	pMsg->Command = MSG_CMD_SEND;
	pMsg->Length  = MSG_CAN_LENGTH;
	pMsg->Msg.BusID = 0; // Only on Bus0
	pMsg->Msg.Identifier = sArchTp.txId;
	pMsg->Msg.DataLengthCode = 8;
	pMsg->Msg.Data[0] = sArchTp.txLength|ISO15765_TPCI_SF;

	memcpy(&(pMsg->Msg.Data[1]),sArchTp.txBuffer,sArchTp.txLength);

	Can_Transmit(&armsg);

	sArchTp.state = CANTP_ST_IDLE;
}
static void SendFF(void)
{
	ArMsgType armsg;
	ArCanMsgType* pMsg = (ArCanMsgType*)&armsg;
	pMsg->Type    = MSG_ON_CAN;
	pMsg->Command = MSG_CMD_SEND;
	pMsg->Length  = MSG_CAN_LENGTH;
	pMsg->Msg.BusID = 0; // Only on Bus0
	pMsg->Msg.Identifier = sArchTp.txId;
	pMsg->Msg.DataLengthCode = 8;
	pMsg->Msg.Data[0] = ((sArchTp.txLength>>8)&0x0F)|ISO15765_TPCI_FF;
	pMsg->Msg.Data[1] = (sArchTp.txLength&0xFF);

	memcpy(&(pMsg->Msg.Data[2]),sArchTp.txBuffer,6);
	sArchTp.txIndex = 6;

	Can_Transmit(&armsg);

	sArchTp.state = CANTP_ST_WAIT_FC;
	sArchTp.SN = 0;
}
static void SendCF(void)
{
	ArMsgType armsg;
	ArCanMsgType* pMsg = (ArCanMsgType*)&armsg;
	uint16 lsize = sArchTp.txLength-sArchTp.txIndex;


	pMsg->Type    = MSG_ON_CAN;
	pMsg->Command = MSG_CMD_SEND;
	pMsg->Length  = MSG_CAN_LENGTH;
	pMsg->Msg.BusID = 0; // Only on Bus0
	pMsg->Msg.Identifier = sArchTp.txId;
	pMsg->Msg.DataLengthCode = 8;

	if(lsize>7)
	{
		lsize=7;
	}

	sArchTp.SN++;
	if(sArchTp.SN>15)
	{
		sArchTp.SN=0;
	}
	pMsg->Msg.Data[0] = ISO15765_TPCI_CF|sArchTp.SN;

	memcpy(&(pMsg->Msg.Data[1]),&(sArchTp.txBuffer[sArchTp.txIndex]),lsize);
	sArchTp.txIndex+=lsize;

	Can_Transmit(&armsg);

	if(sArchTp.txIndex>=sArchTp.txLength)
	{
		sArchTp.state = CANTP_ST_IDLE;
	}
	else
	{
		if(sArchTp.BS > 1)
		{
			sArchTp.BS--;
			if (1 == sArchTp.BS)
			{
				sArchTp.state = CANTP_ST_WAIT_FC;
			}
		}
	}
}
static void SendFC(void)
{
	ArMsgType armsg;
	ArCanMsgType* pMsg = (ArCanMsgType*)&armsg;
	pMsg->Type    = MSG_ON_CAN;
	pMsg->Command = MSG_CMD_SEND;
	pMsg->Length  = MSG_CAN_LENGTH;
	pMsg->Msg.BusID = 0; // Only on Bus0
	pMsg->Msg.Identifier = sArchTp.txId;
	pMsg->Msg.DataLengthCode = 8;
	pMsg->Msg.Data[0] = ISO15765_TPCI_FC;
	pMsg->Msg.Data[1] = cfgBS;
	pMsg->Msg.Data[2] = cfgSTmin;

	for(int i=3;i<8;i++)
	{
		pMsg->Msg.Data[i] = 0x55;
	}

	Can_Transmit(&armsg);

	if(0==cfgBS)
	{
		sArchTp.BS=0;
	}
	else
	{
		sArchTp.BS=cfgBS+1;
	}
	sArchTp.state = CANTP_ST_WAIT_CF;
}

static void SendStart(void)
{
	if(sArchTp.txLength<7)
	{
		SendSF();
	}
	else
	{
		SendFF();
	}
	StartTimer();
}
static void HandleSF(const uint8* data,uint16 size)
{
	uint16 lenSF = data[0]&ISO15765_TPCI_DL;
	memcpy(sArchTp.rxBuffer,&data[1],lenSF);
	sArchTp.rxLength = lenSF;
	sArchTp.state = CANTP_ST_IDLE;

	sArchTp.SN    = 0;

	CanTp_RxIndication(sArchTp.rxBuffer,sArchTp.rxLength);
}
static void HandleFF(const uint8* data,uint16 size)
{
	g_assert(8==size);
	sArchTp.rxLength = ((uint16)(data[0]&0x0F)<<8) + (uint16)data[1];
	memcpy(sArchTp.rxBuffer,&data[2],6);
	sArchTp.rxIndex=6;
	sArchTp.state = CANTP_ST_SEND_FC;
}

static void HandleCF(const uint8* data,uint16 size)
{
	uint16 lsize = sArchTp.rxLength-sArchTp.rxIndex;
	if(lsize>7)
	{
		lsize=7;
	}
	sArchTp.SN++;
	if(sArchTp.SN>15)
	{
		sArchTp.SN=0;
	}
	if((data[0]&0x0F) == sArchTp.SN)
	{
		memcpy(&(sArchTp.rxBuffer[sArchTp.rxIndex]),&data[1],lsize);
		sArchTp.rxIndex+=lsize;

		if(sArchTp.rxIndex>=sArchTp.rxLength)
		{
			CanTp_RxIndication(sArchTp.rxBuffer,sArchTp.rxLength);
			sArchTp.state = CANTP_ST_IDLE;
		}
		else
		{
			if(sArchTp.BS > 1)
			{
				sArchTp.BS--;
			    if (1 == sArchTp.BS)
			    {
			    	sArchTp.state = CANTP_ST_SEND_FC;
			    }
			}
		}
	}
	else
	{	// Error
		sArchTp.state = CANTP_ST_IDLE;
	}

}
static void HandleFC(const uint8* data,uint16 size)
{
	switch (data[0]&ISO15765_TPCI_FS_MASK) //CTS
	{
		case ISO15765_FLOW_CONTROL_STATUS_CTS:
			if(data[1] != 0)
			{
				sArchTp.BS = data[1] + 1;
			}
			else
			{
				sArchTp.BS = 0; // receive all without FC
			}
			sArchTp.STMin = data[2];
			sArchTp.state = CANTP_ST_SEND_CF;
			break;
		case ISO15765_FLOW_CONTROL_STATUS_WAIT:
			sArchTp.state = CANTP_ST_WAIT_FC;
			break;
		case ISO15765_FLOW_CONTROL_STATUS_OVFLW:
			sArchTp.state = CANTP_ST_IDLE;
			break;
		default:
			break;
	}
}

// ======================== [    FUNCTIONS    ] ==============================================
void ArTp_RxIndication(const ArCanMsgType* armsg)
{
	if((sArchTp.rxId == armsg->Msg.Identifier) && (0==armsg->Msg.BusID))
	{
		switch((armsg->Msg.Data[0]&ISO15765_TPCI_MASK))
		{
			case (ISO15765_TPCI_SF):
				HandleSF(armsg->Msg.Data,armsg->Msg.DataLengthCode);
				break;
			case (ISO15765_TPCI_FF):
				HandleFF(armsg->Msg.Data,armsg->Msg.DataLengthCode);
				break;
			case (ISO15765_TPCI_CF):
				HandleCF(armsg->Msg.Data,armsg->Msg.DataLengthCode);
				break;
			case (ISO15765_TPCI_FC):
				HandleFC(armsg->Msg.Data,armsg->Msg.DataLengthCode);
				break;
			default:
				break;
		}
		// Timer process
		switch((armsg->Msg.Data[0]&ISO15765_TPCI_MASK))
		{
			case (ISO15765_TPCI_SF):
			case (ISO15765_TPCI_FF):
			case (ISO15765_TPCI_CF):
			case (ISO15765_TPCI_FC):
				StartTimer();
				break;
			default:
				break;
		}
	}
}

void ArTp_Init(void)
{
	memset(&sArchTp,0,sizeof(sArchTp));

	// Config
	sArchTp.rxId = 0x732;
	sArchTp.txId = 0x731;

	isSysTimerStarted = FALSE;
	pSysTimer = g_timer_new();

	StopTimer();
}

Std_ReturnType CanTp_Transmit(uint8* data,uint16 size)
{
	Std_ReturnType ercd = E_OK;
	if(CANTP_ST_IDLE==sArchTp.state)
	{
		g_assert(NULL != data);
		g_assert((0!=size) && (size<4096));
		memcpy(sArchTp.txBuffer,data,size);
		sArchTp.txLength = size;
		sArchTp.state = CANTP_ST_START_TO_SEND;

		StartTimer();
	}
	else
	{
		return E_NOT_OK;
	}
	return ercd;
}

void ArTp_Schedule(void)
{
	switch(sArchTp.state)
	{
		case CANTP_ST_START_TO_SEND:
			SendStart();
			break;
		case CANTP_ST_SEND_FC:
			if(IsTimerElapsed(10))
			{
				SendFC();
				StartTimer();
			}
			break;
		case CANTP_ST_SEND_CF:
			if(IsTimerElapsed(10))
			{
				SendCF();
				StartTimer();
			}
			break;
		default:
			break;
	}

	switch(sArchTp.state)
	{
		case CANTP_ST_IDLE:
			break;
		case CANTP_ST_START_TO_SEND:
		case CANTP_ST_SENDING:
		case CANTP_ST_WAIT_FC:
		case CANTP_ST_WAIT_CF:
		case CANTP_ST_SEND_CF:
		case CANTP_ST_SEND_FC:
			if(IsTimerElapsed(5000))
			{
				sArchTp.state = CANTP_ST_IDLE;
				StopTimer();
			}
			break;
		default:
			break;
	}
}
