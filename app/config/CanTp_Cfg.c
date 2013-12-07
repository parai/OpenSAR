
#include "CanTp.h"
//#if defined(USE_CANIF)
#include "CanIf.h"
//#endif
//#if defined(USE_PDUR)
#include "PduR.h"
//#endif

const CanTp_GeneralType CanTpGeneralConfig =
{
	.main_function_period =  10,
};

//NSa
const CanTp_NSaType CanTpNSaConfig =
{
	.CanTpNSa =  0,
};

const CanTp_NTaType CanTpNTaConfig =
{
	.CanTpNTa =  0,
};

const CanTp_NSduType CanTpNSduConfigList[] =
{
	{
		.direction =  ISO15765_RECEIVE,
		.listItemType =  CANTP_NOT_LAST_ENTRY,
		.configData.CanTpRxNSdu.CanTp_FcPduId =  CANTP_DIAG_P2P_REQ,
		.configData.CanTpRxNSdu.CanIf_FcPduId =  CANIF_DIAG_P2P_ACK,
		.configData.CanTpRxNSdu.PduR_PduId =  PDUR_DIAG_P2P_REQ,
		.configData.CanTpRxNSdu.CanTpAddressingFormant =  CANTP_STANDARD,
		.configData.CanTpRxNSdu.CanTpBs =  8,
		.configData.CanTpRxNSdu.CanTpNar =  100,
		.configData.CanTpRxNSdu.CanTpNbr =  100,
		.configData.CanTpRxNSdu.CanTpNcr =  100,
		.configData.CanTpRxNSdu.CanTpRxChannel =  CANTP_DIAG_P2P_REQ,
		.configData.CanTpRxNSdu.CanTpRxDI =  8,
		.configData.CanTpRxNSdu.CanTpRxPaddingActivation =  CANTP_OFF,
		.configData.CanTpRxNSdu.CanTpRxTaType =  CANTP_PHYSICAL,
		.configData.CanTpRxNSdu.CanTpWftMax =  5,
		.configData.CanTpRxNSdu.CanTpSTmin =  0,
		.configData.CanTpRxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpRxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
	{
		.direction =  IS015765_TRANSMIT,
		.listItemType =  CANTP_NOT_LAST_ENTRY,
		.configData.CanTpTxNSdu.CanTp_FcPduId =  CANTP_DIAG_P2P_ACK,
		.configData.CanTpTxNSdu.CanIf_PduId   =  CANIF_DIAG_P2P_ACK,
		.configData.CanTpTxNSdu.PduR_PduId =  PDUR_DIAG_P2P_ACK,
		.configData.CanTpTxNSdu.CanTpAddressingMode =  CANTP_STANDARD,
		.configData.CanTpTxNSdu.CanTpNas =  100,
		.configData.CanTpTxNSdu.CanTpNbs =  100,
		.configData.CanTpTxNSdu.CanTpNcs =  100,
		.configData.CanTpTxNSdu.CanTpTxChannel =  CANTP_DIAG_P2P_ACK,
		.configData.CanTpTxNSdu.CanTpTxDI =  8,
		.configData.CanTpTxNSdu.CanTpTxPaddingActivation =  CANTP_OFF,
		.configData.CanTpTxNSdu.CanTpTxTaType =  CANTP_PHYSICAL,
		.configData.CanTpTxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpTxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
	{
		.direction =  ISO15765_RECEIVE,
		.listItemType =  CANTP_NOT_LAST_ENTRY,
		.configData.CanTpRxNSdu.CanTp_FcPduId =  CANTP_DIAG_P2A_REQ,
		.configData.CanTpRxNSdu.CanIf_FcPduId =  CANIF_DIAG_P2A_ACK,
		.configData.CanTpRxNSdu.PduR_PduId =  PDUR_DIAG_P2A_REQ,
		.configData.CanTpRxNSdu.CanTpAddressingFormant =  CANTP_STANDARD,
		.configData.CanTpRxNSdu.CanTpBs =  4,
		.configData.CanTpRxNSdu.CanTpNar =  100,
		.configData.CanTpRxNSdu.CanTpNbr =  100,
		.configData.CanTpRxNSdu.CanTpNcr =  100,
		.configData.CanTpRxNSdu.CanTpRxChannel =  CANTP_DIAG_P2A_REQ,
		.configData.CanTpRxNSdu.CanTpRxDI =  8,
		.configData.CanTpRxNSdu.CanTpRxPaddingActivation =  CANTP_OFF,
		.configData.CanTpRxNSdu.CanTpRxTaType =  CANTP_PHYSICAL,
		.configData.CanTpRxNSdu.CanTpWftMax =  5,
		.configData.CanTpRxNSdu.CanTpSTmin =  0,
		.configData.CanTpRxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpRxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
	{
		.direction =  IS015765_TRANSMIT,
		.listItemType =  CANTP_END_OF_LIST,
		.configData.CanTpTxNSdu.CanTp_FcPduId =  CANTP_DIAG_P2A_ACK,
		.configData.CanTpTxNSdu.CanIf_PduId =  CANIF_DIAG_P2A_ACK,
		.configData.CanTpTxNSdu.PduR_PduId =  PDUR_DIAG_P2A_ACK,
		.configData.CanTpTxNSdu.CanTpAddressingMode =  CANTP_STANDARD,
		.configData.CanTpTxNSdu.CanTpNas =  100,
		.configData.CanTpTxNSdu.CanTpNbs =  100,
		.configData.CanTpTxNSdu.CanTpNcs =  100,
		.configData.CanTpTxNSdu.CanTpTxChannel =  CANTP_DIAG_P2A_ACK,
		.configData.CanTpTxNSdu.CanTpTxDI =  8,
		.configData.CanTpTxNSdu.CanTpTxPaddingActivation =  CANTP_OFF,
		.configData.CanTpTxNSdu.CanTpTxTaType =  CANTP_PHYSICAL,
		.configData.CanTpTxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpTxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
};

const CanTp_RxIdType CanTp_RxIdList[] = 
{
	//DIAG_P2P_REQ
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_DIAG_P2P_REQ,
		.CanTpReferringTxIndex =  CANTP_DIAG_P2P_ACK,
	},
	//DIAG_P2P_ACK
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_DIAG_P2P_ACK,
		.CanTpReferringTxIndex =  0xFFFF,
	},
	//DIAG_P2A_REQ
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_DIAG_P2A_REQ,
		.CanTpReferringTxIndex =  CANTP_DIAG_P2A_ACK,
	},
	//DIAG_P2A_ACK
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_DIAG_P2A_ACK,
		.CanTpReferringTxIndex =  0xFFFF,
	},
};

const CanTp_ConfigType CanTpConfig =
{
	.CanTpGeneral 	=  	&CanTpGeneralConfig,
	.CanTpNSduList = 	CanTpNSduConfigList,
	.CanTpRxIdList =    CanTp_RxIdList
};

