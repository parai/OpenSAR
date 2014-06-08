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
	for(;;)
	{
		vTaskDelay(10);
		printf("Task10ms is running\n");
	}
}

TASK(Task20ms)
{
	for(;;)
	{
		vTaskDelay(20);
		printf("Task20ms is running\n");
	}
}

TASK(Task100ms)
{
	for(;;)
	{
		vTaskDelay(100);
		printf("Task100ms is running\n");
	}
}

TASK(Task1000ms)
{
	for(;;)
	{
		//vTaskDelay(1000);
		WaitEvent(EVENT_MASK_Event1);
		ClearEvent(EVENT_MASK_Event1);
		printf("Task1000ms is running\n");
		TerminateTask();
	}
}

TASK(SchM_Startup)
{
	int i=0;
	//ActivateTask(TASK_ID_Task1000ms);
	for(;;)
	{
		i ++;
		//printf("i=%d\n",i);
		//vTaskDelay(1000);
		//SetEvent(TASK_ID_Task1000ms,EVENT_MASK_Event1);
//		if(i==3)
//		{
//			TerminateTask();
//		}
		ActivateTask(TASK_ID_Task1000ms);
		printf("SchM_Startup is running\n");
		vTaskDelay(1000);
		SetEvent(TASK_ID_Task1000ms,EVENT_MASK_Event1);

	}
}

TASK(SchM_BswService)
{
	for(;;);
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
