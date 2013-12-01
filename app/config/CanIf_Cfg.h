
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
#define CANIF_TX_vEcuC_Pdu_1		0
#define CANIF_TX_vEcuC_Pdu_0		1
/* Rx PduId For CanIF */
#define CANIF_RX_vEcuC_Pdu_1		0
#define CANIF_RX_vEcuC_Pdu_0		1
// Identifiers for the elements in CanIfControllerConfig[]
// This is the ConfigurationIndex in CanIf_InitController()
typedef enum {
	vCanIf_Channel_1_CONFIG_0,
	vCanIf_Channel_0_CONFIG_0,
	CANIF_CHANNEL_CONFIGURATION_CNT
}CanIf_Arc_ConfigurationIndexType;

typedef enum {
	vCanIf_Channel_1,
	vCanIf_Channel_0,
	CANIF_CHANNEL_CNT
}CanIf_Arc_ChannelIdType;

#include "CanIf_ConfigTypes.h"

extern const CanIf_ConfigType CanIf_Config;

#endif

