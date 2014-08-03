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
#include "OsekOs.h"

TASK(OsIdle)
{

}

ALARM(SchM_BswService)
{
	(void)Os.ActivateTask(TASKID_SchM_BswService);
}
DeclareStack(OsIdle,			1024);
DeclareStack(SchM_Startup,		1024);
DeclareStack(SchM_BswService,	1024);
PROTECT CONST task_declare_t TaskList[TASK_NUM] =
{
	DeclareTask(OsIdle,			TRUE,	OSDEFAULTAPPMODE),
	DeclareTask(SchM_Startup,	TRUE,	OSDEFAULTAPPMODE),
	DeclareTask(SchM_BswService,FALSE,	0				)
};


PROTECT CONST alarm_declare_t	AlarmList[ALARM_NUM] =
{
		DeclareAlarm(SchM_BswService)
};
