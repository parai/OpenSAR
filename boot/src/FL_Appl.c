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

#include "FL.h"

FL_ResultType Appl_EcuStartup ( void )
{
	return FL_OK;
}


FL_ResultType Appl_FlStartup ( void )
{
	return FL_OK;
}

void Appl_EcuShutdown ( void )
{

}

void Appl_EcuReset ( void )
{

}

FL_ResultType Appl_GetChecksumValues ( FL_BlockNumberType block,FL_DataType* checksum,
		FL_SegmentListType* segments )
{
	return FL_OK;
}

FL_ResultType Appl_CheckConsistency ( void )
{
	return FL_OK;
}

void Appl_UpdateTriggerCondition ( void )
{

}
