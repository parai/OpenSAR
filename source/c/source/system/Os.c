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

/* ============================= [ FUNCTION ] ================================= */
PUBLIC STATIC void Init ( void )
{
	OsekOs.Init();
}

PUBLIC STATIC void Start ( AppModeType app_mode )
{
	OsekOs.Start(app_mode);
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

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST Os_Class Os = {
	.Init  = Init,
	.Start = Start,
	.Schedule = Schedule,
	.ActivateTask = ActivateTask,
	.TerminateTask = TerminateTask
};

