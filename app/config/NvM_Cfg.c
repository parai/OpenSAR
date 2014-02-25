
#include "NvM.h"
#include "Fee.h"

static uint8 RamData1[4];
static uint8 RamData2[4];
static uint8 RamData3[8];
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
		.NvBlockNum = FEE_BLOCK_NUM_Time,
		.NvramDeviceId = 0,
		.NvBlockBaseNumber = FEE_BLOCK_NUM_Time,
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
		.NvBlockNum = FEE_BLOCK_NUM_Config,
		.NvramDeviceId = 0,
		.NvBlockBaseNumber = FEE_BLOCK_NUM_Config,
		.InitBlockCallback = NULL,
	},
	{
		.BlockManagementType = NVM_BLOCK_NATIVE,
		.SelectBlockForReadall = TRUE,
		.SingleBlockCallback = NULL,
		.NvBlockLength        = 8,
		.BlockUseCrc  = FALSE,
		.BlockCRCType =NVM_CRC16,
		.RamBlockDataAddress = RamData3,
		.CalcRamBlockCrc = TRUE,
		.NvBlockNum = FEE_BLOCK_NUM_FingerPrint,
		.NvramDeviceId = 0,
		.NvBlockBaseNumber = FEE_BLOCK_NUM_FingerPrint,
		.InitBlockCallback = NULL,
	},
};

const NvM_ConfigType NvM_Config = {
		.Common = {
				.MultiBlockCallback = NULL,
		},
		.BlockDescriptor = BlockDescriptorList,		
};
