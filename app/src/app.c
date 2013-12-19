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
void Task20ms(void)
{
	Dio_LevelType ChannelLevel = STD_LOW;
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask20ms);
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
		static uint8 minute = 0;
		minute ++;
		Com_SendSignal(COM_SID_Minute,&minute);
		(void)WaitEvent(EVENT_MASK_Event1000ms);
		Dio_WriteChannel(PortA_PIN0,ChannelLevel);
		ChannelLevel = (STD_LOW==ChannelLevel)?STD_HIGH:STD_LOW;
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}

