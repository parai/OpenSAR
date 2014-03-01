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
 * IMPLEMENTATION NOTES
 * - The SPI implementation only supports 64 bytes in one go so this is
 *   a limitation for the EEP driver also.
 * - The specification if SPI functions should be blocking or not. For now
 *   the driver uses blocking SPI communication.
 *
 * CONFIG NOTES
 *   Look at the 10.4 example in the 4.0 specs, quite good.
 *   Normally the SPI E2 have a number of instructions: READ, WRITE, WRDI, WREN, etc
 *   These instructions have can different sequences. For example WREN can be just
 *   the instruction while WRIE consist of the WRITE instruction, an address and the
 *   data to write.
 *
 *   1. Identify the channels. Best way to do that is to have a look at the E2 instruction
 *      set and its sequences. Example:
 *        READ :  READ| ADDRESS | DATA |
 *         bits    8      16       upto 32*8
 *
 *        WRITE: WRITE | ADDRESS | DATA
 *         bits    8      16       upto 32*8
 *
 *        WREN:   WREN
 *         bits    8
 *
 *        RDSR:    RDSR | DATA
 *                  8       8
 */

/* DEVICE SUPPORT
 *   Microchip:
 *     25LC160
 */

/* REQUIREMENTS
 * - EEP060
 *   Only EEP_WRITE_CYCLE_REDUCTION = STD_OFF is supported
 *
 * - EEP075
 *   MEMIF_COMPARE_UNEQUAL does not exist in the MemIf specification 1.2.1(rel 3.0 )
 *   So, it's not supported. It returns MEMIF_JOB_FAILED instead.
 *
 * - EEP084
 *   EepJobCallCycle not used
 *   We are not using interrupts so EEP_USE_INTERRUPTS must be STD_OFF
 */

#include "Eep.h"
#if defined(USE_DEM)
#include "Dem.h"
#endif
#if defined(USE_DET)
#include "Det.h"
#endif
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//#define USE_LDEBUG_PRINTF	1
#include "debug.h"
#define MODULE_NAME 	"/driver/Eep"

// Define if you to check if the E2 seems sane at init..
#define CFG_EEP_CHECK_SANE    1


/* The width in bytes used by this eeprom */
#define ADDR_LENGTH 	2

/* Helper macro for the process function */
#define SET_STATE(_done,_state) done=(_done);job->state=(_state)

#if ( EEP_DEV_ERROR_DETECT == STD_ON ) // Report DEV errors
#define VALIDATE(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_EEP,0,_api,_err); \
          return; \
        }

#define VALIDATE_W_RV(_exp,_api,_err,_rv ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_EEP,0,_api,_err); \
          return (_rv); \
        }

#define VALID_CHANNEL(_ch)    ( Gpt_Global.configured & (1<<(_ch)) )

#else // Validate but do not report
#define VALIDATE(_exp,_api,_err )\
        if( !(_exp) ) { \
          return; \
        }
#define VALIDATE_W_RV(_exp,_api,_err,_rv )\
        if( !(_exp) ) { \
          return (_rv); \
        }
#endif

#if ( EEP_DEV_ERROR_DETECT == STD_ON )
#define VALIDATE_CONFIG(_x) assert(_x)
#define DET_REPORTERROR(_x,_y,_z,_q) Det_ReportError(MODULE_ID_EEP, _y, _z, _q)
#else
#define VALIDATE_CONFIG(_x)
#define DET_REPORTERROR(_x,_y,_z,_q)
#endif

#define EEP_JOB_END_NOTIFICATION() \
  if (Eep_Global.config->Eep_JobEndNotification!=NULL) { \
    Eep_Global.config->Eep_JobEndNotification(); \
  }

#define EEP_JOB_ERROR_NOTIFICATION() \
  if (Eep_Global.config->Eep_JobErrorNotification!=NULL) { \
    Eep_Global.config->Eep_JobErrorNotification(); \
  }

/* Job state */
typedef enum {
	EEP_NONE, EEP_COMPARE, EEP_ERASE, EEP_READ, EEP_WRITE,
} Eep_Arc_JobType;

/* Spi job state */
typedef enum {
	JOB_MAIN, JOB_READ_STATUS, JOB_READ_STATUS_RESULT,
} Job_StateType;

/* Information about a job */
typedef struct {
	uint8 *targetAddr;
	Eep_AddressType eepAddr;
	uint32 left;
	Job_StateType state;
	Eep_Arc_JobType mainState;
	uint32 chunkSize;
	uint32 pageSize;
	boolean initialOp;
} Eep_JobInfoType;

