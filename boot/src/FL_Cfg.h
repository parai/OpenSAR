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

#ifndef FL_CFG_H_
#define FL_CFG_H_

// Address in FEE: TODO
#define FL_BOOT_MODE     			??
#define FL_APPL_UPDATE   			??

// @FL-702
#define FL_BLK_START_ADDRESS        ??
#define FL_BLK_SIZE                 ??
#define FL_BLK_MAX_PROG_ATTEMPTS  	??

#define FL_SLEEP_TIMER   ((uint16)30)     // 30s
#define FL_RESET_TIMER   ((uint16)5000)  // 5000ms

#define FL_NUM_LOGICAL_BLOCKS  (1)
#define FL_MAX_SEGMENTS        (1)
#define FL_NUM_DEVICES         (1)

#define FL_USE_GAP_FILL        (FALSE)
#define FL_GAP_FILL_VALUE      ((uint8)0xFF)

#define FL_USE_EMBEDDED_CHECKSUM_VALUES  (FALSE)

#define FL_DRV_CHUNKS        (1)
#define FL_DEV_BASE_ADDRESS  ((uint32)0x00000000)
#define FL_DEV_SIZE          ((uint32)0x000EFFFF)




#endif /* FL_CFG_H_ */

