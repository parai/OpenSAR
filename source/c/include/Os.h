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
typedef uint8			TaskType;
typedef TaskType*		TaskRefType;
typedef uint32			TickType;
typedef TickType*		TickRefType;
typedef uint8 			StatusType;
typedef uint32 			AppModeType;
typedef struct
{
	PUBLIC void (*Init) 	(void);
	PUBLIC void (*Start)	(AppModeType);
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

/* ============================= [ INTERFACE ] ================================ */
extern const Os_Class Os;
#endif /* OS_H_ */