#define JOB_SET_STATE(_x,_y)		job->state=(_x);job->mainState=(_y)

/*
 * Holds all global information that is needed by the driver
 *
 */
typedef struct {
	// The configuration
	const Eep_ConfigType *config;

	// Status of driver
	MemIf_StatusType status;
	MemIf_JobResultType jobResultType;
	Eep_Arc_JobType jobType;

	// Saved information from API calls.
	Eep_AddressType e2Addr;
	uint8 *targetAddr;
	Eep_LengthType length;
	// What mode we are in ( normal/fast )
	MemIf_ModeType mode;

	// Hold job information
	Eep_JobInfoType job;

} Eep_GlobalType;


Eep_GlobalType Eep_Global;

static void eep_ReadFail( void ) {
	Eep_Global.jobResultType = MEMIF_BLOCK_INCONSISTENT;
	Eep_Global.jobType = EEP_NONE;
	Eep_Global.status = MEMIF_IDLE;
#if defined(USE_DEM)
	Dem_ReportErrorStatus(EEP_E_READ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
	EEP_JOB_ERROR_NOTIFICATION();

}
static void eep_WriteFail( void ) {
	Eep_Global.jobResultType = MEMIF_BLOCK_INCONSISTENT;
	Eep_Global.jobType = EEP_NONE;
	Eep_Global.status = MEMIF_IDLE;
#if defined(USE_DEM)
	Dem_ReportErrorStatus(EEP_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
	EEP_JOB_ERROR_NOTIFICATION();

}
void Eep_Init(const Eep_ConfigType* ConfigPtr) {
	VALIDATE( (ConfigPtr != NULL), EEP_INIT_ID, EEP_E_PARAM_CONFIG);
	VALIDATE( ( Eep_Global.status != MEMIF_BUSY ), EEP_INIT_ID, EEP_E_BUSY);

	Eep_Global.config = ConfigPtr;

	Eep_Global.status = MEMIF_IDLE;
	Eep_Global.jobResultType = MEMIF_JOB_OK;

	Eep_SetMode(Eep_Global.config->EepDefaultMode);

}

void Eep_SetMode(MemIf_ModeType Mode) {
	VALIDATE( ( Eep_Global.status != MEMIF_UNINIT ), EEP_SETMODE_ID, EEP_E_UNINIT);
	VALIDATE( ( Eep_Global.status != MEMIF_BUSY ), EEP_SETMODE_ID, EEP_E_BUSY);

	Eep_Global.mode = Mode;
}

Std_ReturnType Eep_Read(Eep_AddressType EepromAddress, uint8 *TargetAddressPtr, Eep_LengthType Length) {
	Eep_JobInfoType *job = &Eep_Global.job;

	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_UNINIT ), EEP_READ_ID, EEP_E_UNINIT, E_NOT_OK);
	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_BUSY ), EEP_READ_ID, EEP_E_BUSY, E_NOT_OK);
	VALIDATE_W_RV( ( TargetAddressPtr != NULL ), EEP_READ_ID, EEP_E_PARAM_DATA, E_NOT_OK);
	VALIDATE_W_RV( ( (EepromAddress) < (Eep_Global.config->EepSize) ), EEP_READ_ID, EEP_E_PARAM_ADDRESS, E_NOT_OK);
	VALIDATE_W_RV( ( (Eep_Global.config->EepSize - EepromAddress) >= Length ), EEP_READ_ID, EEP_E_PARAM_LENGTH, E_NOT_OK);

	Eep_Global.status = MEMIF_BUSY;
	Eep_Global.jobResultType = MEMIF_JOB_PENDING;
	Eep_Global.jobType = EEP_READ;

	if (Eep_Global.mode == MEMIF_MODE_FAST) {
		job->chunkSize = Eep_Global.config->EepFastReadBlockSize;
	} else {
		job->chunkSize = Eep_Global.config->EepNormalReadBlockSize;
	}

	job->initialOp = true;
	job->eepAddr = EepromAddress + Eep_Global.config->EepBaseAddress;
	job->targetAddr = TargetAddressPtr;
	job->left = Length;

	JOB_SET_STATE(JOB_MAIN, EEP_READ);

	return E_OK;
}

