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
#include "Os.h"
#include "task_i.h"

/* ===================================== [ MACROS ] ============================================ */

/* ===================================== [ DATAS  ] ============================================ */
extern TaskHandle_t volatile pxCurrentTCB;

static uint8_t volatile   IsrNestCounter = 0;
static TaskHandle_t       TCBList[OS_TASK_CNT];
static EventGroupHandle_t ECBList[OS_TASK_CNT];


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

/* ===================================== [ FUNCTIONS  ] ======================================== */
void InitOS ( void )
{
	memset(TCBList,0u,sizeof(TCBList));
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
#else		/* something about stack, NG */
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
		TCBList[task_id] = NULL;
		vTaskDelete( pxCurrentTCB );
	}

	return rv;
}

StatusType GetTaskID( TaskRefType TaskID ) {
	StatusType rv = E_OK;
	*TaskID = INVALID_TASK;

	for ( int i=0;i<OS_TASK_CNT;i++ )
	{
		if(TCBList[i] == pxCurrentTCB )
		{
			*TaskID = i;
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
			xEventGroupSetBits(ECBList[TaskID], Mask);
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



