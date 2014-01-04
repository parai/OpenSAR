// Cool, Let's copy toppers_osek non-bcc1 strategy for HSI bootloader

#include "Os.h"  
#include <string.h>
#include <assert.h>

typedef void (*task_main_t)(void);

typedef TaskType alarm_main_t; 	// Alarm action is activate Task only.
typedef struct
{
	TickType Left;
	TickType Cycle;
} alarm_cb_t;


#ifndef TICK_MAX
#define TICK_MAX ((TickType)-1)
#endif
// {  Config CHECK Start  =======================================
#if cfgOsMaxTaskId > 32
#error OS NON-BCC:The max task number supported is 32.
#endif
// {  Config CHECK Start  =======================================


// {  Config GEN Start  =======================================
	GEN_TASK_LIST();	
	GEN_ALARM_LIST();
// }  Config GEN End    =======================================

// Note: Each Task should has a unique priority. 
// For easy implementation, let TaskId == TaskPriority
static uint32_t knl_bitmap;  // task ready map for each priority

static alarm_cb_t knl_alarm_cb[cfgOsMaxAlarmId];

static TickType knl_os_tick;

extern void OsIdle(void);

StatusType Schedule(void)
{
	imask_t imask;
	TaskType Id;
	if(0u != knl_bitmap)
	{
		for(Id=cfgOsMaxTaskId; Id>0; Id--)
		{
			if( 0u != (knl_bitmap&(1u<<(Id-1))) )
			{
				Irq_Save(imask);	
				TaskList[Id-1]();
				knl_bitmap &= ~(1u<<(Id-1));
				Irq_Restore(imask);
				break;
			}
		}
	}
	else
	{
		OsIdle();	// only Idle Task can be intrrupted
	}
	return E_OK;
}


void StartOS( AppModeType Mode )
{

	memset(knl_alarm_cb,0u,sizeof(knl_alarm_cb));
	knl_bitmap = 0u;

	StartupHook();

	Irq_Enable();
	
	for(;;)
	{
		(void)Schedule();
	}
}

StatusType 	ActivateTask( TaskType TaskID )
{
	imask_t imask;
	assert(TaskID < cfgOsMaxTaskId);
	Irq_Save(imask);
	knl_bitmap |= (1u<<TaskID); 
	Irq_Restore(imask);
	return E_OK;
}

StatusType GetAlarmBase( AlarmType AlarmId, AlarmBaseRefType Info )
{
	assert(AlarmId<cfgOsMaxAlarmId);

	Info[0].maxallowedvalue = TICK_MAX;
	Info[0].tickperbase     = 1u;
	Info[0].mincycle        = 1u;
	
	return E_OK;
}

StatusType GetAlarm(AlarmType AlarmId, TickRefType Tick)
{
	assert(AlarmId<cfgOsMaxAlarmId);
	
	Tick[0] = knl_alarm_cb[AlarmId].Left;

	return E_OK;
}

StatusType SetRelAlarm(AlarmType AlarmId, TickType Increment, TickType Cycle)
{
	imask_t imask;
	assert(AlarmId<cfgOsMaxAlarmId);
	
	Irq_Save(imask);
	knl_alarm_cb[AlarmId].Left = Increment;
	knl_alarm_cb[AlarmId].Cycle = Cycle;
	Irq_Restore(imask);
	return E_OK;
}

StatusType SetAbsAlarm(AlarmType AlarmId, TickType Start, TickType Cycle)
{
	imask_t imask;
	assert(AlarmId<cfgOsMaxAlarmId);

	if(knl_os_tick < Start)
	{

	}
	else
	{
		Start = TICK_MAX - knl_os_tick + Start + 1;
	}
	Irq_Save(imask);
	knl_alarm_cb[AlarmId].Left = Start;
	knl_alarm_cb[AlarmId].Cycle = Cycle;
	Irq_Restore(imask);
	return E_OK;
}

StatusType CancelAlarm(AlarmType AlarmId)
{
	imask_t imask;
	assert(AlarmId<cfgOsMaxAlarmId);

	Irq_Save(imask);
	knl_alarm_cb[AlarmId].Left = 0u;
	knl_alarm_cb[AlarmId].Cycle = 0u;
	Irq_Restore(imask);

	return E_OK;
}

TickType GetOsTick( void )
{
	return knl_os_tick;
}

void OsTick(void)
{
	imask_t imask;
	AlarmType AlarmId;

	Irq_Save(imask);

	knl_os_tick ++;
	// Process Alarm

	for(AlarmId=0; AlarmId<cfgOsMaxAlarmId; AlarmId++)
	{
		if(knl_alarm_cb[AlarmId].Left > 0)
		{
			knl_alarm_cb[AlarmId].Left--;
			if(0u == knl_alarm_cb[AlarmId].Left)
			{
				(void)ActivateTask(AlarmList[AlarmId]);
				knl_alarm_cb[AlarmId].Left = knl_alarm_cb[AlarmId].Cycle;
			}
		}
	}

	Irq_Restore(imask);
}








