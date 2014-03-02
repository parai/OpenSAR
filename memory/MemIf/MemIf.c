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

/*
 * Note!
 *
 * This file is controlled by config CFG_MEMIF_NO_MACRO and MEMIF_NUMBER_OF_DEVICES .
 */

#include "MemIf.h"
#include <assert.h>

#if (MEMIF_NUMBER_OF_DEVICES > 1) || defined(CFG_MEMIF_NO_MACRO)

void MemIf_SetMode(MemIf_ModeType Mode) {
	//return _MemIf_SetMode(Mode);
	assert(0);
}

Std_ReturnType MemIf_Read(uint8 DeviceIndex, uint16 BlockNumber, uint16 BlockOffset, uint8 *DataBufferPtr, uint16 Length) {
	//return _MemIf_Read(DeviceIndex, BlockNumber , BlockOffset, DataBufferPtr, Length );
	if(FEE_INDEX == DeviceIndex)
	{
		Fee_Read(BlockNumber , BlockOffset, DataBufferPtr, Length );
	}
	else if(EA_INDEX == DeviceIndex)
	{
		Ea_Read(BlockNumber , BlockOffset, DataBufferPtr, Length );
	}
	else
	{
		assert(0);
	}
}

Std_ReturnType MemIf_Write(uint8 DeviceIndex, uint16 BlockNumber, uint8 *DataBufferPtr) {
	//return _MemIf_Write(DeviceIndex, BlockNumber, DataBufferPtr );
	if(FEE_INDEX == DeviceIndex)
	{
		Fee_Write(BlockNumber , DataBufferPtr);
	}
	else if(EA_INDEX == DeviceIndex)
	{
		Ea_Write(BlockNumber , DataBufferPtr);
	}
	else
	{
		assert(0);
	}
}

void MemIf_Cancel(uint8 DeviceIndex) {
	//return _MemIf_Cancel(DeviceIndex);
	if(FEE_INDEX == DeviceIndex)
	{
		Fee_Cancel();
	}
	else if(EA_INDEX == DeviceIndex)
	{
		Ea_Cancel();
	}
	else
	{
		assert(0);
	}
}
MemIf_StatusType MemIf_GetStatus(uint8 DeviceIndex) {
	//return _MemIf_GetStatus(DeviceIndex);
	if(FEE_INDEX == DeviceIndex)
	{
		return Fee_GetStatus();
	}
	else if(EA_INDEX == DeviceIndex)
	{
		return Ea_GetStatus();
	}
	else
	{
		assert(0);
		return MEMIF_UNINIT;
	}
}
MemIf_JobResultType MemIf_GetJobResult(uint8 DeviceIndex) {
	//eturn _MemIf_GetJobResult(DeviceIndex);
	if(FEE_INDEX == DeviceIndex)
	{
		return Fee_GetJobResult();
	}
	else if(EA_INDEX == DeviceIndex)
	{
		return Ea_GetJobResult();
	}
	else
	{
		assert(0);
		return MEMIF_JOB_FAILED;
	}
}
Std_ReturnType MemIf_InvalidateBlock(uint8 DeviceIndex, uint16 BlockNumber) {

	//return _MemIf_InvalidateBlock(DeviceIndex,BlockNumber);
	if(FEE_INDEX == DeviceIndex)
	{
		return Fee_InvalidateBlock(BlockNumber);
	}
	else if(EA_INDEX == DeviceIndex)
	{
		return Ea_InvalidateBlock(BlockNumber);
	}
	else
	{
		assert(0);
		return E_NOT_OK;
	}
}
Std_ReturnType MemIf_EraseImmediateBlock(uint8 DeviceIndex, uint16 BlockNumber) {
	//return _MemIf_EraseImmediateBlock( DeviceIndex, BlockNumber );
	if(FEE_INDEX == DeviceIndex)
	{
		return Fee_EraseImmediateBlock(BlockNumber);
	}
	else if(EA_INDEX == DeviceIndex)
	{
		return Ea_EraseImmediateBlock(BlockNumber);
	}
	else
	{
		assert(0);
		return E_NOT_OK;
	}
}
#endif

