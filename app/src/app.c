#include "Os.h"
#if defined(WIN32)
#define SetEvent SetEvent2
#endif
void OsIdle(void)
{
	static uint32_t count = 0;
	printf("OsIdle is running.\n");
	for(;;)
	{
		count ++;
		if(0xFFFFFFFF == count)
		{
//			ActivateTask(TASK_ID_eTask2);
//			ActivateTask(TASK_ID_eTask1);
//			ActivateTask(TASK_ID_bTask3);
			count = 0;
		}

	}
}

void bTask3(void)
{
	printf("bTask3 is running.\n");
	TerminateTask();
}
void eTask1(void)
{
	printf("eTask1 is running.\n");
	for(;;)
	{
		WaitEvent(EVENT_MASK_Event1);
		ClearEvent(EVENT_MASK_Event1);
		SetEvent(TASK_ID_eTask2,EVENT_MASK_Event2);
		printf("eTask1 Wait Event Ok.\n");
	}
	TerminateTask();
}
void eTask2(void)
{
	printf("eTask2 is running.\n");
	for(;;)
	{
		WaitEvent(EVENT_MASK_Event2);
		ClearEvent(EVENT_MASK_Event2);
		printf("eTask2 Wait Event Ok.\n");
	}
	TerminateTask();
}
