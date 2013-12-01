
#include "CanTp.h"
#if defined(USE_CANIF)
#include "CanIf.h"
#endif
#if defined(USE_PDUR)
#include "PduR.h"
#endif

const CanTp_GeneralType CanTpGeneralConfig =
{
	/* .main_function_period = */ 10,
};

//NSa
const CanTp_NSaType CanTpNSaConfig_RX_vEcuC_Pdu_1 = 
{
	/* .CanTpNSa = */ 0,
};
//NTa
const CanTp_NTaType CanTpNTaConfig_RX_vEcuC_Pdu_1 = 
{
	/* .CanTpNTa = */ 0,
};

//NSa
const CanTp_NSaType CanTpNSaConfig_TX_vEcuC_Pdu_1 = 
{
	/* .CanTpNSa = */ 0,
};
//NTa
const CanTp_NTaType CanTpNTaConfig_TX_vEcuC_Pdu_1 = 
{
	/* .CanTpNTa = */ 0,
};

const CanTp_NSduType CanTpNSduConfigList[] =
{
	{
		/* .direction = */ ISO15765_RECEIVE,
		/* .listItemType = */ CANTP_NOT_LAST_ENTRY,
		{
			/* .configData.CanTpRxNSdu.CanTp_FcPduId = */ 0xDEAD,
			/* .configData.CanTpRxNSdu.CanIf_FcPduId = */ 0xDEAD,
			/* .configData.CanTpRxNSdu.PduR_PduId = */ PDUR_RX_vEcuC_Pdu_1,
			/* .configData.CanTpRxNSdu.CanTpAddressingFormant = */ CANTP_STANDARD,
			/* .configData.CanTpRxNSdu.CanTpBs = */ 4,
			/* .configData.CanTpRxNSdu.CanTpNar = */ 100,
			/* .configData.CanTpRxNSdu.CanTpNbr = */ 100,
			/* .configData.CanTpRxNSdu.CanTpNcr = */ 100,
			/* .configData.CanTpRxNSdu.CanTpRxChannel = */ CANTP_RX_vEcuC_Pdu_1,
			/* .configData.CanTpRxNSdu.CanTpRxDI = */ 8,
			/* .configData.CanTpRxNSdu.CanTpRxPaddingActivation = */ CANTP_OFF,
			/* .configData.CanTpRxNSdu.CanTpRxTaType = */ CANTP_PHYSICAL,
			/* .configData.CanTpRxNSdu.CanTpWftMax = */ 5,
			/* .configData.CanTpRxNSdu.CanTpSTmin = */ 0,
			/* .configData.CanTpRxNSdu.CanTpNSa = */ &CanTpNSaConfig_RX_vEcuC_Pdu_1,
			/* .configData.CanTpRxNSdu.CanTpNTa = */ &CanTpNTaConfig_RX_vEcuC_Pdu_1,
		}
	},
	{
		/* .direction = */ IS015765_TRANSMIT,
		/* .listItemType = */ CANTP_END_OF_LIST,
		{
			/* .configData.CanTpTxNSdu.CanTp_FcPduId = */ 0xDEAD,
			/* .configData.CanTpTxNSdu.CanIf_PduId = */ 0xDEAD,
			/* .configData.CanTpTxNSdu.PduR_PduId = */ PDUR_ALT_TX_vEcuC_Pdu_1,
			/* .configData.CanTpTxNSdu.CanTpAddressingMode = */ CANTP_STANDARD,
			/* .configData.CanTpRxNSdu.reserved_CanTpBs = */ 0xDB,
			/* .configData.CanTpTxNSdu.CanTpNas = */ 100,
			/* .configData.CanTpTxNSdu.CanTpNbs = */ 100,
			/* .configData.CanTpTxNSdu.CanTpNcs = */ 100,
			/* .configData.CanTpTxNSdu.CanTpTxChannel = */ CANTP_TX_vEcuC_Pdu_1,
			/* .configData.CanTpTxNSdu.CanTpTxDI = */ 8,
			/* .configData.CanTpTxNSdu.CanTpTxPaddingActivation = */ CANTP_OFF,
			/* .configData.CanTpTxNSdu.CanTpTxTaType = */ CANTP_PHYSICAL,
			/* .configData.CanTpTxNSdu.reserved_CanTpWftMax = */ 0xDB,
			/* .configData.CanTpTxNSdu.reserved_CanTpSTmin = */ 0xDEAD,
			/* .configData.CanTpTxNSdu.CanTpNSa = */ &CanTpNSaConfig_TX_vEcuC_Pdu_1,
			/* .configData.CanTpTxNSdu.CanTpNTa = */ &CanTpNTaConfig_TX_vEcuC_Pdu_1,
		}
	},
};

const CanTp_RxIdType CanTp_RxIdList[] = 
{
	/* RX_vEcuC_Pdu_1 */
	{
		/* .CanTpAddressingMode =  */CANTP_STANDARD,
		/* .CanTpNSduIndex =  */0,
		/* .CanTpReferringTxIndex = */ 1,
	},
	/* TX_vEcuC_Pdu_1 */
	{
		/* .CanTpAddressingMode =  */CANTP_STANDARD,
		/* .CanTpNSduIndex =  */1,
		/* .CanTpReferringTxIndex = */ 0xFFFF,
	},
};

const CanTp_ConfigType CanTpConfig =
{
	/* .CanTpGeneral 	= */ 	&CanTpGeneralConfig,
	/* .CanTpNSduList = */	CanTpNSduConfigList,
	/* .CanTpRxIdList = */    CanTp_RxIdList
};

