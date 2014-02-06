#include "app.h"

void StartupHook( void )
{
	printf("in %s().\n",__FUNCTION__);
#if defined(WIN32)
	Stmo_Init(&Stmo_ConfigData);
#endif    

	LCDD_On();
	Gui_Init(&GuiConfigData);
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
//	Gui_SetWidgetDegree(GUI_W_SPEED_P,degree);
//	Gui_SetWidgetDegree(GUI_W_TACHO_P,degree);
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
	Gui_MainFunction();
	TerminateTask();
}

void Task1000ms(void)
{
	app_led_1000ms_runnable();
	app_time_1000ms_runnable();
	app_nvm_1000ms_runnable();
	TerminateTask();
}

