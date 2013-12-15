#include "CanSM.h"
#include "CanIf.h"
#include "ComM.h"
#include "Com.h"
const CanSM_ControllerType vCanSmNetwork_Controllers[] = 
{
    {.CanIfControllerId =  CANIF_CHL_LS}
}; 
const const CanSM_NetworkType CanSM_Networks[] = 
{
    {
        .Controllers =   vCanSmNetwork_Controllers,
        .ControllerCount =  1,
        .ComMNetworkHandle =  vComM_Channel_0,
        .ComRxPduGroupId =    COM_RX_vEcuC_Pdu_1,
        .ComTxPduGroupId =    COM_TX_vEcuC_Pdu_1,
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
