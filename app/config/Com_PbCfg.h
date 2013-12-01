
#if !(((COM_SW_MAJOR_VERSION == 1) && (COM_SW_MINOR_VERSION == 2)) )
#error Com: Configuration file expected BSW module version to be 1.2.*
#endif

#ifndef COM_PBCFG_H
#define COM_PBCFG_H

#include "Com_Types.h"

extern const Com_ConfigType ComConfiguration;

// PDU group id definitions
#define vCom_IPduGrp0 0

//  COM IPDU Id Defines.
#define vCom_IPdu1_TX 0
#define COM_TX_vEcuC_Pdu_1 0	/* for vCom_IPdu1_TX */
#define vCom_IPdu1_RX 1
#define COM_RX_vEcuC_Pdu_1 1	/* for vCom_IPdu1_RX */

//General Signal (Group) Id defines
#define vCom_IPdu1_TX_Signal0 0
#define vCom_IPdu1_TX_SigGrp0 1
#define vCom_IPdu1_RX_Signal0 2
#define vCom_IPdu1_RX_Signal1 3
#define vCom_IPdu1_RX_SigGrp0 4

//Group Signal Id defines
#define vCom_IPdu1_TX_SigGrp0_Signal0 0
#define vCom_IPdu1_TX_SigGrp0_Signal1 1
#define vCom_IPdu1_TX_SigGrp0_Signal2 2
#define vCom_IPdu1_TX_SigGrp0_Signal3 3
#define vCom_IPdu1_RX_SigGrp0_Signal0 4
#define vCom_IPdu1_RX_SigGrp0_Signal1 5
#define vCom_IPdu1_RX_SigGrp0_Signal2 6

// Notifications
extern void onSignal0Receive(void);
extern void onSignal1Receive(void);
extern void onSingnalGroup0Receive(void);

// Callouts
#endif /* COM_PBCFG_H */

