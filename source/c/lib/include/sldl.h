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

#ifndef SLDL_H_
#define SLDL_H_
/* ============================= [ INCLUDE ] ================================== */
#include "Os.h"

/* ============================= [ MACROS ] =================================== */

/* ============================= [ TYPES ] ==================================== */
typedef void*  stack_t;
typedef uint32 stack_size_t;
typedef struct
{
	PUBLIC void (*Init) (void);
	PUBLIC void (*Start) (AppModeType);
	PUBLIC void (*SetContext) ( TaskType, task_main_t, stack_t, stack_size_t);

}SLDL_Class;	/* system-level design language */


/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST SLDL_Class SLDL;
#endif /* SLDL_H_ */
