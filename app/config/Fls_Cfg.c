
#include "Fls.h"
#include <stdlib.h>
// ko, this is for simulation running on MinGW
static const Fls_SectorType fls_evbSectorList[] =
{
	{
		.FlsNumberOfSectors = 1024*1024/512,
		.FlsPageSize        = 8,
		.FlsSectorSize      = 512,
		.FlsSectorStartaddress = 0
	}
};
const Fls_ConfigType FlsConfigSet[]=
{
  {
#if ( FLS_AC_LOAD_ON_JOB_START == STD_ON)
    .FlsAcWrite = __FLS_ERASE_RAM__,
    .FlsAcErase = __FLS_WRITE_RAM__,
#else
    .FlsAcWrite = NULL,
    .FlsAcErase = NULL,
#endif
    .FlsJobEndNotification = NULL,
    .FlsJobErrorNotification = NULL,

	.FlsMaxReadFastMode = 16,
	.FlsMaxReadNormalMode = 16,
	.FlsMaxWriteFastMode = 32,
	.FlsMaxWriteNormalMode = 32,

    .FlsSectorList = &fls_evbSectorList[0],
    .FlsSectorListSize = sizeof(fls_evbSectorList)/sizeof(Fls_SectorType),
//    .FlsBlockToPartitionMap = Fls_BlockToPartitionMap,
  }
};
