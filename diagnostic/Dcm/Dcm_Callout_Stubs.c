/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/

/* Ecum Callout Stubs - generic version */

#include "Dcm.h"
#if defined(HIS_FL)
#include "Flash.h"
static tFlashParam FlashParam =
{
    .patchlevel  = FLASH_DRIVER_VERSION_PATCH,
    .minornumber = FLASH_DRIVER_VERSION_MINOR,
    .majornumber = FLASH_DRIVER_VERSION_MAJOR,
    .wdTriggerFct = NULL,
    .errorcode   = kFlashOk,
};
#endif
//#include "Mcu.h"
Dcm_ReturnWriteMemoryType Dcm_WriteMemory(Dcm_OpStatusType OpStatus,
											   uint8 MemoryIdentifier,
											   uint32 MemoryAddress,
											   uint32 MemorySize,
											   uint8* MemoryData)
{
#if defined(HIS_FL)
	FlashParam.address = MemoryAddress;
	FlashParam.length = MemorySize;
	FlashParam.data = MemoryData;
	FLASH_DRIVER_WRITE(FLASH_DRIVER_STARTADDRESS,&FlashParam);
#endif
	//return DCM_WRITE_FAILED;
	return DCM_WRITE_OK;
}

/*@req Dcm495*/
Dcm_ReturnReadMemoryType Dcm_ReadMemory(Dcm_OpStatusType OpStatus,
											   uint8 MemoryIdentifier,
											   uint32 MemoryAddress,
											   uint32 MemorySize,
											   uint8* MemoryData)
{


	//return DCM_WRITE_FAILED;
	return DCM_WRITE_OK;
}

void Dcm_DiagnosticSessionControl(Dcm_SesCtrlType session)
{

}

Std_ReturnType DcmE_EcuReset(Dcm_EcuResetType resetType)
{
	return E_OK;
}

void DcmE_EcuPerformReset(Dcm_EcuResetType resetType)
{
#if defined(USE_MCU) && ( MCU_PERFORM_RESET_API == STD_ON )
	Mcu_PerformReset();
#endif
}
