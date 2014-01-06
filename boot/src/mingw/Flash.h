/* -------------------------------- Flash Loader ------------------------------
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
 * -------------------------------- Flash Loader ------------------------------*/
#ifndef FLASH_H
#define FLASH_H

/* software version information - only an example!!! */
#define FLASH_DRIVER_VERSION_MAJOR 0x01 /* major version number / interface */
#define FLASH_DRIVER_VERSION_MINOR 0x01 /* minor version number / internal */
#define FLASH_DRIVER_VERSION_PATCH 0x00 /* bugfix / patchlevel */


#define FLASH_DRIVER_INIT_OFFSET    0x00000004
#define FLASH_DRIVER_DEINIT_OFFSET  0x00000008
#define FLASH_DRIVER_ERASE_OFFSET   0x0000000C
#define FLASH_DRIVER_WRITE_OFFSET   0x00000010

/* sa holds flash driver start address, 
 * pp contains pointer the pointer to the parameter structure */
#define FLASH_DRIVER_INIT(sa, pp)     (*(tFlashFct*) ((sa)+FLASH_DRIVER_INIT_OFFSET))  ((pp))
#define FLASH_DRIVER_DEINIT(sa, pp)   (*(tFlashFct*) ((sa)+FLASH_DRIVER_DEINIT_OFFSET))((pp))
#define FLASH_DRIVER_ERASE(sa, pp)    (*(tFlashFct*) ((sa)+FLASH_DRIVER_ERASE_OFFSET)) ((pp))
#define FLASH_DRIVER_WRITE(sa, pp)    (*(tFlashFct*) ((sa)+FLASH_DRIVER_WRITE_OFFSET)) ((pp))

/* standard error codes */
#define kFlashOk             0x00 /* called function succeeded */
#define kFlashFailed         0x01 /* called function failed */
/* controller specific error codes - only an example!!! */
#define kFlashInvalidParam 	 0x02
#define kFlashInvalidAddress 0x03
#define kFlashInvalidSize    0x04
#define kFlashInvalidData    0x05
#define kFlashNoProgClock    0x06
#define kFlashNoProgVoltage  0x07
#define kFlashPViol          0x08
#define kFlashAccerr         0x09

typedef unsigned long tFlashAddress;
typedef unsigned long tFlashLength;

typedef unsigned char tFlashData;

typedef unsigned char tMajorVersion; /* flash driver major version number */
typedef unsigned char tMinorVersion; /* flash driver minor version number */
typedef unsigned char tBugfixVersion; /* bugfix / patchlevel number */
typedef unsigned short tFlashResult;
/* result of flash driver routines */
typedef unsigned long tAddress;
/* logical address */
typedef unsigned long tLength;
/* length (in bytes) */
typedef unsigned char tData;
/* ambiguous data */
typedef void (*tWDTriggerFct) (void);
/* watchdog trigger routine */

typedef struct
{
	/* initialization: inout parameters */
	tBugfixVersion patchlevel;  /* flash driver patch level version */
	tMajorVersion  minornumber; /* flash driver minor version number */
	tMinorVersion  majornumber; /* flash driver major version number */
	unsigned char  reserved1;   /* reserved for future use, set to 0x00 for now */
	
	/* retrun value / error code: output parameters */
	tFlashResult   errorcode;   /* return value / error code */
	unsigned short reserved2;   /* reserved for future use, set to 0x0000 for now */
	
	/* erase / write: input parameters */
	tAddress      address; /* logical target address */
	tLength       length;  /* lenght information (in bytes) */
	tData*        data;    /* pointer to data buffer */
	/* additional input parameters */
	tWDTriggerFct wdTriggerFct;  /* pointer to watchdog trigger routine */
	/* erase / write: optional output parameters: debugging information */
	tData    intendedData[2];   /* intented data at error address */
	tData    actualData[2];     /* actual data at error address */
	tAddress errorAddress;   /* address of error */
	/* additonal controller / hardware specific parameters */
	/* --> to be specified by flash driver vendor */
} tFlashParam;
typedef void (*tFlashFct) ( tFlashParam* ); /* prototype of flash driver routine */

typedef struct 
{
	union{
		unsigned long w;
		struct
		{
			unsigned MCU      :8; // LSB
			unsigned mask     :8;
			unsigned reserved :8;
			unsigned version  :8; // MSB
		}W;
	}Info;
	tFlashFct Init;
	tFlashFct Deinit;
	tFlashFct Erase;
	tFlashFct Write;
}tFlashHeader;
// ========================== Config Start ==================================
#define FLASH_DRIVER_RELOCATABLE
#define FLASH_DRIVER_STARTADDRESS   ((unsigned long)(&FlashHeader))
#define FLASH_DRIVER_NUMBER_OF_MODULES 1
// ========================== config  End  ==================================

extern tFlashHeader FlashHeader;

extern void FlashInit(tFlashParam* FlashParam);
extern void FlashDeinit(tFlashParam* FlashParam);
extern void FlashErase(tFlashParam* FlashParam);
extern void FlashWrite(tFlashParam* FlashParam);
#endif 
