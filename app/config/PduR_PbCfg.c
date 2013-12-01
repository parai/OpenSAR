
#include "PduR.h"

#if PDUR_CANIF_SUPPORT == STD_ON
#include "CanIf.h"
#endif
#if PDUR_CANTP_SUPPORT == STD_ON
#include "CanTp.h"
#endif
#if PDUR_LINIF_SUPPORT == STD_ON
#include "LinIf.h"
#endif
#if PDUR_COM_SUPPORT == STD_ON
#include "Com.h"
#endif
#if PDUR_DCM_SUPPORT == STD_ON
#include "Dcm.h"
#endif
#if PDUR_J1939TP_SUPPORT == STD_ON
#include "J1939Tp.h"
#endif
#if(PDUR_ZERO_COST_OPERATION == STD_OFF)
const PduRDestPdu_type PduR_PduRDestination[] = {

	{ // DIAG_P2P TX
		/* .DataProvision = */ PDUR_NO_PROVISION,
		/* .DestPduId = */ CANTP_TX_DIAG_P2P,
		/* .TxBufferRef = */ NULL,
		/* .DestModule = */ ARC_PDUR_CANTP
	},
	{ // DIAG_P2P RX
		/* .DataProvision = */ PDUR_NO_PROVISION,
		/* .DestPduId = */ DCM_TX_DIAG_P2P,
		/* .TxBufferRef = */ NULL,
		/* .DestModule = */ ARC_PDUR_DCM
	},
	{ // DIAG_P2A TX
		/* .DataProvision = */ PDUR_NO_PROVISION,
		/* .DestPduId = */ CANTP_TX_DIAG_P2A,
		/* .TxBufferRef = */ NULL,
		/* .DestModule = */ ARC_PDUR_CANTP
	},
	{ // DIAG_P2A RX
		/* .DataProvision = */ PDUR_NO_PROVISION,
		/* .DestPduId = */ DCM_TX_DIAG_P2A,
		/* .TxBufferRef = */ NULL,
		/* .DestModule = */ ARC_PDUR_DCM
	},
};
const PduRDestPdu_type * const DiagTx_P2P_PduRDestinations[] = {
	&PduR_PduRDestination[0],
	NULL
};
const PduRDestPdu_type * const DiagRx_P2P_PduRDestinations[] = {
	&PduR_PduRDestination[1],
	NULL
};
const PduRDestPdu_type * const DiagTx_P2A_PduRDestinations[] = {
	&PduR_PduRDestination[2],
	NULL
};
const PduRDestPdu_type * const DiagRx_P2A_PduRDestinations[] = {
	&PduR_PduRDestination[3],
	NULL
};

const PduRRoutingPath_type DiagTx_P2P_PduRRoutingPath = {
	/* .SduLength = */ 8,
	/* .TpChunkSize = */0xDEAD,
	/* .PduRDefaultValue = */{0xDB,NULL},
	/* .SrcPduId = */ COM_TX_vEcuC_Pdu_1,
	/* .SrcModule =  */ARC_PDUR_COM,
	/* .PduRDestPdus = */ DiagTx_P2P_PduRDestinations
};
const PduRRoutingPath_type DiagRx_P2P_PduRRoutingPath = {
	/* .SduLength = */ 8,
	/* .TpChunkSize = */0xDEAD,
	/* .PduRDefaultValue = */{0xDB,NULL},
	/* .SrcPduId = */ 0xdd,
	/* .SrcModule =  */ARC_PDUR_CANIF,
	/* .PduRDestPdus = */ DiagRx_P2P_PduRDestinations
};

const PduRRoutingPath_type DiagTx_P2A_PduRRoutingPath = {
	/* .SduLength = */ 8,
	/* .TpChunkSize = */0xDEAD,
	/* .PduRDefaultValue = */{0xDB,NULL},
	/* .SrcPduId = */ DCM_RX_DIAG_P2A,
	/* .SrcModule =  */ARC_PDUR_COM,
	/* .PduRDestPdus = */ DiagTx_P2A_PduRDestinations
};
const PduRRoutingPath_type DiagRx_P2A_PduRRoutingPath = {
	/* .SduLength = */ 8,
	/* .TpChunkSize = */0xDEAD,
	/* .PduRDefaultValue = */{0xDB,NULL},
	/* .SrcPduId = */ CANIF_RX_DIAG_P2A,
	/* .SrcModule =  */ARC_PDUR_CANIF,
	/* .PduRDestPdus = */ DiagRx_P2A_PduRDestinations
};
const PduRRoutingPath_type * const PduRRoutingPaths[] = { 
	&DiagTx_P2P_PduRRoutingPath,
	&DiagRx_P2P_PduRRoutingPath,
	&DiagTx_P2A_PduRRoutingPath,
	&DiagRx_P2A_PduRRoutingPath,
	NULL
};

const PduR_PBConfigType PduR_Config = {
	/* .PduRConfigurationId = */ 0,
	/* .NRoutingPaths = */ 2,
	/* .RoutingPaths = */ PduRRoutingPaths,
	/* .TpBuffers = */ NULL,
	/* .TpRouteBuffers = */ NULL
};

#endif //(PDUR_ZERO_COST_OPERATION == STD_OFF)
