/*
 * app_startup.c
 *
 *  Created on: 2013Äê11ÔÂ23ÈÕ
 *      Author: parai
 */
#include "Os.h"
void StartupHook( void )
{

}
void ShutdownHook( StatusType Error )
{

}
void ErrorHook( StatusType Error )
{

}
void PreTaskHook( void )
{

}
void PostTaskHook( void )
{

}

// to know how to use Os firstly, then ...
extern void Os_IsrInit( void );
extern void InitOS( void );
void EcuM_Init(void)
{
	// Initialize the OS
	InitOS();

	// Setup interrupts
	Os_IsrInit();

	StartOS(OSDEFAULTAPPMODE);
}
