#include "Eep.h"
#include "Ea_Cbk.h"

const Eep_ConfigType EepConfigData[1] ={
	{
		.EepBaseAddress = 0,
		.EepDefaultMode = MEMIF_MODE_FAST,
		.EepFastReadBlockSize = 32,
		.EepFastWriteBlockSize = 32,
		.Eep_JobEndNotification = Ea_JobEndNotification,
		.Eep_JobErrorNotification = Ea_JobErrorNotification,
		.EepNormalReadBlockSize = 8,
		.EepNormalWriteBlockSize = 8,
		.EepSize = 16*1024,
		.EepPageSize = 32
	}
};
