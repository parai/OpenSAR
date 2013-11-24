#include "Os.h"
#if defined(WIN32)
#define SetEvent SetEvent2
#endif

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
