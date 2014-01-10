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
typedef struct
{
	uint16 year;
	uint8  month;
	uint8  day;
	uint8  hour;
	uint8  minute;
	uint8  second;
	uint8  reserved[32];
}tSystemTime;
static void OpenSAR_Time(void)
{
	static tSystemTime SystemTime =
	{
		.year   = 2013,
		.month  = 12,
		.day    = 15,
		.hour   = 19,
		.minute = 49,
		.second = 0
	};
	SystemTime.second ++;
	if(SystemTime.second > 59)
	{
		SystemTime.second = 0;
		SystemTime.minute ++;
		if(SystemTime.minute > 59)
		{
			SystemTime.minute = 0;
			SystemTime.hour ++;
			if(SystemTime.hour > 23)
			{
				SystemTime.hour = 0;
				SystemTime.day++;
				if(SystemTime.day > 30)	//be easy, I just give a example
				{
					SystemTime.day = 0;
					SystemTime.month ++;
					if(SystemTime.month > 12)
					{
						SystemTime.month = 0;
						SystemTime.year ++;
					}
				}
			}
		}
	}
	Com_SendSignal(COM_SID_Year,&(SystemTime.year));
	Com_SendSignal(COM_SID_Month,&(SystemTime.month));
	Com_SendSignal(COM_SID_Day,&(SystemTime.day));
	Com_SendSignal(COM_SID_Hour,&(SystemTime.hour));
	Com_SendSignal(COM_SID_Minute,&(SystemTime.minute));
	Com_SendSignal(COM_SID_Second,&(SystemTime.second));

	static uint8 stage = 0;
	static uint32 Flag;
	static uint32 Flag_Read;
	static tSystemTime SystemTime_Read;
	if(0 == stage)
	{
		printf("Time = %d-%-2d-%-2d:%-2d-%-2d-%-2d\n",SystemTime_Read.year,SystemTime_Read.month,SystemTime_Read.day,
			SystemTime_Read.hour,SystemTime_Read.minute,SystemTime_Read.second);
		Flag = 0xABCD55FF;
		Fee_Write(FEE_BLOCK_NUM_TEST1,&Flag);
	}
	else if(1 == stage)
	{
		Fee_Read(FEE_BLOCK_NUM_TEST1,0,&Flag_Read,4);
	}
	else if(2 == stage)
	{
		printf("1>>>> 0x%x\n",Flag_Read);
		Flag = 0xDEADBEEF;
		Fee_Write(FEE_BLOCK_NUM_TEST2,&Flag);
	}
	else if(3 == stage)
	{
		Fee_Read(FEE_BLOCK_NUM_TEST2,0,&Flag_Read,4);
	}
	else if(4 == stage)
	{
		printf("2>>>> 0x%x\n",Flag_Read);
		Fee_Write(FEE_BLOCK_NUM_SYSTEM_TIME,&SystemTime);
	}
	else if(5 == stage)
	{
		Fee_Read(FEE_BLOCK_NUM_SYSTEM_TIME,0,&SystemTime_Read,7);
	}
	stage ++;
	if(stage > 5)
	{
		stage = 0;
	}
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

