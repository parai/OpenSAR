
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "Can.h"
#include "CanHw.h"
#include "arvfb.h"

#define GTK_CAN_CMD_TX   ((guchar)0x00)
#define GTK_CAN_CMD_RX   ((guchar)0x01)

#define GTK_CAN_CMD_TX_ACK ((guchar)0x80)
#define GTK_CAN_CMD_RX_ACK ((guchar)0x81)

CAN_HW_t Can_HwUnit[CAN_CONTROLLER_CNT];
extern void arch_generate_irqn(IrqType IRQn);

extern CanIf_ControllerModeType Can_GetUnitState(uint8 ctrl);

static void CanSocket_Rx(const ArCanMsgType *ArMsgIn)
{
	CAN_HW_t *canHw;
	uint8 ctrl=ArMsgIn->Msg.BusID;
	canHw = &Can_HwUnit[ctrl];

	if( (CANIF_CS_STARTED==Can_GetUnitState(ctrl)) ||
		(CANIF_CS_SLEEP==Can_GetUnitState(ctrl)))
	{
		if( BM_WUPI == (canHw->RIER & BM_WUPI))
		{
			canHw->IRQF |= cCanIsrWak;
			printf("Can Wakeup!\n");
		}
		else
		{
			GtkCanMsgBox_Type* pMsgBox = GtkCanGetEmptyMsgBox(&(canHw->rxQ));
			//Can_SocketEnterCritical(ctrl);
			// check for any free box
			if(NULL != pMsgBox) {
				pMsgBox->id = ArMsgIn->Msg.Identifier;
				pMsgBox->dlc = ArMsgIn->Msg.DataLengthCode;
				if(pMsgBox->dlc>8)
				{
					pMsgBox->dlc = 8;
				}
				memcpy(pMsgBox->data,ArMsgIn->Msg.Data,8);
			}
			else
			{
				printf("Can Message Box Full, Message Lost!\n");
			}
			canHw->IRQF |= cCanIsrRx;
			//Can_SocketExitCritical(ctrl);
		}
		arch_generate_irqn(SysCan_0_IRQn+ctrl);
	}
}
static void CanSocket_Tx(ArCanMsgType *ArMsgOut)
{
	CAN_HW_t *canHw;
	uint8 ctrl;

	for(ctrl=CAN_CTRL_0;ctrl<CAN_CONTROLLER_CNT;ctrl++)
	{
		canHw = &Can_HwUnit[ctrl];
		GtkCanMsgBox_Type* pMsgBox = GtkCanGetBusyMsgBox(&(canHw->txQ));
		if(NULL != pMsgBox)
		{
			int i;
			ArMsgOut->Type    = MSG_ON_CAN;
			ArMsgOut->Command = MSG_CMD_POLL | MSG_CMD_ACK;
			ArMsgOut->Length  = MSG_CAN_LENGTH;

			ArMsgOut->Msg.Identifier = pMsgBox->id;
			ArMsgOut->Msg.DataLengthCode = pMsgBox->dlc;
			if(ArMsgOut->Msg.DataLengthCode > 8)
			{
				ArMsgOut->Msg.DataLengthCode = 8;
			}
			for(i=0;i<ArMsgOut->Msg.DataLengthCode;i++)
			{
				ArMsgOut->Msg.Data[i] = pMsgBox->data[i];
			}
			for(;i<8;i++)
			{
				ArMsgOut->Msg.Data[i]  = 0x55;  // PADDING
			}
//			{
//				g_print("CANID=0x%-3x,DLC=%x, [",gtkCanMsg.id,(guint)gtkCanMsg.dlc);
//				for(int i=0;i<8;i++)
//				{
//					g_print("%-2x,",(guint)gtkCanMsg.data[i]);
//				}
//				g_print("]\n");
//			}
			ArMsgOut->Msg.BusID = ctrl;

			// Clear TIER
			canHw->TIER &= ~(BM_TX0);
			canHw->IRQF |= cCanIsrTx;

			Can_SetPduHandle(ctrl,pMsgBox->swPduHandle);
			// Request System Isr
			arch_generate_irqn(SysCan_0_IRQn+ctrl);
			break;
		}
	}

	if(CAN_CONTROLLER_CNT==ctrl)
	{ // No ready CAN message
		ArMsgOut->Type    = MSG_ON_CAN;
		ArMsgOut->Command = MSG_CMD_POLL | MSG_CMD_ACK;
		ArMsgOut->Length  = 0;  // no data ready
	}
}

