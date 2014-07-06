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

/* ============================= [ INCLUDE ] ================================== */
#include "bitop.h"

/* ============================= [ DATAS ] ==================================== */
/**
 python:
cstr = 'PRIVATE uint8 tableUnMap[256] = {'
for i in range(0,256):
    if(i%16==0):
        cstr += '\n\t'
    for pos in [7,6,5,4,3,2,1,0]:
        if((i&(1<<pos)) != 0):
            cstr += '%s,'%(pos)
            break
        elif(pos==0):
            cstr += '%s,'%(pos)
cstr += '};\n\n'
print cstr
*/
PRIVATE uint8 tableUnMap[256] = {
	0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};
PRIVATE uint8 readyGrp;
PRIVATE uint8 readyTable[8];
/* ============================= [ FUNCTION ] ================================= */
PUBLIC STATIC void Init ( void )
{
	readyGrp = 0;
	memset(readyTable,0,8);
}
PUBLIC STATIC uint8 GetBit ( void  )
{
	uint8 result;
	if( readyGrp != 0x00u)
	{
		uint8 y = tableUnMap[readyGrp];
		uint8 x = tableUnMap[readyTable[y]];
		result  = ((y<<3)+x);
	}
	else
	{
		assert(0);	/* impossible case */
	}

	return result;
}
PUBLIC STATIC void SetBit(uint8 priority)
{
	uint8 y = priority>>3;
	uint8 x = priority&7;
	readyGrp      |= 1<<y;
	readyTable[y] |= 1<<x;
}
PUBLIC STATIC void ClearBit(uint8 priority)
{
	uint8 y = priority>>3;
	uint8 x = priority&7;
	readyTable[y] &= ~(1<<x);
	if(0u == readyTable[y])
	{
		readyGrp &= ~( 1<<y );
	}
}

/* ============================= [ INTERFACE ] ================================ */
const Bitop_Class Bitop = {
	.Init   = Init,
	.GetBit = GetBit,
	.SetBit = SetBit,
	.ClearBit = ClearBit
};

