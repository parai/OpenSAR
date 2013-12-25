
#include "CanNm.h"
#include "CanIf.h"
#include "Nm.h"

const CanNm_ChannelType  CanNm_Channels[] = {
	{
		.Active =  TRUE,
		.NmNetworkHandle =  CANNM_CHANNEL_LS,
		.NodeId =  0x00,
		.MainFunctionPeriod =  10,
		.TimeoutTime =  200,
		.RepeatMessageTime =  500,
		.WaitBusSleepTime =  5000,
		.MessageCycleTime =  500,
		.MessageCycleOffsetTime =  100,
		.CanIfPduId =  CANIF_LS_NM_TX,
		.PduLength =  8,
		.NidPosition =  CANNM_PDU_BYTE_1,
		.CbvPosition =  CANNM_PDU_BYTE_0
	},
	{
		.Active =  TRUE,
		.NmNetworkHandle =  CANNM_CHANNEL_HS,
		.NodeId =  0x01,
		.MainFunctionPeriod =  10,
		.TimeoutTime =  200,
		.RepeatMessageTime =  500,
		.WaitBusSleepTime =  5000,
		.MessageCycleTime =  500,
		.MessageCycleOffsetTime =  100,
		.CanIfPduId =  CANIF_HS_NM_TX,
		.PduLength =  8,
		.NidPosition =  CANNM_PDU_BYTE_1,
		.CbvPosition =  CANNM_PDU_BYTE_0
	}
};

const CanNm_ConfigType CanNm_Cfg = {
	.Channels =  CanNm_Channels
};

 