// Exported Functions

boolean ArvfbCanProcess(const ArCanMsgType *ArMsgIn,ArCanMsgType *ArMsgOut)
{
	boolean Answer = FALSE;

	if(MSG_CMD_POLL == ArMsgIn->Command)
	{
		CanSocket_Tx(ArMsgOut);
		Answer = TRUE;
	}
	else if(MSG_CMD_SEND == ArMsgIn->Command)
	{
		CanSocket_Rx(ArMsgIn);
	}

	return Answer;
}
void Can_0_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_0];
	//Can_SocketEnterCritical(CAN_CTRL_0);
	if(cCanIsrTx == (canHw->IRQF&cCanIsrTx))
	{
		Can_0_TxIsr();
	}
	if(cCanIsrRx == (canHw->IRQF&cCanIsrRx))
	{
		Can_0_RxIsr();
	}
	if(cCanIsrWak == (canHw->IRQF&cCanIsrWak))
	{
		Can_0_WakeIsr();
	}
	if(cCanIsrErr == (canHw->IRQF&cCanIsrErr))
	{
		Can_0_ErrIsr();
	}
	canHw->IRQF = 0; // clear all
	//Can_SocketExitCritical(CAN_CTRL_0);
}

void Can_1_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_1];
	//Can_SocketEnterCritical(CAN_CTRL_1);
	if(cCanIsrTx == (canHw->IRQF&cCanIsrTx))
	{
		Can_1_TxIsr();
	}
	if(cCanIsrRx == (canHw->IRQF&cCanIsrRx))
	{
		Can_1_RxIsr();
	}
	if(cCanIsrWak == (canHw->IRQF&cCanIsrWak))
	{
		Can_1_WakeIsr();
	}
	if(cCanIsrErr == (canHw->IRQF&cCanIsrErr))
	{
		Can_1_ErrIsr();
	}
	canHw->IRQF = 0; // clear all
	//Can_SocketExitCritical(CAN_CTRL_1);
}

void Can_2_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_2];
	//Can_SocketEnterCritical(CAN_CTRL_2);
	if(cCanIsrTx == (canHw->IRQF&cCanIsrTx))
	{
		Can_2_TxIsr();
	}
	if(cCanIsrRx == (canHw->IRQF&cCanIsrRx))
	{
		Can_2_RxIsr();
	}
	if(cCanIsrWak == (canHw->IRQF&cCanIsrWak))
	{
		Can_2_WakeIsr();
	}
	if(cCanIsrErr == (canHw->IRQF&cCanIsrErr))
	{
		Can_2_ErrIsr();
	}
	canHw->IRQF = 0; // clear all
	//Can_SocketExitCritical(CAN_CTRL_2);
}

void Can_3_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_3];
	//Can_SocketEnterCritical(CAN_CTRL_3);
	if(cCanIsrTx == (canHw->IRQF&cCanIsrTx))
	{
		Can_3_TxIsr();
	}
	if(cCanIsrRx == (canHw->IRQF&cCanIsrRx))
	{
		Can_3_RxIsr();
	}
	if(cCanIsrWak == (canHw->IRQF&cCanIsrWak))
	{
		Can_3_WakeIsr();
	}
	if(cCanIsrErr == (canHw->IRQF&cCanIsrErr))
	{
		Can_3_ErrIsr();
	}
	canHw->IRQF = 0; // clear all
	//Can_SocketExitCritical(CAN_CTRL_3);
}

void Can_4_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_4];
	//Can_SocketEnterCritical(CAN_CTRL_4);
	if(cCanIsrTx == (canHw->IRQF&cCanIsrTx))
	{
		Can_4_TxIsr();
	}
	if(cCanIsrRx == (canHw->IRQF&cCanIsrRx))
	{
		Can_4_RxIsr();
	}
	if(cCanIsrWak == (canHw->IRQF&cCanIsrWak))
	{
		Can_4_WakeIsr();
	}
	if(cCanIsrErr == (canHw->IRQF&cCanIsrErr))
	{
		Can_4_ErrIsr();
	}
	canHw->IRQF = 0; // clear all
	//Can_SocketExitCritical(CAN_CTRL_4);
}

