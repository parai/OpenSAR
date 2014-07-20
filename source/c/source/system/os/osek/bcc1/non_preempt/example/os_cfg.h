/**
 * MicroSAR - the open source AUTOSAR platform https://github.com/parai
 *
 * Copyright (C) 2014  MicroSAR <parai@foxmail.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#ifndef OS_CFG_H_
#define OS_CFG_H_
/* ============================= [ INCLUDE ] ================================== */
#include "OsekOs.h"

#define TASKID_OsIdle		   0
#define TASKID_SchM_Startup    1
#define TASKID_SchM_BswService 2

#define TASK_NUM               3

TASK(SchM_Startup);
TASK(SchM_BswService);
TASK(OsIdle);

#endif /* OS_CFG_H_ */
