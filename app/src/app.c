#include "app.h"

void StartupHook( void )
{
	printf("in %s().\n",__FUNCTION__);
#if defined(WIN32)
	Stmo_Init(&Stmo_ConfigData);
#endif    
}

void ShutdownHook( StatusType Error )
{
	printf("in %s().\n",__FUNCTION__);
}
void ErrorHook( StatusType Error )
{
	//printf("in %s().\n",__FUNCTION__);
}
void PreTaskHook( void )
{
	//printf("in %s().\n",__FUNCTION__);
}
void PostTaskHook( void )
{
	//printf("in %s().\n",__FUNCTION__);
}

void Task10ms(void)
{
	Lcd_Test();
	TerminateTask();
}

void Task20ms(void)
{
	app_led_20ms_runnable();
	app_gauge_20ms_runnable();
	TerminateTask();
}
extern void Lcd_Test(void);
void Task100ms(void)
{
	TerminateTask();
}

void Task1000ms(void)
{
	app_led_1000ms_runnable();
	app_time_1000ms_runnable();
	app_nvm_1000ms_runnable();
	TerminateTask();
}

