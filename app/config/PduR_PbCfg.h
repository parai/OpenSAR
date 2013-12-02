

#ifndef PDUR_PB_CFG_H_H
#define PDUR_PB_CFG_H_H
#if !(((PDUR_SW_MAJOR_VERSION == 2) && (PDUR_SW_MINOR_VERSION == 0)) )
#error PduR: Configuration file expected BSW module version to be 2.0.*
#endif

#include "Dcm.h"
#include "Com.h"
#include "CanIf.h"
#include "CanTp.h"

extern const PduR_PBConfigType PduR_Config;
//  PduR Polite Defines.
#define PDUR_TX_DIAG_P2P		0
#define PDUR_RX_DIAG_P2P		1
#define PDUR_TX_DIAG_P2A		2
#define PDUR_RX_DIAG_P2A		3

#define vPduR_SrcPath1 1


#define vPduR_SrcPath0 0
#endif /* PDUR_PB_CFG_H_H */

