#include "app.h"

void Task10ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask10ms);
		CanTp_MainFunction();
		Dcm_MainFunction();
		Com_MainFunctionRx();
		Com_MainFunctionTx();
		(void)ClearEvent(EVENT_MASK_EventTask10ms);
	}
	TerminateTask();
}
#define LED1 PortA_PIN3
#define LED2 PortA_PIN4
#define LED3 PortA_PIN5

#define LDE_ON   STD_LOW
#define LDE_OFF  STD_HIGH
void Task20ms(void)
{
	Dio_LevelType ChannelLevel = STD_LOW;
	uint8_t Led1Sts;
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask20ms);
		Com_ReceiveSignal(COM_SID_Led1Sts,&Led1Sts);
		if(0u == Led1Sts)
		{
			Dio_WriteChannel(LED1,LDE_ON); // On
		}
		else
		{
			Dio_WriteChannel(LED1,LDE_OFF); // Off
		}
		ChannelLevel=Dio_ReadChannel(PortA_PIN1);
		Dio_WriteChannel(PortA_PIN2,ChannelLevel);
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
	Dio_LevelType ChannelLevel = STD_LOW;
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_Event1000ms);
		Dio_WriteChannel(PortA_PIN0,ChannelLevel);
		ChannelLevel = (STD_LOW==ChannelLevel)?STD_HIGH:STD_LOW;
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}

