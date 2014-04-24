"""/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
"""
import sys,os
from GCF import *

__all__ = ['GenPduR']

__dir = '.'
    
def GenPduR(root,dir):
    global __dir,__root
    GLInit(root)
    __dir = '%s'%(dir)
    GenH()
    GenC()
    print ">>> Gen PduR DONE <<<"

def isUpperLayer(layer):
    if(layer=='Com' or layer=='Dcm'):
        return True
    return False

def isLowerLayer(layer):
    if(layer=='CanTp' or layer=='CanIf' or 
       layer=='LinTp' or layer=='LinIf'):
        return True
    return False

def GenH():
    global __dir
    # =========================  PduR_Cfg.h ==================
    fp = open('%s/PduR_Cfg.h'%(__dir),'w')
    fp.write(GHeader('PduR'))
    General=GLGet('General')
    fp.write("""
#if !(((PDUR_SW_MAJOR_VERSION == 2) && (PDUR_SW_MINOR_VERSION == 0)) )
#error PduR: Configuration file expected BSW module version to be 2.0.*
#endif

#ifndef PDUR_CFG_H_
#define PDUR_CFG_H_

// Module support
#define PDUR_CANIF_SUPPORT STD_%s
#define PDUR_CANTP_SUPPORT STD_%s
#define PDUR_FRIF_SUPPORT STD_OFF  /* Not supported */
#define PDUR_FRTP_SUPPORT STD_OFF  /* Not supported */
#define PDUR_LINIF_SUPPORT STD_%s
#define PDUR_LINTP_SUPPORT STD_OFF  /* Not supported */
#define PDUR_COM_SUPPORT STD_%s
#define PDUR_DCM_SUPPORT STD_%s
#define PDUR_IPDUM_SUPPORT STD_OFF  /* Not supported */
#define PDUR_J1939TP_SUPPORT STD_%s
#define PDUR_SOAD_SUPPORT STD_OFF  /* Not supported */

#define PDUR_DEV_ERROR_DETECT         STD_%s
#define PDUR_VERSION_INFO_API         STD_%s

// Zero cost operation mode
#define PDUR_ZERO_COST_OPERATION STD_%s
#define PDUR_SINGLE_IF %s
#define PDUR_SINGLE_TP %s\n\n"""%(GAGet(General,'CanIfUsed').replace('True','ON').replace('False','OFF'),
                                  GAGet(General,'CanTpUsed').replace('True','ON').replace('False','OFF'),
                                  GAGet(General,'LinIfUsed').replace('True','ON').replace('False','OFF'),
                                  GAGet(General,'ComUsed').replace('True','ON').replace('False','OFF'),
                                  GAGet(General,'DcmUsed').replace('True','ON').replace('False','OFF'),
                                  GAGet(General,'J1939TpUsed').replace('True','ON').replace('False','OFF'),
                                  GAGet(General,'DevelopmentErrorDetection'),
                                  GAGet(General,'VersionInfoApi'),
                                  GAGet(General,'ZeroCostOperationMode'),
                                  GAGet(General,'ZeroCostIf').upper().replace('CANIF','CAN_IF').replace('LINIF','LIN_IF'),
                                  GAGet(General,'ZeroCostTp').upper().replace('CANTP','CAN_TP').replace('LINTP','LIN_TP'),))
    fp.write("""
// Gateway operation
#define PDUR_GATEWAY_OPERATION                STD_OFF
#define PDUR_MEMORY_SIZE                    10 /* Not used */
#define PDUR_SB_TX_BUFFER_SUPPORT            STD_OFF
#define PDUR_FIFO_TX_BUFFER_SUPPORT            STD_OFF

/* The maximum numbers of Tx buffers. */
#define PDUR_MAX_TX_BUFFER_NUMBER            10 /* Not used */
#define PDUR_N_TP_ROUTES_WITH_BUFFER        0 //"not understand by parai"
#define PDUR_N_TP_BUFFERS                    2

// Multicast,not understand by parai
#define PDUR_MULTICAST_TOIF_SUPPORT            STD_ON
#define PDUR_MULTICAST_FROMIF_SUPPORT        STD_ON
#define PDUR_MULTICAST_TOTP_SUPPORT            STD_ON
#define PDUR_MULTICAST_FROMTP_SUPPORT        STD_ON

// Minimum routing,not understand by parai
/* Minimum routing not supported.
#define PDUR_MINIMUM_ROUTING_UP_MODULE        COM
#define PDUR_MINIMUM_ROUTING_LO_MODULE        CAN_IF
#define PDUR_MINIMUM_ROUTING_UP_RXPDUID        ((PduIdType)100)
#define PDUR_MINIMUM_ROUTING_LO_RXPDUID     ((PduIdType)255)
#define PDUR_MINIMUM_ROUTING_UP_TXPDUID     ((PduIdType)255)
#define PDUR_MINIMUM_ROUTING_LO_TXPDUID     ((PduIdType)255)
*/

#if(PDUR_ZERO_COST_OPERATION == STD_ON)
// Zero cost operation support active.
#if PDUR_CANIF_SUPPORT == STD_ON
#define PduR_CanIfRxIndication Com_RxIndication
#define PduR_CanIfTxConfirmation Com_TxConfirmation
#else
#define PduR_CanIfRxIndication(CanRxPduId,PduInfoPtr)
#define PduR_CanIfTxConfirmation(CanTxPduId)
#endif

#if PDUR_CANTP_SUPPORT == STD_ON
#define PduR_CanTpProvideRxBuffer Dcm_ProvideRxBuffer
#define PduR_CanTpRxIndication Dcm_RxIndication
#define PduR_CanTpProvideTxBuffer Dcm_ProvideTxBuffer
#define PduR_CanTpTxConfirmation Dcm_TxConfirmation
#else
#define PduR_CanTpProvideRxBuffer(id,length,PduInfoPtr)
#define PduR_CanTpRxIndication(CanTpRxPduId,Result)
#define PduR_CanTpProvideTxBuffer(CanTpTxId,PduinfoPtr,Length)
#define PduR_CanTpTxConfirmation(CanTpTxPduId,Result)
#endif

#if PDUR_LINIF_SUPPORT == STD_ON 
#define PduR_LinIfRxIndication Com_RxIndication
#define PduR_LinIfTxConfirmation Com_TxConfirmation
#define PduR_LinIfTriggerTransmit Com_TriggerTransmit
#else
#define PduR_LinIfRxIndication(LinRxPduId,PduInfoPtr)
#define PduR_LinIfTxConfirmation(LinTxPduId)
#define PduR_LinIfTriggerTransmit(LinTxPduId,PduInfoPtr)
#endif

#if PDUR_SOAD_SUPPORT == STD_ON
#define PduR_SoAdTpProvideRxBuffer Dcm_ProvideRxBuffer
#define PduR_SoAdTpRxIndication Dcm_RxIndication
#define PduR_SoAdTpProvideTxBuffer Dcm_ProvideTxBuffer
#define PduR_SoAdTpTxConfirmation Dcm_TxConfirmation
#else
#define PduR_SoAdProvideRxBuffer()
#define PduR_SoAdRxIndication()
#define PduR_SoAdProvideTxBuffer()
#define PduR_SoAdTxConfirmation()
#endif

#if PDUR_COM_SUPPORT == STD_ON
#define PduR_ComTransmit CanIf_Transmit
#else
#define PduR_ComTransmit(CanTxPduId,PduInfoPtr)    (E_OK)
#endif

#if PDUR_DCM_SUPPORT == STD_ON
#define PduR_DcmTransmit CanTp_Transmit
#else
#define PduR_DcmTransmit(CanTpTxSduId,CanTpTxInfoPtr)    (E_OK)
#endif
#endif  /* PDUR_ZERO_COST_OPERATION */

#endif /* PDUR_CFG_H_ */    
    """)
    fp.close()
    
    # =========================  PduR_Cfg.h ==================
    fp = open('%s/PduR_PbCfg.h'%(__dir),'w')
    fp.write(GHeader('PduR'))
    cstr1 = ''
    id = 0
    for path in GLGet('RoutineList'):
        cstr1 += '#define PDUR_ID_%-32s %s\n'%(GAGet(path,'PduRef'),id)
        if(isUpperLayer(GAGet(path,'Module'))):
            cstr1 += '#define PDUR_ID2_%-31s %s\n'%(GAGet(path,'PduRef'),id)
        id += 1
    cstr2 = ''
    for path in GLGet('RoutineList'):
        #if(isUpperLayer(GAGet(path,'Module'))):
        for dest in GLGet(path,'DestinationList'):
            cstr2 += '#define PDUR_ID_%-32s %s_ID_%s\n'%(GAGet(path,'PduRef'),GAGet(dest,'Module').upper(),GAGet(path,'PduRef'))
            break # in ZeroCost Mode, only 1 destination is allowed
        cstr2 += '#define PDUR_ID2_%-31s %s_ID_%s\n'%(GAGet(path,'PduRef'),GAGet(path,'Module').upper(),GAGet(path,'PduRef'))

    fp.write("""
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

#if(PDUR_ZERO_COST_OPERATION == STD_OFF)
%s
#else
%s
#endif

#endif /* PDUR_PB_CFG_H_H */\n\n"""%(cstr1,cstr2))
    fp.close()
    
