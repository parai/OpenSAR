

#include "CanIf.h"
#if defined(USE_CANTP)
#include "CanTp.h"
#include "CanTp_Cbk.h"
#endif
#if defined(USE_J1939TP)
#include "J1939Tp.h"
#include "J1939Tp_Cbk.h"
#endif
#if defined(USE_PDUR)
#include "PduR.h"
#endif
#if defined(USE_CANNM)
#include "CanNm_Cbk.h"
#endif
//#include <stdlib.h>

// Imported structs from Can_PBcfg.c
extern const Can_ControllerConfigType Can_ControllerCfgData[];
extern const Can_ConfigSetType Can_ConfigSetData;

extern void CanIf_UserRxIndication(uint8 channel, PduIdType pduId, const uint8 *sduPtr,uint8 dlc, Can_IdType canId);
void CanIf_User_BusoffNotification(uint8 Controller){}
void CanIf_User_ErrorNotification(uint8 Controller,Can_Arc_ErrorType Error){}

// Contains the mapping from CanIf-specific Channels to Can Controllers
const CanControllerIdType CanIf_Arc_ChannelToControllerMap[CANIF_CHANNEL_CNT] = {
	CAN_CTRL_0,	/* CANIF_CHL_LS */
	CAN_CTRL_1,	/* CANIF_CHL_HS */
};

const uint8 CanIf_Arc_ChannelDefaultConfIndex[CANIF_CHANNEL_CNT] = {
	CANIF_CHL_LS_CONFIG_0,
	CANIF_CHL_HS_CONFIG_0,
};

// Container that gets slamed into CanIf_InitController()
// Inits ALL controllers
// Multiplicity 1/*/**
const CanIf_ControllerConfigType CanIfControllerConfig[] = {
	// This is the ConfigurationIndex in CanIf_InitController()
	{
		/*WakeupSupport =*/ CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
		/*CanIfControllerIdRef =*/ CANIF_CHL_LS,
		/*CanIfDriverNameRef =*/ "FLEXCAN",  // Not used
		/*CanIfInitControllerRef =*/ &Can_ControllerCfgData[INDEX_OF_CAN_CTRL_0],
	},
	{
		/*WakeupSupport =*/ CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
		/*CanIfControllerIdRef =*/ CANIF_CHL_HS,
		/*CanIfDriverNameRef =*/ "FLEXCAN",  // Not used
		/*CanIfInitControllerRef =*/ &Can_ControllerCfgData[INDEX_OF_CAN_CTRL_1],
	},
};

// Function callbacks for higher layers
const CanIf_DispatchConfigType CanIfDispatchConfig =
{
	/*CanIfBusOffNotification =*/ CanIf_User_BusoffNotification,
	/*CanIfWakeUpNotification =*/ NULL,        // Not used
	/*CanIfWakeupValidNotification =*/ NULL,   // Not used
	/*CanIfErrorNotificaton =*/ CanIf_User_ErrorNotification,
};

// Data for init configuration CanIfInitConfiguration

