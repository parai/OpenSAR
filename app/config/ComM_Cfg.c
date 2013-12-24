
#include "ComM.h"
#include "Nm.h"
#include "CanSM.h"

const ComM_ChannelType ComM_Channels[] = 
{
	{
		.BusType =   COMM_BUS_TYPE_CAN,
		.BusSMNetworkHandle =   0,
		.NmChannelHandle =   0x000000,
		.NmVariant =   COMM_NM_VARIANT_FULL,
		.MainFunctionPeriod =   4,
		.LightTimeout =   1000,
		.Number =   0
	},
};

const ComM_ChannelType* vComM_User_0_ChannleList[] = 
{
	&ComM_Channels[0],
};
const ComM_UserType ComM_Users[] =
{
	{//vComM_User_0
		.ChannelList =  vComM_User_0_ChannleList,
		.ChannelCount =  1
	},
};

const ComM_ConfigType ComM_Cfg =
{
    .Channels =  ComM_Channels,
    .Users =  ComM_Users
};

