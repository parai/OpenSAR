
#include "ComM.h"
#include "Nm.h"
#include "CanSM.h"

const ComM_ChannelType ComM_Channels[] = 
{
	{
		.BusType =   COMM_BUS_TYPE_CAN,
		.BusSMNetworkHandle =   CANSM_CHL_LS,
		.NmChannelHandle =   NM_CHL_LS,
		.NmVariant =   COMM_NM_VARIANT_FULL,
		.MainFunctionPeriod =   10,
		.LightTimeout =   1000,
		.Number =   0
	},
	{
		.BusType =   COMM_BUS_TYPE_CAN,
		.BusSMNetworkHandle =   CANSM_CHL_HS,
		.NmChannelHandle =   NM_CHL_HS,
		.NmVariant =   COMM_NM_VARIANT_FULL,
		.MainFunctionPeriod =   10,
		.LightTimeout =   1000,
		.Number =   1
	},
};

const ComM_ChannelType* ComMLsUser_ChannleList[] =
{
	&ComM_Channels[0],
};
const ComM_ChannelType* ComMHsUser_ChannleList[] =
{
	&ComM_Channels[1],
};
const ComM_UserType ComM_Users[] =
{
	{//COMM_LS_USER
		.ChannelList =  ComMLsUser_ChannleList,
		.ChannelCount =  1
	},
	{//COMM_HS_USER
		.ChannelList =  ComMHsUser_ChannleList,
		.ChannelCount =  1
	},
};

const ComM_ConfigType ComM_Cfg =
{
    .Channels =  ComM_Channels,
    .Users =  ComM_Users
};

