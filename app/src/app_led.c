#include "app.h"


static void RefreshByComSignal(void)
{
	uint8_t LedSts;
	// Update Led1
	Com_ReceiveSignal(COM_SID_Led1Sts,&LedSts);
	if(0u == LedSts)
	{
		Dio_WriteChannel(LED1,LED_ON); // On
	}
	else
	{
		Dio_WriteChannel(LED1,LED_OFF); // Off
	}
	// Update Led2
	Com_ReceiveSignal(COM_SID_Led2Sts,&LedSts);
	if(0u == LedSts)
	{
		Dio_WriteChannel(LED2,LED_ON); // On
	}
	else
	{
		Dio_WriteChannel(LED2,LED_OFF); // Off
	}
	// Update Led3
	Com_ReceiveSignal(COM_SID_Led3Sts,&LedSts);
	if(0u == LedSts)
	{
		Dio_WriteChannel(LED3,LED_ON); // On
	}
	else
	{
		Dio_WriteChannel(LED3,LED_OFF); // Off
	}
}

static void RefresByDio(void)
{
	Dio_LevelType IgSts;
	IgSts=Dio_ReadChannel(IG_INPUT);
	if(STD_LOW == IgSts)
	{	// Ig On
		Dio_WriteChannel(LED_IG_STATE,LED_ON);
	}
	else
	{
		Dio_WriteChannel(LED_IG_STATE,LED_OFF);
	}
}
void app_led_1000ms_runnable(void)
{
	static Dio_LevelType CpuSts = LED_ON;
	Dio_WriteChannel(LED_CPU,CpuSts);
	CpuSts = (CpuSts==LED_ON)?LED_OFF:LED_ON;
}
void app_led_20ms_runnable(void)
{
	RefreshByComSignal();
	RefresByDio();
}


