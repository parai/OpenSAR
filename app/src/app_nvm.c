#include "app.h"

#if 0
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
//		printf("Year=%d,Month=%d,Day=%d,Hour=%d,Minute=%d,Second=%d\n",
//				SystemTime_Read.year,SystemTime_Read.month,SystemTime_Read.day,
//				SystemTime_Read.hour,SystemTime_Read.minute,SystemTime_Read.second);
		Fee_Read(FEE_BLOCK_NUM_Config,0,&Flag_Read,4);
	}
	else if(2 == stage)
	{
		Flag = 0xDEADBEEF;
		Fee_Write(FEE_BLOCK_NUM_FingerPrint,&Flag);
	}
	else if(3 == stage)
	{
		//printf("Flag=0x%X\n",Flag_Read);
		Fee_Read(FEE_BLOCK_NUM_FingerPrint,0,&Flag_Read,4);
	}
	else if(4 == stage)
	{
		Fee_Write(FEE_BLOCK_NUM_Time,&SystemTime);
	}
	else if(5 == stage)
	{
		//printf("Flag=0x%X\n",Flag_Read);
		Fee_Read(FEE_BLOCK_NUM_Time,0,&SystemTime_Read,7);
	}
	stage ++;
	if(stage > 5)
	{
		stage = 0;
	}
}
#endif

static void NvM_Test(void)
{
	App_TimeType SystemTime;

	app_get_system(&SystemTime);
//	NvM_ReadBlock(NVM_BLOCK_ID_Time,(uint8*)&NvM_BlockTime_DataGroup_RAM);
//	printf("NvM>>> Year=%d,Month=%d,Day=%d,Hour=%d,Minute=%d,Second=%d\n",
//			NvM_BlockTime_DataGroup_RAM.Year,NvM_BlockTime_DataGroup_RAM.Month,NvM_BlockTime_DataGroup_RAM.Day,
//			NvM_BlockTime_DataGroup_RAM.Hour,NvM_BlockTime_DataGroup_RAM.Minute,NvM_BlockTime_DataGroup_RAM.Second);
	printf("NvM>>> Year=%d,Month=%d,Day=%d,Hour=%d,Minute=%d,Second=%d\n",
			Rte_NvMRead(Time,Year),Rte_NvMRead(Time,Month),Rte_NvMRead(Time,Day),
			Rte_NvMRead(Time,Hour),Rte_NvMRead(Time,Minute),Rte_NvMRead(Time,Second));

	Rte_NvMWrite(Time,Year,SystemTime.year);
	Rte_NvMWrite(Time,Month,SystemTime.month);
	Rte_NvMWrite(Time,Day,SystemTime.day);
	Rte_NvMWrite(Time,Hour,SystemTime.hour);
	Rte_NvMWrite(Time,Minute,SystemTime.minute);
	Rte_NvMWrite(Time,Second,SystemTime.second);

	if(SystemTime.second%2 != 0)
	{
		Rte_NvmWriteBlock(Time);
	}
	else
	{
		memcpy(Rte_NvMReadBuffer(FingerPrint),"Test by parai@foxmail.com\n",26);
		Rte_NvmWriteBlock(FingerPrint);
	}
}

void app_nvm_1000ms_runnable(void)
{
	//Fee_Test();
	NvM_Test();
}
