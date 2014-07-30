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
#include "e4c.h"
#include "sldl.h"
#include <setjmp.h>
#include <stdlib.h>

/* ============================= [ TYPES ] ==================================== */
typedef jmp_buf task_context_t;

/* ============================= [ MACROS ] =================================== */
#define SAVE_SP(stack) 		__asm__ ( "movl %%esp,%0":"=m" ( stack ) )
#define RESTORE_SP(stack) 	__asm__ ( "movl %0,%%esp":"=m" ( stack ) )

/* ============================= [ DATAS ] ==================================== */
PRIVATE task_context_t  TaskContext[TASK_NUM];
PRIVATE void*           pSystemStack;

/* ============================= [ FUNCTION ] ================================= */
PUBLIC STATIC void Init ( void )
{

}

PUBLIC STATIC void SetContext ( TaskType TaskId, task_main_t task_main, stack_t stack , stack_size_t stack_size)
{
	int result;
	stack_t task_stack = (stack_t)(((stack_size_t)stack) + stack_size);

	assert(stack != NULL);
	assert( (stack_size > 255) && (0 == (stack_size&0x03u)) );

	SAVE_SP(pSystemStack);
	RESTORE_SP(task_stack);
	result = setjmp(TaskContext[TaskId]);

	switch(result)
	{
		case 0:
			RESTORE_SP(pSystemStack);	/* set-up context done */
			break;
		case 1:
			task_main();	/* execute this task */
			break;
	}
}

PUBLIC STATIC void Start( AppModeType app_mode )
{
	//Os.Start(app_mode);

	e4c_context_begin(false);
	try
	{
		throw(RuntimeException,NULL);
	}
	catch(RuntimeException)
	{
		printf("Hello World!\n");
	}
	finally
	{
		printf("do finally!\n");
	}
	e4c_context_end();
}


/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST SLDL_Class SLDL = {
	.Init  = Init,
	.Start = Start,
	.SetContext = SetContext,
};

