#include "app.h"

void StartupHook( void )
{
#if defined(WIN32)
	Stmo_Init(&Stmo_ConfigData);
#endif    
}

void ShutdownHook( StatusType Error )
{

}
void ErrorHook( StatusType Error )
{

}
void PreTaskHook( void )
{
}
void PostTaskHook( void )
{

}

void Task10ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask10ms);

		(void)ClearEvent(EVENT_MASK_EventTask10ms);
	}
	TerminateTask();
}

void Task20ms(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_EventTask20ms);
		app_led_20ms_runnable();
		app_gauge_20ms_runnable();
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
		app_nvm_100ms_runnable();
		(void)ClearEvent(EVENT_MASK_EventTask100ms);
	}
	TerminateTask();
}

void TaskEvent(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_Event1000ms);
		app_led_1000ms_runnable();
		app_time_1000ms_runnable();
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}

