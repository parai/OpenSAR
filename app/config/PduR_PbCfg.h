

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
/* PduR Rx NSdu Id */
#define PDUR_RX_vEcuC_Pdu_1 1
#define vPduR_SrcPath1 1
/* PduR Tx NSdu Id */
#define PDUR_TX_vEcuC_Pdu_1 0
#define PDUR_ALT_TX_vEcuC_Pdu_1 0
#define vPduR_SrcPath0 0
#endif /* PDUR_PB_CFG_H_H */

