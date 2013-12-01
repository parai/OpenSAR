

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
	CAN_CTRL_1,	/* vCanIf_Channel_1 */
	CAN_CTRL_0,	/* vCanIf_Channel_0 */
};

const uint8 CanIf_Arc_ChannelDefaultConfIndex[CANIF_CHANNEL_CNT] = {
	vCanIf_Channel_1_CONFIG_0,
	vCanIf_Channel_0_CONFIG_0,
};

// Container that gets slamed into CanIf_InitController()
// Inits ALL controllers
// Multiplicity 1/*/**
const CanIf_ControllerConfigType CanIfControllerConfig[] = {
	// This is the ConfigurationIndex in CanIf_InitController()
	{
		/*WakeupSupport =*/ CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
		/*CanIfControllerIdRef =*/ vCanIf_Channel_1,
		/*CanIfDriverNameRef =*/ "FLEXCAN",  // Not used
		/*CanIfInitControllerRef =*/ &Can_ControllerCfgData[INDEX_OF_CAN_CTRL_1],
	},
	{
		/*WakeupSupport =*/ CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
		/*CanIfControllerIdRef =*/ vCanIf_Channel_0,
		/*CanIfDriverNameRef =*/ "FLEXCAN",  // Not used
		/*CanIfInitControllerRef =*/ &Can_ControllerCfgData[INDEX_OF_CAN_CTRL_0],
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

const CanIf_HthConfigType CanIfHthConfigData_vCanIf_Channel_1[]=
{
	{
		/*CanIfHthType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfCanControllerIdRef =*/ vCanIf_Channel_1,
		/*CanIfHthIdSymRef =*/ CAN_CTRL_1_HTH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_HrhConfigType CanIfHrhConfigData_vCanIf_Channel_1[]=
{
	{
		/*CanIfHrhType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfSoftwareFilterHrh =*/ FALSE,
		/*CanIfCanControllerIdRef =*/ vCanIf_Channel_1,
		/*CanIfHrhIdSymRef =*/ CAN_CTRL_1_HRH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_HthConfigType CanIfHthConfigData_vCanIf_Channel_0[]=
{
	{
		/*CanIfHthType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfCanControllerIdRef =*/ vCanIf_Channel_0,
		/*CanIfHthIdSymRef =*/ CAN_CTRL_0_HTH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_HrhConfigType CanIfHrhConfigData_vCanIf_Channel_0[]=
{
	{
		/*CanIfHrhType =*/ CAN_ARC_HANDLE_TYPE_BASIC,
		/*CanIfSoftwareFilterHrh =*/ FALSE,
		/*CanIfCanControllerIdRef =*/ vCanIf_Channel_0,
		/*CanIfHrhIdSymRef =*/ CAN_CTRL_0_HRH,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_InitHohConfigType CanIfHohConfigData[] =
{
	{
		/*CanConfigSet =*/ &Can_ConfigSetData,
		/*CanIfHrhConfig =*/ CanIfHrhConfigData_vCanIf_Channel_1,
		/*CanIfHthConfig =*/ CanIfHthConfigData_vCanIf_Channel_1,
		/*CanIf_Arc_EOL =*/ FALSE
	},
	{
		/*CanConfigSet =*/ &Can_ConfigSetData,
		/*CanIfHrhConfig =*/ CanIfHrhConfigData_vCanIf_Channel_0,
		/*CanIfHthConfig =*/ CanIfHthConfigData_vCanIf_Channel_0,
		/*CanIf_Arc_EOL =*/ TRUE
	},
};

const CanIf_TxPduConfigType CanIfTxPduConfigData[] = 
{
	{
		/*CanIfTxPduId =*/ 0,
		/*CanIfCanTxPduIdCanId =*/ 0x00,
		/*CanIfCanTxPduIdDlc =*/ 8,
		/*CanIfCanTxPduType =*/ CANIF_PDU_TYPE_STATIC,
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadTxPduNotifyStatus =*/ FALSE, 
#endif
		/*CanIfTxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_11,
		/*CanIfUserTxConfirmation =*/ 0,
		/*CanIfCanTxPduHthRef =*/ &CanIfHthConfigData_vCanIf_Channel_1[0],
		/*PduIdRef =*/ NULL
	},
	{
		/*CanIfTxPduId =*/ CANIF_TX_vEcuC_Pdu_0,
		/*CanIfCanTxPduIdCanId =*/ 0x01,
		/*CanIfCanTxPduIdDlc =*/ 8,
		/*CanIfCanTxPduType =*/ CANIF_PDU_TYPE_STATIC,
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadTxPduNotifyStatus =*/ FALSE, 
#endif
		/*CanIfTxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_29,
		/*CanIfUserTxConfirmation =*/ NULL,
		/*CanIfCanTxPduHthRef =*/ &CanIfHthConfigData_vCanIf_Channel_0[0],
		/*PduIdRef =*/ NULL
	},
};

const CanIf_RxPduConfigType CanIfRxPduConfigData[] = 
{
	{
		/*CanIfCanRxPduId =*/ 0,
		/*CanIfCanRxPduCanId =*/ 0x01,
		/*CanIfCanRxPduDlc =*/ 3,
#if ( CANIF_CANPDUID_READDATA_API == STD_ON )
		/*CanIfReadRxPduData =*/ TRUE,
#endif
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadRxPduNotifyStatus =*/ TRUE, 
#endif
		/*CanIfRxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_29,
		/*CanIfRxUserType =*/ CANIF_USER_TYPE_CAN_NM,
		/*CanIfUserRxIndication =*/ NULL,
		/*CanIfCanRxPduHrhRef =*/ &CanIfHrhConfigData_vCanIf_Channel_1[0],
		/*PduIdRef =*/ NULL,
		/*CanIfSoftwareFilterType =*/ CANIF_SOFTFILTER_TYPE_MASK,
		/*CanIfCanRxPduCanIdMask =*/ 0x7FF
	},
	{
		/*CanIfCanRxPduId =*/ CANIF_RX_vEcuC_Pdu_0,
		/*CanIfCanRxPduCanId =*/ 0x00,
		/*CanIfCanRxPduDlc =*/ 3,
#if ( CANIF_CANPDUID_READDATA_API == STD_ON )
		/*CanIfReadRxPduData =*/ TRUE,
#endif
#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
		/*CanIfReadRxPduNotifyStatus =*/ TRUE, 
#endif
		/*CanIfRxPduIdCanIdType =*/ CANIF_CAN_ID_TYPE_11,
		/*CanIfRxUserType =*/ CANIF_USER_TYPE_CAN_SPECIAL,
		/*CanIfUserRxIndication =*/ CanIf_UserRxIndication,
		/*CanIfCanRxPduHrhRef =*/ &CanIfHrhConfigData_vCanIf_Channel_0[0],
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
