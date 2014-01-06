/* -------------------------------- HIS Flash Loader ------------------------------
 *
 * Copyright (C) 2014  parai <parai@foxmail.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- HIS Flash Loader ------------------------------*/

#ifndef SECM_H_
#define SECM_H_

#include "FL.h"

typedef uint8 SecM_StatusType;
#define SECM_OK     ((SecM_StatusType)0)
#define SECM_NOT_OK ((SecM_StatusType)1)

typedef uint32 SecM_WordType;

typedef struct
{
	SecM_WordType seedX;
	SecM_WordType seedY;
}SecM_SeedType;

typedef SecM_WordType SecM_KeyType;

typedef uint32 SecM_CRCType;
typedef uint16 SecM_LengthType;
typedef struct
{
	SecM_CRCType        currentCRC;
	uint8               crcState;
	const uint8*        crcSourceBuffer;
	SecM_LengthType     crcByteCount;
	FL_WDTriggerFctType wdTriggerFct;
}SecM_CRCParamType;

typedef struct
{
	uint32* sigResultBuffer;
	uint32  length ;
}SecM_SignatureType;
typedef struct
{
	SecM_SignatureType currentHash;
	uint32             currentDataLength;
	uint8              sigState;
	uint8*             sigSourceBuffer;
	SecM_LengthType    sigByteCount;
	FL_WDTriggerFctType wdTriggerFct;
}SecM_SignatureParamType;

#endif /* SECM_H_ */
