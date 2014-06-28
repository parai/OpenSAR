
#include <assert.h>
#include <stdio.h>
#include "Can.h"
#include "CanSocket.h"

CAN_HW_t Can_HwUnit[CAN_ARC_CTRL_CONFIG_CNT];

// Exported Functions
void Can_0_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_0];
	Can_SocketEnterCritical(CAN_CTRL_0);
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
	Can_SocketExitCritical(CAN_CTRL_0);
}

void Can_1_IsrEntry(void)
{

}

void Can_2_IsrEntry(void)
{

}

void Can_3_IsrEntry(void)
{

}

void Can_4_IsrEntry(void)
{

}

void Can_SocketDeInit(uint8_t ctrl)
{

}
void Can_SocketInit(uint8_t ctrl)
{

}

void Can_SocketEnterCritical(uint8_t ctrl)
{

}
void Can_SocketExitCritical(uint8_t ctrl)
{

}

void Can_SocketTriggerTx(uint8_t ctrl)
{

}

void Can_SocketSetMode(uint8_t controller,Can_StateTransitionType transition)
{
	switch(transition )
	{
		case CAN_T_START:

		break;
		case CAN_T_WAKEUP:

		break;
		case CAN_T_SLEEP:  //CAN258, CAN290

		break;
		case CAN_T_STOP:

		break;
		default:
		break;
	}
}

