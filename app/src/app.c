#include "Os.h"
#if defined(WIN32)
#define SetEvent SetEvent2
#endif

void Task10ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask10ms);
		//printf("Task10ms is running.\n");
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
		printf("TaskEvent is running.\n");
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}
