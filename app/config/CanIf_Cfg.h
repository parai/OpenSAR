
// + Channel      ( Channel Interface to CAN Controller)
// +--+ HTH       ( HTH Interface to the HTH of CAN Controller)
// ++----+ Pdu0
// ++----+ Pdu1
// .............
// +--+ HRH       ( HRH Interface to the HRH of CAN Controller)
// ++----+ Pdu3
// ++----+ Pdu4
// .............


#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_

#include "Can.h"

#define CANIF_VERSION_INFO_API   STD_OFF
#define CANIF_DEV_ERROR_DETECT   STD_OFF
#define CANIF_DLC_CHECK          STD_OFF
#define CANIF_MULITPLE_DRIVER_SUPPORT       STD_OFF   // Not supported
#define CANIF_READRXPDU_DATA_API			 STD_OFF   // Not supported
#define CANIF_READRXPDU_NOTIFY_STATUS_API	 STD_OFF   // Not supported
#define CANIF_READTXPDU_NOTIFY_STATUS_API	 STD_OFF   // Not supported
#define CANIF_SETDYNAMICTXID_API            STD_OFF   // Not supported
#define CANIF_WAKEUP_EVENT_API			     STD_OFF   // Not supported
#define CANIF_TRANSCEIVER_API               STD_OFF   // Not supported
#define CANIF_TRANSMIT_CANCELLATION         STD_OFF   // Not supported
#define CANIF_ARC_RUNTIME_PDU_CONFIGURATION STD_OFF   // Not supported
#define CANIF_CANPDUID_READDATA_API         STD_OFF   // Not supported
#define CANIF_READRXPDU_NOTIF_STATUS_API    STD_OFF   // Not supported

/* Tx PduId For CanIF */
#define CANIF_TX_DIAG_P2P		0
#define CANIF_TX_DIAG_P2A		1
/* Rx PduId For CanIF */
#define CANIF_RX_DIAG_P2P		0
#define CANIF_RX_DIAG_P2A		1
// Identifiers for the elements in CanIfControllerConfig[]
// This is the ConfigurationIndex in CanIf_InitController()
typedef enum {
	CANIF_CHL_LS_CONFIG_0,
	CANIF_CHL_HS_CONFIG_0,
	CANIF_CHANNEL_CONFIGURATION_CNT
}CanIf_Arc_ConfigurationIndexType;

typedef enum {
	CANIF_CHL_LS,  // Low  Speed CanIf Channel map to CAN_CTRL_0
	CANIF_CHL_HS,  // High Speed CanIf Channel map to CAN_CTRL_1
	CANIF_CHANNEL_CNT
}CanIf_Arc_ChannelIdType;

#include "CanIf_ConfigTypes.h"

extern const CanIf_ConfigType CanIf_Config;

#endif