def GenC():   
    global __dir
    fp = open('%s/PduR_PbCfg.c'%(__dir),'w')
    fp.write(GHeader('PduR'))
    cstr = ''
    for path in GLGet('RoutineList'):
        for dest in GLGet(path,'DestinationList'):
            cstr += """
    {
        .DataProvision =  PDUR_NO_PROVISION,
        .DestPduId =  %s_ID_%s,
        .TxBufferRef =  NULL,
        .DestModule =  ARC_PDUR_%s
    },\n"""%(GAGet(dest,'Module').upper(),GAGet(dest,'PduRef'),
             GAGet(dest,'Module').upper())
    fp.write("""

#include "PduR.h"

#if PDUR_CANIF_SUPPORT == STD_ON
#include "CanIf.h"
#endif
#if PDUR_CANTP_SUPPORT == STD_ON
#include "CanTp.h"
#endif
#if PDUR_LINIF_SUPPORT == STD_ON
#include "LinIf.h"
#endif
#if PDUR_COM_SUPPORT == STD_ON
#include "Com.h"
#endif
#if PDUR_DCM_SUPPORT == STD_ON
#include "Dcm.h"
#endif
#if PDUR_J1939TP_SUPPORT == STD_ON
#include "J1939Tp.h"
#endif
#if(PDUR_ZERO_COST_OPERATION == STD_OFF)
const PduRDestPdu_type PduR_PduRDestination[] = {
%s
};
    """%(cstr))
    cstr = ''
    Index = -1
    for path in GLGet('RoutineList'):
        cstr += 'const PduRDestPdu_type * const %s_PduRDestinations[] = \n{\n'%(GAGet(path,'PduRef'))
        for dest in GLGet(path,'DestinationList'):
            Index += 1
            cstr += '\t&PduR_PduRDestination[%s],\n'%(Index)
        cstr += '\tNULL,\n};\n'
    fp.write(cstr)
    cstr = ''
    for path in GLGet('RoutineList'):
        cstr += """
const PduRRoutingPath_type %s_PduRRoutingPath = {
    .SduLength =  8,
    .SrcPduId =  %s_ID_%s,
    .SrcModule =  ARC_PDUR_%s,
    .PduRDestPdus =  %s_PduRDestinations
};\n"""%(GAGet(path,'PduRef'),
         GAGet(path,'Module').upper(),GAGet(path,'PduRef'),
         GAGet(path,'Module').upper(),
         GAGet(path,'PduRef'))
    fp.write(cstr)
    cstr = ''
    for path in GLGet('RoutineList'):
        cstr += '\t&%s_PduRRoutingPath,\n'%(GAGet(path,'PduRef'))
    fp.write("""
const PduRRoutingPath_type * const PduRRoutingPaths[] = { 
%s
    NULL
};  

const PduR_PBConfigType PduR_Config = {
    .PduRConfigurationId =  0,
    .NRoutingPaths =  %s,       
    .RoutingPaths =  PduRRoutingPaths,
    .TpBuffers =  NULL,
    .TpRouteBuffers =  NULL
};

#endif //(PDUR_ZERO_COST_OPERATION == STD_OFF)  
    \n"""%( cstr, len( GLGet('RoutineList') ) ) )
    fp.close() 