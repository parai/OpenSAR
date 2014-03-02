
#ifndef MEMIF_CFG_H_
#define MEMIF_CFG_H_

#include "MemIf_Types.h"

#if defined(USE_FEE)
#include "Fee.h"
#endif

#if defined(USE_EA)
#include "Ea.h"
#endif


#define MEMIF_VERSION_INFO_API    		STD_ON		// Activate/Deactivate ver info API.
#define MEMIF_DEV_ERROR_DETECT			STD_ON		// Activate/Deactivate Dev Error Detection and Notification.

#define MEMIF_NUMBER_OF_DEVICES			2			// Number of underlying memory abstraction modules.

#if (MEMIF_NUMBER_OF_DEVICES == 1)
#define MEMIF_DEVICE_TO_USE				FLS_DRIVER_INDEX
#endif
#endif /*MEMIF_CFG_H_*/
