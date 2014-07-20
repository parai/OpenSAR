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

#ifndef OS_H_
#define OS_H_

/* ============================= [ INCLUDE ] ================================== */
#include "Std_Types.h"

/* ============================= [ TYPES ] ==================================== */
/*! standard OS types */
typedef uint8			TaskType;
typedef TaskType*		TaskRefType;
typedef uint32			TickType;
typedef TickType*		TickRefType;
typedef uint8 			StatusType;
typedef uint32 			AppModeType;	/*! each bit is a mode */

/*! extended OS types */
typedef void_fn_void_t task_entry_t;
typedef uint8 		   task_priority_t;

/*! \class Os_Class "Os.h"
 *  \brief Operating System.
 *
 * AUTOSAR OS
 */
typedef struct
{
	/*! \fn void Init( void )
	 *  \brief Initialize OS.
	 */
	PUBLIC void (*Init) 	(void);
	/*! \fn void Start( AppModeType app_mode )
	 *  \brief Start OS.
	 *  \param app_mode application mode
	 */
	PUBLIC void (*Start)	(AppModeType);
	PUBLIC StatusType (*Schedule) (void);
	PUBLIC StatusType (*ActivateTask) (TaskType);
	PUBLIC StatusType (*TerminateTask) ( void );
}Os_Class;

/* ============================= [ MACROS ] =================================== */
#define E_OS_ACCESS 	(StatusType)1
#define	E_OS_CALLEVEL 	(StatusType)2
#define	E_OS_ID			(StatusType)3
#define	E_OS_LIMIT 		(StatusType)4
#define	E_OS_NOFUNC 	(StatusType)5
#define	E_OS_RESOURCE 	(StatusType)6
#define	E_OS_STATE 		(StatusType)7
#define	E_OS_VALUE 		(StatusType)8

#define OSDEFAULTAPPMODE (AppModeType)1

#define TASK(_task)		void _task( void )

#include "OsekOs.h"

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST Os_Class Os;
#endif /* OS_H_ */
