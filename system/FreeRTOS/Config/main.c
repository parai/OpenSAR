#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Os.h"

#ifdef WIN32
#define SetEvent SetEvent2
#endif

/*-----------------------------------------------------------*/
void ShutdownHook( StatusType Error )
{
}
void StartupHook( void )
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

TASK(OsIdle)
{
	for(;;)
	{
	}
}

TASK(Task10ms)
{
	printf("Task10ms is running\n");
	CancelAlarm(ALARM_ID_Alarm10ms);
	TerminateTask();
}

TASK(Task20ms)
{
	printf("Task20ms is running\n");
	CancelAlarm(ALARM_ID_Alarm20ms);
	TerminateTask();
}

TASK(Task100ms)
{
	printf("Task100ms is running\n");
	CancelAlarm(ALARM_ID_Alarm100ms);
	TerminateTask();
}

TASK(Task1000ms)
{
	printf("Task1000ms is running\n");
	TerminateTask();
}

TASK(SchM_Startup)
{

	printf("SchM_Startup is running\n");
	TerminateTask();
}

TASK(SchM_BswService)
{
	TerminateTask();
}
/*-----------------------------------------------------------*/

int main( int argc, char* argv[] )
{
	StartOS(OSDEFAULTAPPMODE);
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
	printf("Line %d: %s\n",(int)ulLine,pcFileName);
	for(;;);
}

unsigned long ulGetRunTimeCounterValue( void )
{
	return 0;
}

void vConfigureTimerForRunTimeStats( void )
{

}
