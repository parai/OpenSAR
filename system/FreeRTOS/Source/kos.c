/*
 * kos.c
 *
 *  Created on: 2014-6-2
 *      Author: parai
 *      Email:  parai@foxmail.com
 */
/* ===================================== [ INCLUDE ] =========================================== */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "timers.h"
#include "Os.h"
#include "task_i.h"
#include "alarm_i.h"

/* ===================================== [ MACROS ] ============================================ */
#ifdef WIN32
#define SetEvent SetEvent2
#endif

/* ===================================== [ DATAS  ] ============================================ */
extern TaskHandle_t volatile pxCurrentTCB;

static uint8_t volatile   IsrNestCounter = 0;
static TaskHandle_t       TCBList[OS_TASK_CNT];
static EventGroupHandle_t ECBList[OS_TASK_CNT];
static TimerHandle_t      ACBList[OS_ALARM_CNT];


/* ===================================== [ LOCAL FUNCTIONS  ] ================================== */
/* Task Start-up */
static void StartTasks ( void )
{
	const OsTaskConstType *task;

	for ( int i=0;i<OS_TASK_CNT;i++ )
	{
		task = &Os_TaskConstList[i];
		if ( task->autostart )
		{
			ActivateTask( i );
		}
	}
}
/* Event Start-up */
static void StartEvents ( void )
{
	const OsTaskConstType *task;

	for ( int i=0;i<OS_TASK_CNT;i++ )
	{
		task = &Os_TaskConstList[i];
		if ( PROC_EXTENDED == task->proc_type )
		{
			ECBList[i] = xEventGroupCreate();
			configASSERT(ECBList[i] != NULL);
		}
		else
		{
			ECBList[i] = NULL;	/* no events */
		}
	}
}
/* Alarm Start-up */
static void StartAlarms ( void )
{
	const OsAlarmType *alarm;

	for ( int i=0;i<OS_ALARM_CNT;i++)
	{
		alarm = &alarm_list[i];

		if ( alarm->autostartPtr != NULL)
		{
			if ( ALARM_AUTOSTART_RELATIVE == alarm->autostartPtr->autostartType )
			{
				SetRelAlarm(i, alarm->autostartPtr->alarmTime, alarm->autostartPtr->cycleTime);
			}
			else
			{
				SetAbsAlarm(i, alarm->autostartPtr->alarmTime, alarm->autostartPtr->cycleTime);
			}
		}
	}

}

static void ProcessExpiredAlarms( TimerHandle_t xTimer )
{
	const OsAlarmType *alarm;
	alarm = (const OsAlarmType *)pvTimerGetTimerID(xTimer);

	switch ( alarm->action.type )
	{
		case ALARM_ACTION_ACTIVATETASK:
			(void) ActivateTask(alarm->action.task_id);
			break;
		case ALARM_ACTION_SETEVENT:
			(void) SetEvent(alarm->action.task_id,alarm->action.event_id);
			break;
		case ALARM_ACTION_ALARMCALLBACK:
			/* TODO */
			break;
		default:
			break;
	}
}

/* ===================================== [ FUNCTIONS  ] ======================================== */
void InitOS ( void )
{
	memset(TCBList,0u,sizeof(TCBList));
	memset(ECBList,0u,sizeof(ECBList));
	memset(ACBList,0u,sizeof(ACBList));
}

void Os_IsrInit ( void )
{
	IsrNestCounter = 0;
}

/* ===================================== [ OSEK STANDARD  ] ==================================== */
void StartOS ( AppModeType Mode )
{
	extern struct OsHooks os_conf_global_hooks;
	if ( os_conf_global_hooks.StartupHook!=NULL )
	{
		os_conf_global_hooks.StartupHook();
	}

	InitOS();

	Os_IsrInit();

	StartEvents();
	StartTasks();
	StartAlarms();

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	for ( ;; );
}

void EnterISR (void)
{
	if (IsrNestCounter < 0xFF)
	{
		IsrNestCounter ++;
	}
}

void ExitISR (void)
{
	if (IsrNestCounter > 0)
	{
		IsrNestCounter --;
	}
}

StatusType ActivateTask ( TaskType TaskID )
{
	const OsTaskConstType *task;
	TaskHandle_t* tcb;
	StatusType rv = E_OK;
	if (TaskID < OS_TASK_CNT)
	{
		task = &Os_TaskConstList[TaskID];
		tcb  = &TCBList[TaskID];

		if(NULL == (*tcb) )
		{
#if 1
			xTaskCreate( (TaskFunction_t)task->entry, task->name, task->stack.size/sizeof(StackType_t), NULL, task->prio, tcb );
#else		/* something about stack, NG, as stack free(), see prvCheckTasksWaitingTermination() */
			xTaskGenericCreate( (TaskFunction_t)task->entry, task->name, task->stack.size/sizeof(StackType_t), NULL,
					task->prio, tcb, (StackType_t*)task->stack.top, NULL );
#endif
			if(NULL == (*tcb) )
			{
				/* create action failed */
				rv = E_OS_RESOURCE; /* most possible reason is that resource is not enough */
			}
		}
		else
		{
			rv=E_OS_LIMIT;
		}
	}
	else
	{
		rv = E_OS_ID;
	}

	return rv;
}


StatusType TerminateTask( void )
{
	StatusType rv = E_OK;

	if(NULL == pxCurrentTCB )
	{
		rv = E_OS_CALLEVEL;
	}
	else
	{
		TaskType task_id;
		GetTaskID(&task_id);

		if ( INVALID_TASK == task_id )
		{
			rv = E_OS_CALLEVEL;
		}
		else
		{
			TCBList[task_id] = NULL;
			vTaskDelete( NULL );
		}
	}

	return rv;
}

