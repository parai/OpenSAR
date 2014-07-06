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
#ifndef ECUM_H_
#define ECUM_H_
/* ============================= [ INCLUDE ] ================================== */
#include "Std_Types.h"
/* ============================= [ TYPES ] ==================================== */
typedef struct
{
	PUBLIC void (*Init) (void);
} EcuM_Class;
/* ============================= [ INTERFACE ] ================================ */
extern const EcuM_Class EcuM;


#endif /* ECUM_H_ */
