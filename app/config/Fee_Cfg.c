
#include "Fee.h"
#include "NvM_Cbk.h"

const Fee_BlockConfigType BlockConfigList[] = {
};
const Fee_ConfigType Fee_Config = {
		.General = {
				.NvmJobEndCallbackNotificationCallback = NULL,
				.NvmJobErrorCallbackNotificationCallback = NULL,
		},
		.BlockConfig = BlockConfigList,
};