StatusType 	ChainTask( TaskType TaskID )
{
	StatusType rv = E_OK;

	rv = ActivateTask(TaskID);

	if (E_OK == rv)
	{
		rv = TerminateTask();
	}

	return rv;
}

StatusType GetTaskID( TaskRefType TaskID ) {
	StatusType rv = E_OK;
	*TaskID = INVALID_TASK;

	if ( NULL == pxCurrentTCB )
	{

	}
	else
	{
		for ( int i=0;i<OS_TASK_CNT;i++ )
		{
			if(TCBList[i] == pxCurrentTCB )
			{
				*TaskID = i;
				break;
			}
		}
	}

    return rv;
}


StatusType WaitEvent ( EventMaskType Mask )
{
	StatusType rv = E_OK;
	TaskType task_id;
	GetTaskID(&task_id);
	if (INVALID_TASK != task_id)
	{
		if ( NULL != ECBList[task_id] )
		{
			xEventGroupWaitBits(ECBList[task_id], Mask, pdFALSE, pdFALSE, portMAX_DELAY); /* wait forever until one the event specified by Mask happened */
		}
		else
		{
			rv = E_OS_ACCESS;
		}
	}
	else
	{
		rv = E_OS_CALLEVEL;
	}

	return rv;
}

#if defined(WIN32)
StatusType SetEvent2 ( TaskType TaskID, EventMaskType Mask )
#else
StatusType SetEvent ( TaskType TaskID, EventMaskType Mask )
#endif
{
	StatusType rv = E_OK;
	if (TaskID < OS_TASK_CNT)
	{
		if ( NULL != ECBList[TaskID] )
		{
			if ( 0u == IsrNestCounter )
			{
				xEventGroupSetBits(ECBList[TaskID], Mask);
			}
			else
			{
				BaseType_t xHigherPriorityTaskWoken,xResult;

				xHigherPriorityTaskWoken = pdFALSE;

				xResult = xEventGroupSetBitsFromISR(ECBList[TaskID], Mask, &xHigherPriorityTaskWoken);

				if ( xResult == pdPASS )
				{
					portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
				}
				else
				{
					rv = E_OS_ACCESS;
				}
			}
		}
		else
		{
			rv = E_OS_ACCESS;
		}
	}
	else
	{
		rv = E_OS_ID;
	}

	return rv;
}

StatusType ClearEvent ( EventMaskType Mask)
{
	StatusType rv = E_OK;
	TaskType task_id;
	GetTaskID(&task_id);
	if (INVALID_TASK != task_id)
	{
		if ( NULL != ECBList[task_id] )
		{
			xEventGroupClearBits(ECBList[task_id], Mask);
		}
		else
		{
			rv = E_OS_ACCESS;
		}
	}
	else
	{
		rv = E_OS_CALLEVEL;
	}

	return rv;
}

StatusType GetEvent ( TaskType TaskId, EventMaskRefType Mask)
{
	StatusType rv = E_OK;
	if (TaskId < OS_TASK_CNT)
	{
		if ( NULL != ECBList[TaskId] )
		{
			*Mask = xEventGroupGetBits(ECBList[TaskId]);
		}
		else
		{
			rv = E_OS_ACCESS;
		}
	}
	else
	{
		rv = E_OS_ID;
	}

	return rv;
}

StatusType SetRelAlarm(AlarmType AlarmId, TickType Increment, TickType Cycle)
{
	StatusType rv = E_OK;

	if ( AlarmId < OS_ALARM_CNT )
	{
		const OsAlarmType *alarm;
		alarm = &alarm_list[AlarmId];
		if ( NULL == ACBList[AlarmId] )
		{
			TickType offset = 0;
			if ( 0 == Cycle )
			{
				ACBList[AlarmId] = xTimerCreate(alarm->name,Increment,pdFALSE,(void * const)alarm,ProcessExpiredAlarms);
			}
			else
			{
				ACBList[AlarmId] = xTimerCreate(alarm->name,Cycle,pdTRUE,(void * const)alarm,ProcessExpiredAlarms);
				offset = Increment;
			}

			if ( NULL == ACBList[AlarmId] )
			{
				rv = E_OS_VALUE;
			}
			else
			{
				xTimerStart(ACBList[AlarmId],offset);
			}
		}
		else
		{
			rv = E_OS_STATE;	/* already active */
		}
	}
	else
	{
		rv = E_OS_ID;
	}

	return rv;
}

StatusType SetAbsAlarm(AlarmType AlarmId, TickType Start, TickType Cycle)
{
	return SetRelAlarm(AlarmId,Start,Cycle);
}

StatusType CancelAlarm(AlarmType AlarmId)
{
	StatusType rv = E_OK;
	if ( AlarmId < OS_ALARM_CNT )
	{
		if ( NULL != ACBList[AlarmId] )
		{
			BaseType_t xResult = xTimerDelete(ACBList[AlarmId],0);
			if( pdFAIL == xResult)
			{
				rv = E_OS_ACCESS;
			}
			else
			{
				ACBList[AlarmId] = NULL;
			}
		}
		else
		{
			rv = E_OS_NOFUNC;
		}
	}
	else
	{
		rv = E_OS_ID;
	}
	return rv;
}
#if defined(USE_FREERTOS)
#include "EcuM.h"
int main (int argc,char* argv[])
{
	EcuM_Init();
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
#endif


