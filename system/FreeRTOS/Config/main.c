#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
#define TASK1_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define TASK2_PRIORITY		( tskIDLE_PRIORITY + 1 )


/*-----------------------------------------------------------*/

static void Task1(void* in)
{
	for(;;)
	{
		vTaskDelay(1000);
		printf("Task1 is running\n");
	}
}

static void Task2(void* in)
{
	for(;;)
	{
		vTaskDelay(1000);
		printf("Task2 is running\n");
	}
}

/*-----------------------------------------------------------*/

int main( int argc, char* argv[] )
{
	xTaskCreate( Task1, "Task1", 512, ( void * ) NULL, TASK1_PRIORITY, NULL );
	xTaskCreate( Task2, "Task2", 512, ( void * ) NULL, TASK2_PRIORITY, NULL );

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}


void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{

}
unsigned long ulGetRunTimeCounterValue( void )
{
	return 0;
}

void vConfigureTimerForRunTimeStats( void )
{

}
