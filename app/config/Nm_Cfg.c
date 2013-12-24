
#include "Nm.h"
#include "CanNm.h"
#include "ComM.h"

const Nm_ChannelType Nm_Channels[] = {
	{//vNm_Channel_0
		.BusType =  NM_BUSNM_CANNM,
		.BusNmNetworkHandle =  CANNM_CHANNEL_LS,
		.ComMNetworkHandle =  vComM_Channel_0,
	},
};

const Nm_ConfigType Nm_Config = 
{
	.Channels =  Nm_Channels
};

