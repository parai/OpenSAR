#include "app.h"

void StartupHook( void )
{
	printf("in %s().\n",__FUNCTION__);
#if defined(WIN32)
	Stmo_Init(&Stmo_ConfigData);
#endif    
#if defined(USE_GUI)
	LCDD_On();
	Sg_Init(&GuiConfigData);
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
//	static uint32 degree = 0;
//	degree += 2;
//	Sg_SetWidgetDegree(GUI_W_SPEED_B,degree);
//	Sg_SetWidgetDegree(GUI_W_TACHO_B,degree);
	Stmo_MainFunction();
	TerminateTask();
}

void Task20ms(void)
{
	app_led_20ms_runnable();
	app_gauge_20ms_runnable();
	TerminateTask();
}

void Task100ms(void)
{
#if defined(USE_GUI)
	Sg_MainFunction();
#endif
	TerminateTask();
}

void Task1000ms(void)
{
	app_led_1000ms_runnable();
	app_time_1000ms_runnable();
	app_nvm_1000ms_runnable();

	TerminateTask();
}

