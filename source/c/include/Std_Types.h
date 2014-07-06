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
#ifndef STD_TYPES_H_
#define STD_TYPES_H_
/* ============================= [ INCLUDE ] ================================== */
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ============================= [ TYPES ] ==================================== */
typedef unsigned char   	uint8;
typedef unsigned short  	uint16;
typedef unsigned long   	uint32;
typedef unsigned long long 	uint64;		/* this type for 32 bit cpu only */
typedef   signed char   	sint8;
typedef   signed short  	sint16;
typedef   signed long   	sint32;
typedef   signed long long 	sint64;		/* this type for 32 bit cpu only */
typedef   uint8 			Std_ReturnType;

/* ============================= [ MACROS ] =================================== */
#define E_OK 					(Std_ReturnType)0
#define E_NOT_OK 				(Std_ReturnType)1

#define STATIC  static
#define PRIVATE static
#define PUBLIC
#define PROTECT

#endif /* STD_TYPES_H_ */
