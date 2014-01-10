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
		(void)ClearEvent(EVENT_MASK_EventTask100ms);
	}
	TerminateTask();
}
static void OpenSAR_Time(void)
{
	static uint16 year = 2013;
	static uint8  month= 12;
	static uint8  day  = 15;
	static uint8  hour = 19;
	static uint8  minute= 49;
	static uint8  second= 00;
	second ++;
	if(second > 59)
	{
		second = 0;
		minute ++;
		if(minute > 59)
		{
			minute = 0;
			hour ++;
			if(hour > 23)
			{
				hour = 0;
				day++;
				if(day > 30)	//be easy, I just give a example
				{
					day = 0;
					month ++;
					if(month > 12)
					{
						month = 0;
						year ++;
					}
				}
			}
		}
	}
	Com_SendSignal(COM_SID_Year,&year);
	Com_SendSignal(COM_SID_Month,&month);
	Com_SendSignal(COM_SID_Day,&day);
	Com_SendSignal(COM_SID_Hour,&hour);
	Com_SendSignal(COM_SID_Minute,&minute);
	Com_SendSignal(COM_SID_Second,&second);
}
void TaskEvent(void)
{
	for(;;)
	{
		(void)WaitEvent(EVENT_MASK_Event1000ms);
		app_led_1000ms_runnable();
		OpenSAR_Time();
		(void)ClearEvent(EVENT_MASK_Event1000ms);
	}
	TerminateTask();
}

