

#include "Os.h"
#include "Can.h"
#include "CanIf.h"
#include "CanTp.h"
#include "Dcm.h"
#include "Fls.h"
#include "Fee.h"
#include <stdio.h>


void StartupHook(void)
{
	// Init
	Can_Init(&Can_ConfigData);
	CanIf_Init(&CanIf_Config);
	CanTp_Init();
	Dcm_Init();
	Fls_Init(&FlsConfigSet);
	Fee_Init();

	CanIf_SetControllerMode(CANIF_CHL_LS,CANIF_CS_STARTED);
	(void)SetRelAlarm(ALARM_ID_task1ms,Ms2AlarmTick(10),Ms2AlarmTick(1));
	(void)SetRelAlarm(ALARM_ID_task5ms,Ms2AlarmTick(10),Ms2AlarmTick(5));
	(void)SetRelAlarm(ALARM_ID_task10ms,Ms2AlarmTick(10),Ms2AlarmTick(10));
	(void)SetRelAlarm(ALARM_ID_task50ms,Ms2AlarmTick(10),Ms2AlarmTick(50));
	(void)SetRelAlarm(ALARM_ID_task100ms,Ms2AlarmTick(10),Ms2AlarmTick(100));
	(void)SetRelAlarm(ALARM_ID_task500ms,Ms2AlarmTick(10),Ms2AlarmTick(500));
	(void)SetRelAlarm(ALARM_ID_task1000ms,Ms2AlarmTick(10),Ms2AlarmTick(1000));
}

TASK(OsIdle)
{
	
}

TASK(task1ms)
{
}

TASK(task5ms)
{
	Fls_MainFunction();
	Fee_MainFunction();
}

TASK(task10ms)
{
	//printf("task10ms is running.\n");
	CanTp_MainFunction();
	Dcm_MainFunction();
}

TASK(task50ms)
{
	//printf("task50ms is running.\n");
}

TASK(task100ms)
{
	//printf("task100ms is running.\n");
}

TASK(task500ms)
{
	//printf("task500ms is running.\n");
}

TASK(task1000ms)
{
	//printf("task1000ms is running.\n");
}
