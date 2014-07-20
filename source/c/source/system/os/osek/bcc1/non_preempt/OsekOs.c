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

/* ============================= [ DATAS ] ==================================== */
IMPORT PROTECT task_declare_t	TaskList[];
PRIVATE TaskType    			CurrentTask;
PRIVATE AppModeType				AppMode;
/* ============================= [ FUNCTION ] ================================= */
PUBLIC STATIC void Init ( void )
{
	Bitop.Init();
	CurrentTask = 0;	/* 0 means OsIdle task is running */
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
	task_declare_t* declare;
	StatusType ercd = E_OK;

	task = Bitop.GetBit();
	assert(task < TASK_NUM);
	declare = &TaskList[task];

	if (task > CurrentTask)
	{	/* task with high priority */
		TaskType previous = CurrentTask;	/* link it in the dynamic ram queue */

		CurrentTask = task;				/* preempt */

		Bitop.ClearBit(CurrentTask);	/* pop up the higher ready task from ready map */
		declare->entry();

		CurrentTask = previous;
	}
	else
	{
		if(0 == CurrentTask)
		{	/* no task is ready */
			declare->entry();
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
			task_declare_t* declare = &TaskList[task];
			assert(task == declare->priority);
			if ( (TRUE == declare->autostart) &&
				 (0u   != (declare->app_mode&AppMode)) )
			{
				Bitop.SetBit(task);
			}
		}
	}
	for (;;)
	{
		(void)Schedule();
	}
}

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST OsekOs_Class OsekOs = {
	.Init  = Init,
	.Start = Start,
	.Schedule = Schedule,
	.ActivateTask = ActivateTask,
	.TerminateTask = TerminateTask
};

