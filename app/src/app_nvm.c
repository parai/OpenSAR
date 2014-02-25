#include "app.h"

static void Fee_Test(void)
{
	static App_TimeType SystemTime;
	static uint8 stage = 0;
	static uint32 Flag;
	static uint32 Flag_Read;
	static App_TimeType SystemTime_Read;

	app_get_system(&SystemTime);
	if(0 == stage)
	{
		Flag = 0xABCD55FF;
		Fee_Write(FEE_BLOCK_NUM_Config,&Flag);
	}
	else if(1 == stage)
	{
		printf("Year=%d,Month=%d,Day=%d,Hour=%d,Minute=%d,Second=%d\n",
				SystemTime_Read.year,SystemTime_Read.month,SystemTime_Read.day,
				SystemTime_Read.hour,SystemTime_Read.minute,SystemTime_Read.second);
		Fee_Read(FEE_BLOCK_NUM_Config,0,&Flag_Read,4);
	}
	else if(2 == stage)
	{
		Flag = 0xDEADBEEF;
		Fee_Write(FEE_BLOCK_NUM_FingerPrint,&Flag);
	}
	else if(3 == stage)
	{
		printf("Flag=0x%X\n",Flag_Read);
		Fee_Read(FEE_BLOCK_NUM_FingerPrint,0,&Flag_Read,4);
	}
	else if(4 == stage)
	{
		Fee_Write(FEE_BLOCK_NUM_Time,&SystemTime);
	}
	else if(5 == stage)
	{
		printf("Flag=0x%X\n",Flag_Read);
		Fee_Read(FEE_BLOCK_NUM_Time,0,&SystemTime_Read,7);
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
