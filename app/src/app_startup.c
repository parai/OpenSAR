#include "app.h"

void StartupHook( void )
{
#if defined(USE_DET)
	Det_Init();
	Det_Start();
#endif
	Port_Init(&PortConfigData);
	Can_Init(&Can_ConfigData);

	CanIf_Init(&CanIf_Config);
	CanTp_Init();
	PduR_Init(&PduR_Config);
	Dcm_Init();
	Com_Init(&ComConfiguration);
	Com_IpduGroupStart(COM_DEFAULT_IPDU_GROUP,True);

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