Std_ReturnType Eep_Erase(Eep_AddressType TargetAddress, Eep_LengthType Length) {
	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_UNINIT ), EEP_ERASE_ID, EEP_E_UNINIT, E_NOT_OK);
	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_BUSY ), EEP_ERASE_ID, EEP_E_BUSY, E_NOT_OK);
	VALIDATE_W_RV( ( (TargetAddress) < (Eep_Global.config->EepSize) ), EEP_ERASE_ID, EEP_E_PARAM_ADDRESS, E_NOT_OK);
	VALIDATE_W_RV( ( (Eep_Global.config->EepSize - TargetAddress) >= Length ), EEP_ERASE_ID, EEP_E_PARAM_LENGTH, E_NOT_OK);

	/* TODO : NOT IMPLEMENTED
	 * ( Since this E2 do not have erase )
	 * */
	Std_ReturnType rv = E_NOT_OK;
	Eep_Global.status = MEMIF_BUSY;
	Eep_Global.status = MEMIF_IDLE;
	return rv;
}

Std_ReturnType Eep_Write(Eep_AddressType EepromAddress, const uint8* DataBufferPtr, Eep_LengthType Length) {
	Eep_JobInfoType *job = &Eep_Global.job;

	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_UNINIT ), EEP_WRITE_ID, EEP_E_UNINIT, E_NOT_OK);
	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_BUSY ), EEP_WRITE_ID, EEP_E_BUSY, E_NOT_OK);
	VALIDATE_W_RV( ( DataBufferPtr != NULL ), EEP_WRITE_ID, EEP_E_PARAM_DATA, E_NOT_OK);
	VALIDATE_W_RV( ( (EepromAddress) < (Eep_Global.config->EepSize) ), EEP_WRITE_ID, EEP_E_PARAM_ADDRESS, E_NOT_OK);
	VALIDATE_W_RV( ( Length <= (Eep_Global.config->EepSize - EepromAddress) ), EEP_WRITE_ID, EEP_E_PARAM_LENGTH, E_NOT_OK);

	Eep_Global.jobResultType = MEMIF_JOB_PENDING;
	Eep_Global.status = MEMIF_BUSY;
	Eep_Global.jobType = EEP_WRITE;

	if (Eep_Global.mode == MEMIF_MODE_FAST) {
		job->chunkSize = Eep_Global.config->EepFastWriteBlockSize;
	} else {
		job->chunkSize = Eep_Global.config->EepNormalWriteBlockSize;
	}

	job->initialOp = true;
	job->pageSize = Eep_Global.config->EepPageSize;
	job->eepAddr = EepromAddress  + Eep_Global.config->EepBaseAddress;
	job->targetAddr = (uint8 *) DataBufferPtr;
	job->left = Length;

	JOB_SET_STATE(JOB_MAIN, EEP_WRITE);

	return E_OK;
}

Std_ReturnType Eep_Compare(Eep_AddressType EepromAddress, uint8 *TargetAddressPtr, Eep_LengthType Length) {
	Eep_JobInfoType *job = &Eep_Global.job;

	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_UNINIT ), EEP_COMPARE_ID, EEP_E_UNINIT, E_NOT_OK);
	VALIDATE_W_RV( ( Eep_Global.status != MEMIF_BUSY ), EEP_COMPARE_ID, EEP_E_BUSY, E_NOT_OK);
	VALIDATE_W_RV( ( TargetAddressPtr != NULL ), EEP_COMPARE_ID, EEP_E_PARAM_DATA, E_NOT_OK);
	VALIDATE_W_RV( ( (EepromAddress) < (Eep_Global.config->EepSize) ), EEP_COMPARE_ID, EEP_E_PARAM_ADDRESS, E_NOT_OK);
	VALIDATE_W_RV( ( (Eep_Global.config->EepSize - EepromAddress) >= Length ), EEP_COMPARE_ID, EEP_E_PARAM_LENGTH, E_NOT_OK);

	Eep_Global.status = MEMIF_BUSY;
	Eep_Global.jobResultType = MEMIF_JOB_PENDING;
	Eep_Global.jobType = EEP_COMPARE;

	/* This is a compare job but the compare jobs really issues read in portions
	 * big enough to fit it's static buffers
	 */
	if (Eep_Global.mode == MEMIF_MODE_FAST) {
		job->chunkSize = Eep_Global.config->EepFastReadBlockSize;
	} else {
		job->chunkSize = Eep_Global.config->EepNormalReadBlockSize;
	}

	job->initialOp = true;
	job->pageSize = Eep_Global.config->EepPageSize; // Not relevant to compare/read operations, but set anyways.
	job->eepAddr = EepromAddress  + Eep_Global.config->EepBaseAddress;
	job->targetAddr = TargetAddressPtr;
	job->left = Length;

	JOB_SET_STATE(JOB_MAIN, EEP_COMPARE);

	return E_OK;
}

