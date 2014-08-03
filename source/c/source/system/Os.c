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
#include "Os.h"
#include "OsekOs.h"
#include "EcuM.h"
#include "sldl.h"

/* ============================= [ FUNCTION ] ================================= */
PUBLIC STATIC void Init ( void )
{
	OsekOs.Init();
}

PUBLIC STATIC void Start ( AppModeType app_mode )
{
	OsekOs.Start(app_mode);
}

PUBLIC STATIC StatusType GetTaskState ( TaskType TaskID,TaskStateRefType State )
{
	return OsekOs.GetTaskState(TaskID,State);
}
PUBLIC STATIC StatusType ActivateTask ( TaskType TaskID )
{
	return OsekOs.ActivateTask(TaskID);
}

PUBLIC STATIC StatusType TerminateTask ( void )
{
	return OsekOs.TerminateTask();
}

PUBLIC STATIC StatusType Schedule(void)
{
	return OsekOs.Schedule();
}

PUBLIC STATIC StatusType GetAlarmBase( AlarmType AlarmId, AlarmBaseRefType Info )
{
	return OsekOs.GetAlarmBase(AlarmId,Info);
}

PUBLIC STATIC StatusType GetAlarm(AlarmType AlarmId, TickRefType Tick)
{
	return OsekOs.GetAlarm(AlarmId,Tick);
}
PUBLIC STATIC StatusType SetRelAlarm(AlarmType AlarmId, TickType Increment, TickType Cycle)
{
	return OsekOs.SetRelAlarm(AlarmId,Increment,Cycle);
}

PUBLIC STATIC StatusType SetAbsAlarm(AlarmType AlarmId, TickType Start, TickType Cycle)
{
	return OsekOs.SetAbsAlarm(AlarmId,Start,Cycle);
}

PUBLIC STATIC StatusType CancelAlarm(AlarmType AlarmId)
{
	return OsekOs.CancelAlarm(AlarmId);
}

PUBLIC STATIC TickType GetOsTick( void )
{
	return OsekOs.GetOsTick();
}

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST Os_Class Os = {
	.Init  = Init,
	.Start = Start,
	.Schedule = Schedule,
	.ActivateTask = ActivateTask,
	.TerminateTask = TerminateTask,
	.GetTaskState = GetTaskState,
	.GetAlarmBase = GetAlarmBase,
	.GetAlarm = GetAlarm,
	.SetRelAlarm = SetRelAlarm,
	.SetAbsAlarm = SetAbsAlarm,
	.CancelAlarm = CancelAlarm,
	.GetOsTick = GetOsTick,
};

