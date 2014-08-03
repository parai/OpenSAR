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
#ifndef OSEKOS_H_
#define OSEKOS_H_
/* ============================= [ INCLUDE ] ================================== */
#include "Os.h"

/* ============================= [ TYPES ] ==================================== */
typedef void*  stack_t;
typedef uint32 stack_size_t;

typedef struct
{
	task_main_t    	main;
	task_priority_t priority;	/*! priority also represent the task id, the same as TaskType */
	bool            autostart;
	AppModeType     app_mode;	/*! means task runnable modes */
	stack_t*		stack;
	stack_size_t	stack_size;
}task_declare_t;

typedef struct
{
	alarm_main_t main;
	/* No Autostart support */
}alarm_declare_t;

typedef struct
{
	PUBLIC void (*Init) 	(void);
	PUBLIC void (*Start)	(AppModeType);
	PUBLIC StatusType (*Schedule) (void);
	PUBLIC StatusType (*GetTaskState) ( TaskType TaskID,TaskStateRefType State );
	PUBLIC StatusType (*ActivateTask) (TaskType);
	PUBLIC StatusType (*TerminateTask) ( void );
	PUBLIC StatusType (*GetAlarmBase)  ( AlarmType, AlarmBaseRefType );
	PUBLIC StatusType (*GetAlarm)      ( AlarmType, TickRefType );
	PUBLIC StatusType (*SetRelAlarm)   ( AlarmType, TickType, TickType );
	PUBLIC StatusType (*SetAbsAlarm)   ( AlarmType, TickType, TickType );
	PUBLIC StatusType (*CancelAlarm)   ( AlarmType);
	PUBLIC TickType   (*GetOsTick)     ( void );
}OsekOs_Class;

/* ============================= [ MACROS ] =================================== */
#define DeclareStack(Name,Size)  STATIC stack_t Stack##Name[(Size+3)/4];
#define DeclareTask(Name,Autostart,AppMode)		\
	{											\
		.main = Name,							\
		.priority = TASKID_##Name,				\
		.autostart = Autostart,					\
		.app_mode = AppMode,					\
		.stack = Stack##Name,					\
		.stack_size = sizeof(Stack##Name)		\
	}

#define DeclareAlarm(Name)						\
	{											\
		.main = AlarmMain_##Name				\
	}

#include "os_cfg.h"

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST OsekOs_Class OsekOs;


#endif /* OSEKOS_H_ */
