#include "Os.h"
#include "Can.h"
#if defined(WIN32)
#define SetEvent SetEvent2
#endif
#include "debug.h"

void Task10ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask10ms);
		//printf("Task10ms is running.\n");
		Can_PduType pdu;
		pdu.id = 0x731;
		pdu.length = 8;
		pdu.sdu = "Task10ms";
		pdu.swPduHandle = 0x55;
		Can_Write(CAN_CTRL_0_HTH,&pdu);
		(void)ClearEvent(EVENT_MASK_EventTask10ms);
	}
	TerminateTask();
}
void Task20ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask20ms);
		//printf("Task20ms is running.\n");
		(void)ClearEvent(EVENT_MASK_EventTask20ms);
	}
	TerminateTask();
}
void Task100ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask100ms);
		//printf("Task100ms is running.\n");
		(void)ClearEvent(EVENT_MASK_EventTask100ms);
	}
	TerminateTask();
}
void TaskEvent(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_Event1000ms);
		// printf("TaskEvent is running.\n");
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}

void CanIf_UserRxIndication(uint8 channel, PduIdType pduId, const uint8 *sduPtr,uint8 dlc, Can_IdType canId)
{
}

