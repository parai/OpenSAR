
#include "Nm.h"
#include "CanNm.h"
#include "ComM.h"

const Nm_ChannelType Nm_Channels[] = {
	{//vNm_Channel_0
		.BusType =  NM_BUSNM_CANNM,
		.BusNmNetworkHandle =  vCanNm_Channel_0,
		.ComMNetworkHandle =  vComM_Channel_0,
	},
};

const Nm_ConfigType Nm_Config = 
{
	.Channels =  Nm_Channels
};

