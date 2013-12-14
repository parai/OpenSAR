/*
 * app_startup.c
 *
 *  Created on: 2013Äê11ÔÂ23ÈÕ
 *      Author: parai
 */
#include "Os.h"
#include "Can.h"
#include "CanIf.h"
#include "CanTp.h"
#include "PduR.h"
#include "Dcm.h"
void StartupHook( void )
{
	Can_Init(&Can_ConfigData);

	CanIf_Init(&CanIf_Config);
	CanTp_Init();
	PduR_Init(&PduR_Config);
	Dcm_Init();

	CanIf_SetControllerMode(CANIF_CHL_LS,CANIF_CS_STARTED);
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