const CanIf_HthConfigType CanIfHthConfigData_CANIF_CHL_LS[]=
{
	{
		/*CanIfHthType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfCanControllerIdRef =*/ CANIF_CHL_LS,
		/*CanIfHthIdSymRef =*/ CAN_CTRL_0_HTH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_HrhConfigType CanIfHrhConfigData_CANIF_CHL_LS[]=
{
	{
		/*CanIfHrhType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfSoftwareFilterHrh =*/ FALSE,
		/*CanIfCanControllerIdRef =*/ CANIF_CHL_LS,
		/*CanIfHrhIdSymRef =*/ CAN_CTRL_0_HRH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_HthConfigType CanIfHthConfigData_CANIF_CHL_HS[]=
{
	{
		/*CanIfHthType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfCanControllerIdRef =*/ CANIF_CHL_HS,
		/*CanIfHthIdSymRef =*/ CAN_CTRL_1_HTH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_HrhConfigType CanIfHrhConfigData_CANIF_CHL_HS[]=
{
	{
		/*CanIfHrhType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfSoftwareFilterHrh =*/ FALSE,
		/*CanIfCanControllerIdRef =*/ CANIF_CHL_HS,
		/*CanIfHrhIdSymRef =*/ CAN_CTRL_1_HRH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_InitHohConfigType CanIfHohConfigData[] =
{
	{
		/*CanConfigSet =*/ &Can_ConfigSetData,
		/*CanIfHrhConfig =*/ CanIfHrhConfigData_CANIF_CHL_LS,
		/*CanIfHthConfig =*/ CanIfHthConfigData_CANIF_CHL_LS,
		/*CanIf_Arc_EOL =*/ FALSE
	},
	{
		/*CanConfigSet =*/ &Can_ConfigSetData,
		/*CanIfHrhConfig =*/ CanIfHrhConfigData_CANIF_CHL_HS,
		/*CanIfHthConfig =*/ CanIfHthConfigData_CANIF_CHL_HS,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_TxPduConfigType CanIfTxPduConfigData[] = 
{
	{
		/*CanIfTxPduId =*/ CANIF_TX_DIAG_P2P,
		/*CanIfCanTxPduIdCanId =*/ 0x732, // DiagP2P
		/*CanIfCanTxPduIdDlc =*/ 8,
		/*CanIfCanTxPduType =*/ CANIF_PDU_TYPE_STATIC,
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadTxPduNotifyStatus =*/ FALSE, 
#endif
		/*CanIfTxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_11,
		/*CanIfUserTxConfirmation =*/ CanTp_TxConfirmation,
		/*CanIfCanTxPduHthRef =*/ &CanIfHthConfigData_CANIF_CHL_LS[0],
		/*PduIdRef =*/ NULL
	},
	{
		/*CanIfTxPduId =*/ CANIF_TX_DIAG_P2A,
		/*CanIfCanTxPduIdCanId =*/ 0x742, // DiagP2A
		/*CanIfCanTxPduIdDlc =*/ 8,
		/*CanIfCanTxPduType =*/ CANIF_PDU_TYPE_STATIC,
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadTxPduNotifyStatus =*/ FALSE, 
#endif
		/*CanIfTxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_11,
		/*CanIfUserTxConfirmation =*/ CanTp_TxConfirmation,
		/*CanIfCanTxPduHthRef =*/ &CanIfHthConfigData_CANIF_CHL_LS[0],
		/*PduIdRef =*/ NULL
	},
};

const CanIf_RxPduConfigType CanIfRxPduConfigData[] = 
{
	{
		/*CanIfCanRxPduId =*/ CANIF_RX_DIAG_P2P,
		/*CanIfCanRxPduCanId =*/ 0x731, // DiagP2P
		/*CanIfCanRxPduDlc =*/ 3,
#if ( CANIF_CANPDUID_READDATA_API == STD_ON )
		/*CanIfReadRxPduData =*/ TRUE,
#endif
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadRxPduNotifyStatus =*/ TRUE, 
#endif
		/*CanIfRxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_11,
		/*CanIfRxUserType =*/ CANIF_USER_TYPE_CAN_TP,
		/*CanIfUserRxIndication =*/ NULL,
		/*CanIfCanRxPduHrhRef =*/ &CanIfHrhConfigData_CANIF_CHL_LS[0],
		/*PduIdRef =*/ NULL,
		/*CanIfSoftwareFilterType =*/ CANIF_SOFTFILTER_TYPE_MASK,
		/*CanIfCanRxPduCanIdMask =*/ 0x7FF
	},
	{
		/*CanIfCanRxPduId =*/ CANIF_RX_DIAG_P2A,
		/*CanIfCanRxPduCanId =*/ 0x741, // DiagP2A
		/*CanIfCanRxPduDlc =*/ 3,
#if ( CANIF_CANPDUID_READDATA_API == STD_ON )
		/*CanIfReadRxPduData =*/ TRUE,
#endif
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadRxPduNotifyStatus =*/ TRUE, 
#endif
		/*CanIfRxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_11,
		/*CanIfRxUserType =*/ CANIF_USER_TYPE_CAN_TP,
		/*CanIfUserRxIndication =*/ NULL,
		/*CanIfCanRxPduHrhRef =*/ &CanIfHrhConfigData_CANIF_CHL_LS[0],
		/*PduIdRef =*/ NULL,
		/*CanIfSoftwareFilterType =*/ CANIF_SOFTFILTER_TYPE_MASK,
		/*CanIfCanRxPduCanIdMask =*/ 0x7FF
	},
};

// This container contains the init parameters of the CAN
// Multiplicity 1/*/**
const CanIf_InitConfigType CanIfInitConfig =
{
	/*CanIfConfigSet =*/ 0, // Not used 
	/*CanIfNumberOfCanRxPduIds =*/ 2,
	/*CanIfNumberOfCanTXPduIds =*/ 2,
	/*CanIfNumberOfDynamicCanTXPduIds =*/ 0, // Not used
	// Containers
	/*CanIfHohConfigPtr =*/ CanIfHohConfigData,
	/*CanIfRxPduConfigPtr =*/ CanIfRxPduConfigData,
	/*CanIfTxPduConfigPtr =*/ CanIfTxPduConfigData,
};
// This container includes all necessary configuration sub-containers
// according the CAN Interface configuration structure
const CanIf_ConfigType CanIf_Config =
{
	/*ControllerConfig =*/ CanIfControllerConfig,
	/*DispatchConfig =*/ &CanIfDispatchConfig,
	/*InitConfig =*/ &CanIfInitConfig,
	/*TransceiverConfig =*/ NULL, // Not used
	/*Arc_ChannelToControllerMap =*/ CanIf_Arc_ChannelToControllerMap,
	/*Arc_ChannelDefaultConfIndex =*/ CanIf_Arc_ChannelDefaultConfIndex,
};
