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

// see HIS Flashloader Specification v1.1.pdf

#ifndef FL_H_
#define FL_H_

#include "Std_Types.h"

typedef uint8 FL_ResultType;
#define FL_OK      ((FL_ResultType)0)
#define FL_FAILED  ((FL_ResultType)0)

typedef uint8 FL_ExtProgRequestType;
#define FL_EXT_PROG_REQUEST_RECEIVED ((FL_ExtProgRequestType)0x2a)

typedef uint8 FL_ApplicationUpdateType;
#define FL_APPL_UPDATED   ((FL_ApplicationUpdateType)0xd5)

typedef uint8  FL_BlockNumberType;
typedef uint32 FL_AddressType;
typedef uint32 FL_LengthType;
typedef uint8  FL_DataType;

typedef struct
{
	FL_BlockNumberType blockNumber;
	FL_AddressType     address;
	FL_LengthType      length;
}FL_BlockDescriptorType; // @SecM240

typedef struct
{
	FL_AddressType transferredAddress;
	FL_AddressType targetAddress;
	FL_LengthType  length;
}FL_SegmentInfoType;

typedef struct
{
	uint8 nrOfSegments;
	FL_SegmentInfoType* segmentInfo;
}FL_SegmentListType;

typedef void (*FL_WDTriggerFctType)(void);

#endif /* FL_H_ */
