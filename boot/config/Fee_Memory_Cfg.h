
#ifndef FEE_MEMORY_CFG_H_
#define FEE_MEMORY_CFG_H_

#if defined(AUTOSAR_SIMULATOR)
// 1M Flash.img
#define FEE_BANK1_OFFSET			   0xF8000
#define FEE_BANK2_OFFSET			   0xFC000
#define FEE_BANK1_LENGTH			   0x4000
#define FEE_BANK2_LENGTH			   0x4000
#else
#define FEE_BANK1_OFFSET			   0x0000
#define FEE_BANK2_OFFSET			   0x4000
#define FEE_BANK1_LENGTH			   0x4000
#define FEE_BANK2_LENGTH			   0x4000
#endif
#endif
