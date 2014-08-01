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
#include <time.h>
#include <stdlib.h>


/* ============================= [ TYPES ] ==================================== */
typedef jmp_buf task_context_t;

/* ============================= [ MACROS ] =================================== */
#define SAVE_SP(stack) 		__asm__ ( "movl %%esp,%0":"=m" ( stack ) )
#define RESTORE_SP(stack) 	__asm__ ( "movl %0,%%esp":"=m" ( stack ) )


#define DRIVE_OS_TICK()		OsTick()
#define OS_TICK_INIT()      PreviousClock = clock()
#define OS_TICK_RESTART()   PreviousClock = clock()
// normally CLOCKS_PER_SEC == 1000, so it is OK
#define OS_TICK_1_MS_ELAPSED if( (clock() > PreviousClock) ||  (clock() </*overflow*/ PreviousClock))

/* ============================= [ DATAS ] ==================================== */
PRIVATE E4C_DEFINE_EXCEPTION(DispatcherException,"Dispatcher of the OS",RuntimeException);
PRIVATE task_context_t  TaskContext[TASK_NUM];
PRIVATE void*           pSystemStack;

PRIVATE clock_t	PreviousClock;	// see CLOCKS_PER_SEC
/* ============================= [ IMPORT ] ================================== */
IMPORT PROTECT void OsTick(void);
/* ============================= [ FUNCTION ] ================================= */
PRIVATE void Simulator(void)
{
	OS_TICK_1_MS_ELAPSED
	{
		OS_TICK_RESTART();
		OsTick();
	}
}
PUBLIC STATIC void Init ( void )
{
	OS_TICK_INIT();
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

PUBLIC STATIC void Start( void )
{
	e4c_context_begin(false);
	try
	{
		EcuM.Init();

		throw(RuntimeException,"Start the operating system failed.");
	}
	catch(DispatcherException)
	{

	}
	finally
	{
		// do nothing...
	}

	for(;;)
	{
		try{

		}
		catch(DispatcherException)
		{

		}
		finally
		{
			Simulator();
		}
	}
	e4c_context_end();
}
PUBLIC STATIC void Dispatch(void)
{
	throw(DispatcherException,NULL);
}

/* ============================= [ INTERFACE ] ================================ */
INSTANCE CONST SLDL_Class SLDL = {
	.Init  = Init,
	.Start = Start,
	.SetContext = SetContext,
	.Dispatch   = Dispatch,
};

