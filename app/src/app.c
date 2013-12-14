#include "Os.h"
#include "CanTp.h"
#include "Dcm.h"
#if defined(WIN32)
#define SetEvent SetEvent2
#endif
#include "debug.h"
#include "Dio.h"

void Task10ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask10ms);
		CanTp_MainFunction();
		Dcm_MainFunction();
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
	Dio_LevelType ChannelLevel = STD_LOW;
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_Event1000ms);
		Dio_WriteChannel(0,ChannelLevel);
		ChannelLevel = (STD_LOW==ChannelLevel)?STD_HIGH:STD_LOW;
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}