void Eep_Cancel(void) {
	EEP_JOB_ERROR_NOTIFICATION();

	if (MEMIF_JOB_PENDING == Eep_Global.jobResultType) {
		Eep_Global.jobResultType = MEMIF_JOB_CANCELLED;
	}

	Eep_Global.status = MEMIF_IDLE;
}

MemIf_StatusType Eep_GetStatus(void) {
	return Eep_Global.status;
}

MemIf_JobResultType Eep_GetJobResult(void) {
	return Eep_Global.jobResultType;
}

#define EEPROM_IMG "Eeprom.img"

void Eep_MainFunction(void) {
	FILE* fp = NULL;
	int ercd;
	int result;
	uint32 chunkSize;

	if (Eep_Global.jobResultType == MEMIF_JOB_PENDING) {
		switch (Eep_Global.jobType) {
		case EEP_COMPARE:
			chunkSize = MIN( Eep_Global.job.left, Eep_Global.job.chunkSize );
			fp = fopen(EEPROM_IMG,"r");
			if(NULL != fp)
			{
				void* buffer = malloc(chunkSize);
				fseek(fp,Eep_Global.job.eepAddr,SEEK_SET);
				fread(buffer,chunkSize,1,fp);
				fclose(fp);

				/** @req FLS244 */
				result = memcmp((void *)Eep_Global.job.targetAddr,(void *)buffer, chunkSize );
				free(buffer);
				Eep_Global.job.targetAddr += chunkSize;
				Eep_Global.job.eepAddr    += chunkSize;
				Eep_Global.job.left       -= chunkSize;
				ercd = E_OK;
			}
			else
			{
				ercd = E_NOT_OK;
			}
			if( E_NOT_OK == ercd ){
				eep_ReadFail();
			} else {
				if(result != 0)
				{
					Eep_Global.jobResultType = MEMIF_BLOCK_INCONSISTENT;
					Eep_Global.status = MEMIF_IDLE;
					Eep_Global.jobType = EEP_NONE;
				}
				else if( 0 == Eep_Global.job.left ) {
					Eep_Global.jobResultType = MEMIF_JOB_OK;
					Eep_Global.status = MEMIF_IDLE;
					Eep_Global.jobType = EEP_NONE;
				} else {
					/* Do nothing, wait for next loop */
				}
			}
			break;
		case EEP_ERASE:
			break; // no need
		case EEP_READ:
			chunkSize = MIN( Eep_Global.job.left, Eep_Global.job.chunkSize );

			fp = fopen(EEPROM_IMG,"r");
			if(NULL != fp)
			{
				void* buffer = malloc(chunkSize);
				fseek(fp,Eep_Global.job.eepAddr,SEEK_SET);
				fread(buffer,chunkSize,1,fp);
				fclose(fp);

				/** @req FLS244 */
				memcpy( (void *)Eep_Global.job.targetAddr, (void *) buffer, chunkSize );
				free(buffer);
				Eep_Global.job.targetAddr += chunkSize;
				Eep_Global.job.eepAddr    += chunkSize;
				Eep_Global.job.left       -= chunkSize;
				ercd = E_OK;
			}
			else
			{
				ercd = E_NOT_OK;
			}

			if( E_NOT_OK == ercd ){
				eep_ReadFail();
			} else {
				if( 0 == Eep_Global.job.left ) {
					Eep_Global.jobResultType = MEMIF_JOB_OK;
					Eep_Global.status = MEMIF_IDLE;
					Eep_Global.jobType = EEP_NONE;
					EEP_JOB_END_NOTIFICATION();
				}
			}
			break;
		case EEP_WRITE:
			chunkSize = MIN( Eep_Global.job.left, Eep_Global.job.chunkSize );

			if (Eep_Global.job.left == 0) {
				/* Done! */
				Eep_Global.jobResultType = MEMIF_JOB_OK;
				Eep_Global.status = MEMIF_IDLE;
				Eep_Global.jobType = EEP_NONE;
				EEP_JOB_END_NOTIFICATION();
				break;
			}

			fp = fopen(EEPROM_IMG,"r+");
			if(NULL != fp)
			{
				fseek(fp,Eep_Global.job.eepAddr,SEEK_SET);
				fwrite((void*)(Eep_Global.job.targetAddr),chunkSize,1,fp);
				Eep_Global.job.targetAddr += chunkSize;
				Eep_Global.job.eepAddr    += chunkSize;
				Eep_Global.job.left       -= chunkSize;
				fclose(fp);
			}
			else
			{
				eep_WriteFail();
			}
			break;
		case EEP_NONE:
			assert(0);
			break;
		}
	}
}
