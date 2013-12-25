
#include "Nm.h"
#include "CanNm.h"
#include "ComM.h"

const Nm_ChannelType Nm_Channels[] = {
	{
		.BusType =  NM_BUSNM_CANNM,
		.BusNmNetworkHandle =  CANNM_CHANNEL_LS,
		.ComMNetworkHandle =  COMM_CHL_LS,
	},
	{
		.BusType =  NM_BUSNM_CANNM,
		.BusNmNetworkHandle =  CANNM_CHANNEL_HS,
		.ComMNetworkHandle =  COMM_CHL_HS,
	}
};

const Nm_ConfigType Nm_Config = 
{
	.Channels =  Nm_Channels
};

