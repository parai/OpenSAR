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

#ifndef SECM_C_
#define SECM_C_
#include "SecM.h"

SecM_StatusType SecM_GenerateSeed(SecM_SeedType *seed)
{
	return SECM_OK;
}

SecM_StatusType SecM_ComputeKey(SecM_SeedType seed,SecM_WordTypek,SecM_KeyType*key)
{
	return SECM_OK;
}

SecM_StatusType SecM_CompareKey(SecM_KeyType key,SecM_SeedType seed)
{
	return SECM_OK;
}

SecM_StatusType SecM_ComputeCRC(SecM_CRCParamType *crcParam)
{
	return SECM_OK;
}

SecM_StatusType SecM_VerifySignature(SecM_SignatureParamType *signatureParam)
{
	return SECM_OK;
}


#endif /* SECM_C_ */
