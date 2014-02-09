#include "CanSM.h"
#include "CanIf.h"
#include "ComM.h"
#include "Com.h"
const CanSM_ControllerType CanSmNetwork_Controllers[] =
{
    {.CanIfControllerId =  CANIF_CHL_LS},
    {.CanIfControllerId =  CANIF_CHL_HS}
}; 
const const CanSM_NetworkType CanSM_Networks[] = 
{
    {
        .Controllers =   &CanSmNetwork_Controllers[0],
        .ControllerCount =  1,
        .ComMNetworkHandle =  COMM_CHL_LS,
        .ComRxPduGroupId =    0,
        .ComTxPduGroupId =    0,
        .CanSMBorTimeTxEnsured =  1000, //ms
        .CanSMBorTimeL1 =  100,
        .CanSMBorTimeL2 =  100,
        .CanSMBorCounterL1ToL2 =  100,
        .CanSMBorCounterL2Err =   100,
        .CanSMBorTxConfirmationPolling =  TRUE,
        .CanSMBorDisableRxDlMonitoring =  TRUE,
        #if defined(USE_DEM)
        .CanSMBusOffDemEvent =  0xEF,
        #endif         
    },
    {
		.Controllers =   &CanSmNetwork_Controllers[1],
		.ControllerCount =  1,
		.ComMNetworkHandle =  COMM_CHL_HS,
		.ComRxPduGroupId =    0,
		.ComTxPduGroupId =    0,
		.CanSMBorTimeTxEnsured =  1000, //ms
		.CanSMBorTimeL1 =  100,
		.CanSMBorTimeL2 =  100,
		.CanSMBorCounterL1ToL2 =  100,
		.CanSMBorCounterL2Err =   100,
		.CanSMBorTxConfirmationPolling =  TRUE,
		.CanSMBorDisableRxDlMonitoring =  TRUE,
		#if defined(USE_DEM)
		.CanSMBusOffDemEvent =  0xEF,
		#endif
	}
};
const CanSM_ConfigType CanSM_Cfg =
{
    .Networks =  CanSM_Networks
};
