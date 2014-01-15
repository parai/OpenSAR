#include "app.h"

static void Fee_Test(void)
{
	static App_TimeType SystemTime =
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

	static uint8 stage = 0;
	static uint32 Flag;
	static uint32 Flag_Read;
	static App_TimeType SystemTime_Read;
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

static void NvM_Test(void)
{
//	static uint32 Flag_Read[8];
//	NvM_ReadBlock(NVM_BLOCK_ID_TEST1,Flag_Read);
//	printf("NvM: Read = 0x%x\n",Flag_Read[0]);
}

void app_nvm_1000ms_runnable(void)
{
	Fee_Test();
	NvM_Test();
}
