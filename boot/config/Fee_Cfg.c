
#include "Fee.h"
#include "NvM_Cbk.h"

const Fee_BlockConfigType BlockConfigList[] = {
	{	// System Time
		.DeviceIndex = 0x00,  // TODO
		.BlockNumber = FEE_BLOCK_NUM_SYSTEM_TIME,
		.BlockSize  =  8,
		.ImmediateData = FALSE,
		.NumberOfWriteCycles = 0xFFFFFFFF
	},
	{
		.DeviceIndex = 0x00,  // TODO
		.BlockNumber = FEE_BLOCK_NUM_TEST1,
		.BlockSize  =  4,
		.ImmediateData = FALSE,
		.NumberOfWriteCycles = 0xFFFFFFFF
	},
	{
		.DeviceIndex = 0x00,  // TODO
		.BlockNumber = FEE_BLOCK_NUM_TEST2,
		.BlockSize  =  4,
		.ImmediateData = FALSE,
		.NumberOfWriteCycles = 0xFFFFFFFF
	}
};
const Fee_ConfigType Fee_Config = {
		.General = {
				.NvmJobEndCallbackNotificationCallback = NULL,
				.NvmJobErrorCallbackNotificationCallback = NULL,
		},
		.BlockConfig = BlockConfigList,
};
