
#include "CanTp.h"
#if defined(USE_CANIF)
#include "CanIf.h"
#endif
#if defined(USE_PDUR)
#include "PduR.h"
#endif

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
		.configData.CanTpRxNSdu.CanTp_FcPduId =  CANTP_RX_DIAG_P2P,
		.configData.CanTpRxNSdu.CanIf_FcPduId =  CANIF_TX_DIAG_P2P,
		.configData.CanTpRxNSdu.PduR_PduId =  PDUR_RX_DIAG_P2P,
		.configData.CanTpRxNSdu.CanTpAddressingFormant =  CANTP_STANDARD,
		.configData.CanTpRxNSdu.CanTpBs =  8,
		.configData.CanTpRxNSdu.CanTpNar =  100,
		.configData.CanTpRxNSdu.CanTpNbr =  100,
		.configData.CanTpRxNSdu.CanTpNcr =  100,
		.configData.CanTpRxNSdu.CanTpRxChannel =  CANTP_RX_DIAG_P2P,
		.configData.CanTpRxNSdu.CanTpRxDI =  8,
		.configData.CanTpRxNSdu.CanTpRxPaddingActivation =  CANTP_OFF,
		.configData.CanTpRxNSdu.CanTpRxTaType =  CANTP_PHYSICAL,
		.configData.CanTpRxNSdu.CanTpWftMax =  5,
		.configData.CanTpRxNSdu.CanTpSTmin =  0,
		.configData.CanTpRxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpRxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
	{
		.direction =  ISO15765_RECEIVE,
		.listItemType =  CANTP_NOT_LAST_ENTRY,
		.configData.CanTpRxNSdu.CanTp_FcPduId =  CANTP_RX_DIAG_P2A,
		.configData.CanTpRxNSdu.CanIf_FcPduId =  CANIF_TX_DIAG_P2A,
		.configData.CanTpRxNSdu.PduR_PduId =  PDUR_RX_DIAG_P2A,
		.configData.CanTpRxNSdu.CanTpAddressingFormant =  CANTP_STANDARD,
		.configData.CanTpRxNSdu.CanTpBs =  4,
		.configData.CanTpRxNSdu.CanTpNar =  100,
		.configData.CanTpRxNSdu.CanTpNbr =  100,
		.configData.CanTpRxNSdu.CanTpNcr =  100,
		.configData.CanTpRxNSdu.CanTpRxChannel =  CANTP_RX_DIAG_P2A,
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
		.configData.CanTpTxNSdu.CanTp_FcPduId =  CANTP_TX_DIAG_P2P,
		.configData.CanTpTxNSdu.CanIf_PduId =  CANIF_RX_DIAG_P2P,
		.configData.CanTpTxNSdu.PduR_PduId =  PDUR_TX_DIAG_P2P,
		.configData.CanTpTxNSdu.CanTpAddressingMode =  CANTP_STANDARD,
		.configData.CanTpTxNSdu.CanTpNas =  100,
		.configData.CanTpTxNSdu.CanTpNbs =  100,
		.configData.CanTpTxNSdu.CanTpNcs =  100,
		.configData.CanTpTxNSdu.CanTpTxChannel =  CANTP_TX_DIAG_P2P,
		.configData.CanTpTxNSdu.CanTpTxDI =  8,
		.configData.CanTpTxNSdu.CanTpTxPaddingActivation =  CANTP_OFF,
		.configData.CanTpTxNSdu.CanTpTxTaType =  CANTP_PHYSICAL,
		.configData.CanTpTxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpTxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
	{
		.direction =  IS015765_TRANSMIT,
		.listItemType =  CANTP_END_OF_LIST,
		.configData.CanTpTxNSdu.CanTp_FcPduId =  CANTP_TX_DIAG_P2A,
		.configData.CanTpTxNSdu.CanIf_PduId =  CANIF_RX_DIAG_P2A,
		.configData.CanTpTxNSdu.PduR_PduId =  PDUR_TX_DIAG_P2A,
		.configData.CanTpTxNSdu.CanTpAddressingMode =  CANTP_STANDARD,
		.configData.CanTpTxNSdu.CanTpNas =  100,
		.configData.CanTpTxNSdu.CanTpNbs =  100,
		.configData.CanTpTxNSdu.CanTpNcs =  100,
		.configData.CanTpTxNSdu.CanTpTxChannel =  CANTP_TX_DIAG_P2A,
		.configData.CanTpTxNSdu.CanTpTxDI =  8,
		.configData.CanTpTxNSdu.CanTpTxPaddingActivation =  CANTP_OFF,
		.configData.CanTpTxNSdu.CanTpTxTaType =  CANTP_PHYSICAL,
		.configData.CanTpTxNSdu.CanTpNSa =  &CanTpNSaConfig,
		.configData.CanTpTxNSdu.CanTpNTa =  &CanTpNTaConfig,
	},
};

const CanTp_RxIdType CanTp_RxIdList[] = 
{
	//CANTP_RX_DIAG_P2P
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_RX_DIAG_P2P,
		.CanTpReferringTxIndex =  CANTP_TX_DIAG_P2P,
	},
	//CANTP_RX_DIAG_P2A
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_RX_DIAG_P2A,
		.CanTpReferringTxIndex =  CANTP_TX_DIAG_P2A,
	},
	//CANTP_TX_DIAG_P2P
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_TX_DIAG_P2P,
		.CanTpReferringTxIndex =  0xFFFF,
	},
	//CANTP_TX_DIAG_P2A
	{
		.CanTpAddressingMode =  CANTP_STANDARD,
		.CanTpNSduIndex =  CANTP_TX_DIAG_P2A,
		.CanTpReferringTxIndex =  0xFFFF,
	},
};

const CanTp_ConfigType CanTpConfig =
{
	.CanTpGeneral 	=  	&CanTpGeneralConfig,
	.CanTpNSduList = 	CanTpNSduConfigList,
	.CanTpRxIdList =     CanTp_RxIdList
};

