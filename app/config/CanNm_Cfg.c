
#include "CanNm.h"
#include "CanIf.h"
#include "Nm.h"

const CanNm_ChannelType  CanNm_Channels[] = {
	{
		.Active =  TRUE,
		.NmNetworkHandle =  0x0000,
		.NodeId =  0xA5,
		.MainFunctionPeriod =  10,
		.TimeoutTime =  1000,
		.RepeatMessageTime =  1000,
		.WaitBusSleepTime =  1000,
		.MessageCycleTime =  1000,
		.MessageCycleOffsetTime =  100,
		.CanIfPduId =  0x000000,
		.PduLength =  8,
		.NidPosition =  CANNM_PDU_BYTE_1,
		.CbvPosition =  CANNM_PDU_BYTE_0
	},
};

const CanNm_ConfigType CanNm_Config = {
	.Channels =  CanNm_Channels
};

 
