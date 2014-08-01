/**
 * MicroSAR - the open source AUTOSAR platform https://github.com/parai
 *
 * Copyright (C) 2014  MicroSAR <parai@foxmail.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
/* ============================= [ INCLUDE ] ================================== */
#include "OsekOs.h"
#include "bitop.h"
#include "sldl.h"

/* ============================= [ MACROS ] =================================== */

/* ============================= [ DATAS ] ==================================== */
IMPORT PROTECT CONST task_declare_t		TaskList[];
IMPORT PROTECT CONST alarm_declare_t	AlarmList[];

PROTECT TaskType    			CurrentTask;
PRIVATE AppModeType				AppMode;
PRIVATE	TickType				OsTickCounter;

PRIVATE TickType			 	AlarmTick[ALARM_NUM];
PRIVATE TickType			 	AlarmPeriod[ALARM_NUM];


/* ============================= [ FUNCTION ] ================================= */
PUBLIC STATIC void Init ( void )
{
	Bitop.Init();
	CurrentTask = 0;	/* 0 means OsIdle task is running */

	OsTickCounter = 0;
	memset(AlarmTick,0,sizeof(AlarmTick));
	memset(AlarmPeriod,0,sizeof(AlarmPeriod));
}

PUBLIC STATIC StatusType ActivateTask ( TaskType TaskID )
{
	StatusType ercd = E_OK;

	if(TaskID < TASK_NUM)
	{
		Bitop.SetBit(TaskID);
	}
	else
	{
		ercd = E_OS_ID;
	}
	return ercd;
}

PUBLIC STATIC StatusType TerminateTask ( void )
{	/* task self-terminate when it do return */
	return E_OK;
}

PUBLIC STATIC StatusType Schedule(void)
{
	TaskType task;
	CONST task_declare_t* declare;
	StatusType ercd = E_OK;

	task = Bitop.GetBit();
	assert(task < TASK_NUM);
	declare = &TaskList[task];

	if (task > CurrentTask)
	{	/* task with high priority */
		TaskType previous = CurrentTask;	/* link it in the dynamic ram queue */

		CurrentTask = task;				/* preempt */

		Bitop.ClearBit(CurrentTask);	/* pop up the higher ready task from ready map */

		declare->main();

		CurrentTask = previous;
	}
	else
	{
		if(0 == CurrentTask)
		{	/* no task is ready, execute idle*/
			declare->main();
		}
	}

	return ercd;
}

PUBLIC STATIC void Start ( AppModeType app_mode )
{
	AppMode = app_mode;

	{	/* task auto-start up */
		TaskType task;
		for (task=0; task < TASK_NUM; task++)
		{
			CONST task_declare_t* declare = &TaskList[task];
			assert(task == declare->priority);
			if ( (TRUE == declare->autostart) &&
				 (0u   != (declare->app_mode&AppMode)) )
			{
				Bitop.SetBit(task);
			}
		}
	}

	SLDL.Dispatch();
}

PROTECT void OsTick(void)
{
	AlarmType AlarmId;

	OsTickCounter ++;

	for(AlarmId=0; AlarmId<ALARM_NUM; AlarmId++)
	{
		if(AlarmTick[AlarmId] > 0)
		{
			AlarmTick[AlarmId]--;
			if(0u == AlarmTick[AlarmId])
			{
				AlarmList[AlarmId].main();
				AlarmTick[AlarmId] = AlarmPeriod[AlarmId];
			}
		}
	}
}

PUBLIC STATIC StatusType GetAlarmBase( AlarmType AlarmId, AlarmBaseRefType Info )
{
	assert(AlarmId<ALARM_NUM);

	Info[0].MaxAllowedValue = TICK_MAX;
	Info[0].TickPerBase     = 1u;
	Info[0].MinCycle        = 1u;

	return E_OK;
}

PUBLIC STATIC StatusType GetAlarm(AlarmType AlarmId, TickRefType Tick)
{
	assert(AlarmId<ALARM_NUM);

	Tick[0] = AlarmTick[AlarmId];

	return E_OK;
}

PUBLIC STATIC StatusType SetRelAlarm(AlarmType AlarmId, TickType Increment, TickType Cycle)
{
	assert(AlarmId<ALARM_NUM);

	AlarmTick[AlarmId] = Increment;
	AlarmPeriod[AlarmId] = Cycle;

	return E_OK;
}

PUBLIC STATIC StatusType SetAbsAlarm(AlarmType AlarmId, TickType Start, TickType Cycle)
{
	assert(AlarmId<ALARM_NUM);

	if (OsTickCounter < Start)
	{
		Start = Start - OsTickCounter;
	}
	else
	{
		Start = TICK_MAX - OsTickCounter + Start + 1;
	}

	AlarmTick[AlarmId] = Start;
	AlarmPeriod[AlarmId] = Cycle;

	return E_OK;
}

PUBLIC STATIC StatusType CancelAlarm(AlarmType AlarmId)
{
	assert(AlarmId<ALARM_NUM);

	AlarmTick[AlarmId] = 0;
	AlarmPeriod[AlarmId] = 0;

	return E_OK;
}

PUBLIC STATIC TickType GetOsTick( void )
{
	return OsTickCounter;
}

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST OsekOs_Class OsekOs = {
	.Init  = Init,
	.Start = Start,
	.Schedule = Schedule,
	.ActivateTask = ActivateTask,
	.TerminateTask = TerminateTask,
	.GetAlarmBase = GetAlarmBase,
	.GetAlarm = GetAlarm,
	.SetRelAlarm = SetRelAlarm,
	.SetAbsAlarm = SetAbsAlarm,
	.CancelAlarm = CancelAlarm,
	.GetOsTick = GetOsTick,
};

