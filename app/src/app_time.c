#include "app.h"

static App_TimeType SystemTime =
{
	.year   = 2013,
	.month  = 12,
	.day    = 15,
	.hour   = 19,
	.minute = 49,
	.second = 0
};

void app_get_system(App_TimeType* time)
{
	memcpy(time,&SystemTime,sizeof(App_TimeType));
}
void app_time_1000ms_runnable(void)
{
	static boolean called = False;
	if(!called)
	{
#if 0
		SystemTime.year   = Rte_NvMRead(Time,Year,SystemTime.year);
		SystemTime.month  = Rte_NvMRead(Time,Month,SystemTime.month);
		SystemTime.day    = Rte_NvMRead(Time,Day,SystemTime.day);
		SystemTime.hour   = Rte_NvMRead(Time,Hour,SystemTime.hour);
		SystemTime.minute = Rte_NvMRead(Time,Minute,SystemTime.minute);
		SystemTime.second = Rte_NvMRead(Time,Second,SystemTime.second);
#else
		memcpy(&SystemTime,Rte_NvMReadBuffer(Time),sizeof(App_TimeType));
#endif
	}
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
}
