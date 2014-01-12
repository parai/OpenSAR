
#include "NvM.h"
#include "Fee.h"

static uint8 RamData1[4];
static uint8 RamData2[4];
const NvM_BlockDescriptorType BlockDescriptorList[] = {
	{
		.BlockManagementType = NVM_BLOCK_NATIVE,
		.SelectBlockForReadall = TRUE,
		.SingleBlockCallback = NULL,
		.NvBlockLength        = 4,
		.BlockUseCrc  = FALSE,
		.BlockCRCType =NVM_CRC16,
		.RamBlockDataAddress = RamData1,
		.CalcRamBlockCrc = TRUE,
		.NvBlockNum = FEE_BLOCK_NUM_TEST1,
		.NvramDeviceId = 0,
		.NvBlockBaseNumber = FEE_BLOCK_NUM_TEST1,
		.InitBlockCallback = NULL,
	},
	{
		.BlockManagementType = NVM_BLOCK_NATIVE,
		.SelectBlockForReadall = TRUE,
		.SingleBlockCallback = NULL,
		.NvBlockLength        = 4,
		.BlockUseCrc  = FALSE,
		.BlockCRCType =NVM_CRC16,
		.RamBlockDataAddress = RamData2,
		.CalcRamBlockCrc = TRUE,
		.NvBlockNum = FEE_BLOCK_NUM_TEST2,
		.NvramDeviceId = 0,
		.NvBlockBaseNumber = FEE_BLOCK_NUM_TEST2,
		.InitBlockCallback = NULL,
	}

};

const NvM_ConfigType NvM_Config = {
		.Common = {
				.MultiBlockCallback = NULL,
		},
		.BlockDescriptor = BlockDescriptorList,		
};
