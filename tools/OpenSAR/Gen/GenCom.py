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
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */
"""
import sys,os
from GCF import *

__all__ = ['GenCom']


__dir = '.'

def GenCom(root,dir):
    global __dir
    GLInit(root)
    __dir = '%s'%(dir)
    GenH()
    GenC()
    print '>>> Gen Com DONE <<<'

def GenH():
    global __dir
    # =========================  PduR_Cfg.h ==================
    fp = open('%s/Com_Cfg.h'%(__dir),'w')
    fp.write(GHeader('Com'))
    sigNbr=sigGrpNbr=0
    for pdu in GLGet('IPduList'):
        sigNbr+=len(GLGet(pdu,'SignalList'))+len(GLGet(pdu,'GroupSignalList'))
        for grpSig in GLGet(pdu,'GroupSignalList'):
            sigGrpNbr+=len(GLGet(grpSig,'SignalList'))
    fp.write("""

#if !(((COM_SW_MAJOR_VERSION == 1) && (COM_SW_MINOR_VERSION == 2)))
#error Com: Configuration file expected BSW module version to be 1.2.*
#endif

#ifndef COM_CFG_H_
#define COM_CFG_H_

#define COM_MODULE_ID 20
#define COM_INSTANCE_ID 1
#define COM_E_INVALID_FILTER_CONFIGURATION 101
#define COM_E_INITIALIZATION_FAILED 102
#define COM_E_INVALID_SIGNAL_CONFIGURATION 103
#define COM_INVALID_PDU_ID 104
#define COM_INVALID_SIGNAL_ID 109
#define COM_ERROR_SIGNAL_IS_SIGNALGROUP 105
#define COM_E_TOO_MANY_IPDU 106
#define COM_E_TOO_MANY_SIGNAL 107
#define COM_E_TOO_MANY_GROUPSIGNAL 108

#define COM_DEV_ERROR_DETECT STD_OFF

#define COM_N_IPDUS          %s
#define COM_N_SIGNALS        %s
#define COM_N_GROUP_SIGNALS  %s

#define ComConfigurationTimeBase 0
#define ComVersionInfoApi

#endif /*COM_CFG_H*/
    """%(len(GLGet('IPduList')),sigNbr,sigGrpNbr))
    fp.close()
    # ====================
    fp = open('%s/Com_PbCfg.h'%(__dir),'w')
    fp.write(GHeader('Com'))
    fp.write("""
#if !(((COM_SW_MAJOR_VERSION == 1) && (COM_SW_MINOR_VERSION == 2)) )
#error Com: Configuration file expected BSW module version to be 1.2.*
#endif

#ifndef COM_PBCFG_H
#define COM_PBCFG_H

#include "Com_Types.h"
extern const Com_ConfigType ComConfiguration;

// PDU group id definitions
#define COM_DEFAULT_IPDU_GROUP  0\n\n""")
    id = 0
    for grp in GLGet('PduGroupList'):
        fp.write('#define COM_IPDU_GROUP_%-32s %s\n'%(GAGet(grp,'Name'),id))
        id += 2
    cstr = ''
    id = 0
    for pdu in GLGet('IPduList'):
        cstr += '#define COM_ID_%-32s %s\n'%(GAGet(pdu,'PduRef'),id)
        id += 1
    fp.write("""
//  COM IPDU Id Defines.
%s \n\n"""%(cstr))
    id1 = id2 = 0
    cstr1 = cstr2 = cstr3 = cstr4 =''
    for pdu in GLGet('IPduList'):
        if(GAGet(pdu,'Callout')!='NULL'):
            cstr4 += 'extern boolean %s(PduIdType, const uint8*);/* %s Callout */\n'%(GAGet(pdu,'Callout'),GAGet(pdu,'Name'))
        for sig in GLGet(pdu,'SignalList'):
            cstr1 += '#define COM_SID_%-32s %s\n'%(GAGet(sig,'Name'),id1)
            id1 += 1
            if(GAGet(pdu,'Direction')=='RECEIVE'):
                if(GAGet(sig,'TimeoutNotification')!='NULL'):
                    cstr3+= 'extern void %s(void); /* %s TimeoutNotification */\n'%(GAGet(sig,'TimeoutNotification'),GAGet(sig,'Name'))
                if(GAGet(sig,'ReceivedNotification')!='NULL'):
                    cstr3+= 'extern void %s(void); /* %s ReceivedNotification */\n'%(GAGet(sig,'ReceivedNotification'),GAGet(sig,'Name'))
        for grpSig in GLGet(pdu,'GroupSignalList'):
            cstr1 += '#define COM_SID_%s %s\n'%(GAGet(grpSig,'Name'),id1)
            id1 += 1
            for sig in GLGet(grpSig,'SignalList'):
                cstr2 += '#define COM_GSID_%-32s %s\n'%(GAGet(sig,'Name'),id2)
                id2 += 1
            if(GAGet(pdu,'Direction')=='RECEIVE'):
                if(GAGet(sig,'TimeoutNotification')!='NULL'):
                    cstr3+= 'extern void %s(void); /* %s TimeoutNotification */\n'%(GAGet(sig,'TimeoutNotification'),GAGet(sig,'Name'))
                if(GAGet(sig,'ReceivedNotification')!='NULL'):
                    cstr3+= 'extern void %s(void); /* %s ReceivedNotification */\n'%(GAGet(sig,'ReceivedNotification'),GAGet(sig,'Name'))
    fp.write("""
//General Signal (Group) Id defines
%s

//Group Signal Id defines
%s

// Notifications
%s

// Callouts
%s
#endif /* COM_PBCFG_H */    
    """%(cstr1,cstr2,cstr3,cstr4))
    fp.close()

def GenC():
    fp = open('%s/Com_PbCfg.c'%(__dir),'w')
    fp.write(GHeader('Com'))
    fp.write("""
#include "Com.h"
#include "Com_Internal.h"
#if defined(USE_PDUR)
#include "PduR.h"
#endif 

//Signal init values.\n""")
    for pdu in GLGet('IPduList'):
        for sig in GLGet(pdu,'SignalList'):
            if(GAGet(sig,'Type')=='uint8' or GAGet(sig,'Type')=='uint16' or GAGet(sig,'Type')=='uint32'):
                fp.write('static const %s %s_InitValue = %s;\n'%(GAGet(sig,'Type'),GAGet(sig,'Name'),
                                                           GAGet(sig,'InitialValue')))
            else:
                fp.write('static const %s %s_InitValue[%s] = {%s};\n'%(GAGet(sig,'Type'),GAGet(sig,'Name'),
                                                                 (Interger(GAGet(sig,'Size'))+7)/8,
                                                                 GAGet(sig,'InitialValue')))
        for gsig in GLGet(pdu,'GroupSignalList'):
            for sig in GLGet(gsig,'SignalList'):
                if(GAGet(sig,'Type')=='uint8' or GAGet(sig,'Type')=='uint16' or GAGet(sig,'Type')=='uint32'):
                    fp.write('static const %s %s_InitValue = %s;\n'%(GAGet(sig,'Type'),GAGet(sig,'Name'),
                                                               GAGet(sig,'InitialValue')))
                else:
                    fp.write('static const %s %s_InitValue[%s] = {%s};\n'%(GAGet(sig,'Type'),GAGet(sig,'Name'),
                                                                 (Interger(GAGet(sig,'Size'))+7)/8,
                                                                 GAGet(sig,'InitialValue')))
    sigNbr=sigGrpNbr=0
    for pdu in GLGet('IPduList'):
        sigNbr+=len(GLGet(pdu,'SignalList'))+len(GLGet(pdu,'GroupSignalList'))
        for grpSig in GLGet(pdu,'GroupSignalList'):
            sigGrpNbr+=len(GLGet(grpSig,'SignalList'))
    cstr1 = cstr2 = ''
    Number = -1
    for pdu in GLGet('IPduList'):
        for gsig in GLGet(pdu,'GroupSignalList'):
            for sig in GLGet(gsig,'SignalList'):
                Number += 1
                if(Number==sigGrpNbr):
                    isEol = 'TRUE'
                else:
                    isEol = 'FALSE'
                cstr1 += """    
    {
        .ComBitPosition= %s,
        .ComBitSize= %s,
        .ComHandleId= COM_ID_%s,
        .ComSignalEndianess= COM_%s,
        .ComSignalInitValue= &%s_InitValue,
        .ComSignalType= %s,
        .Com_Arc_EOL= %s
    },\n"""%(GAGet(sig,'StartBit'),
             GAGet(sig,'Size'),
             GAGet(pdu,'PduRef'),
             GAGet(sig,'Endianess'),
             GAGet(sig,'Name'),
             GAGet(sig,'Type').upper(),
             isEol)
    for pdu in GLGet('IPduList'):
        for gsig in GLGet(pdu,'GroupSignalList'):
            cstr2+='static const ComGroupSignal_type * const %s_GrpSignalRefs[] =\n{\n'%(GAGet(gsig,'Name'))
            for sig in GLGet(gsig,'SignalList'):
                cstr2 += '\t&ComGroupSignal[ COM_GSID_%s ],\n'%(GAGet(sig,'Name'))
            cstr2 += '\tNULL\n};\n'
    if(sigGrpNbr>0):
        fp.write("""
static const ComGroupSignal_type ComGroupSignal[] = {
%s
};
//SignalGroup GroupSignals lists.
%s

//IPdu buffers and signal group buffers\n"""%(cstr1,cstr2)) 
    for pdu in GLGet('IPduList'): 
        fp.write('static uint8 %s_IPduBuffer[8];\n'%(GAGet(pdu,'PduRef')))  
    cstr = ''
    id = 0
    for pdu in GLGet('IPduList'):
        for sig in GLGet(pdu,'SignalList'):
            cstr += """
    {
        #if defined(__GTK__)
        .name = "%s",
        #endif
        .ComBitPosition =  %s,
        .ComBitSize =  %s,
        .ComErrorNotification =  NULL,
        .ComFirstTimeoutFactor =  %s,
        .ComHandleId =  COM_SID_%s,
        .ComNotification =  %s,
        .ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_%s,
        .ComSignalEndianess =  COM_%s, 
        .ComSignalInitValue =  &%s_InitValue,
        .ComSignalType =  %s,
        .ComTimeoutFactor =  %s,
        .ComTimeoutNotification =  %s,
        .ComTransferProperty =  %s,    
        .ComUpdateBitPosition =  %s,         
        .ComSignalArcUseUpdateBit =  %s, 
        .Com_Arc_IsSignalGroup =  FALSE,
        .ComGroupSignal =  NULL,
        .Com_Arc_ShadowBuffer =  NULL,
        .Com_Arc_ShadowBuffer_Mask =  NULL,
        .ComIPduHandleId = COM_ID_%s,
        .Com_Arc_EOL =  FALSE
    },\n"""%(GAGet(sig,'Name'),
             GAGet(sig,'StartBit'),
             GAGet(sig,'Size'),
             GAGet(sig,'FirstTimeoutFactor').replace('TBD','0xDB'),
             GAGet(sig,'Name'),
             GAGet(sig,'ReceivedNotification'),
             GAGet(sig,'TimeoutAction'),
             GAGet(sig,'Endianess'),
             GAGet(sig,'Name'),
             GAGet(sig,'Type').upper(),
             GAGet(sig,'TimeoutFactor').replace('TBD','0xDB'),
             GAGet(sig,'TimeoutNotification'),
             GAGet(sig,'TransferProperty'),
             GAGet(sig,'UpdateBitPosition').replace('TBD','0xDB'),
             GAGet(sig,'UpdateBitUsed').upper(),
             GAGet(pdu,'PduRef')
             )
        for sig in GLGet(pdu,'GroupSignalList'):
            cstr += """
    {
        #if defined(__GTK__)
        .name = "%s",
        #endif
        .ComBitPosition =  %s,
        .ComBitSize =  %s,
        .ComErrorNotification =  NULL,
        .ComFirstTimeoutFactor =  %s,
        .ComHandleId =  COM_SID_%s,
        .ComNotification =  %s,
        .ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_%s,
        .ComSignalEndianess =  COM_%s, 
        .ComSignalInitValue =  NULL,
        .ComSignalType =  UINT8, // For group signal, this means nothing
        .ComTimeoutFactor =  %s,
        .ComTimeoutNotification =  %s,
        .ComTransferProperty =  %s,    
        .ComUpdateBitPosition =  %s,         
        .ComSignalArcUseUpdateBit =  %s, 
        .Com_Arc_IsSignalGroup =  TRUE,
        .ComGroupSignal =  %s_GrpSignalRefs,
        .Com_Arc_ShadowBuffer =  NULL,
        .Com_Arc_ShadowBuffer_Mask =  NULL,
        .ComIPduHandleId = COM_ID_%s,
        .Com_Arc_EOL =  FALSE
    },\n"""%(GAGet(sig,'Name'),
             GAGet(sig,'StartBit'),
             GAGet(sig,'Size'),
             GAGet(sig,'FirstTimeoutFactor').replace('TBD','0xDB'),
             GAGet(sig,'Name'),
             GAGet(sig,'ReceivedNotification'),
             GAGet(sig,'TimeoutAction'),
             GAGet(sig,'Endianess'),
             GAGet(sig,'TimeoutFactor').replace('TBD','0xDB'),
             GAGet(sig,'TimeoutNotification'),
             GAGet(sig,'TransferProperty'),
             GAGet(sig,'UpdateBitPosition').replace('TBD','0xDB'),
             GAGet(sig,'UpdateBitUsed').upper(),
             GAGet(sig,'Name'),
             GAGet(pdu,'PduRef'),
             )
    fp.write("""
//Signal definitions
static const ComSignal_type ComSignal[] = {
%s
    {
         .Com_Arc_EOL =  True,
    }
};\n\n    """%(cstr))
    cstr = ''
    Number = -1
    for grp in GLGet('PduGroupList'):
        Number += 1
        if(Number == len(GLGet('PduGroupList'))):
            isEol = 'TRUE'
        else:
            isEol = 'FALSE'
        cstr += """
    {
        .ComIPduGroupHandleId =  COM_IPDU_GROUP_%s, 
        .Com_Arc_EOL =  %s
    },\n"""%(GAGet(grp,'Name'),isEol)
    fp.write("""
//I-PDU group definitions
static const ComIPduGroup_type ComIPduGroup[] = {
%s
};\n\n"""%(cstr))
    cstr = ''
    for pdu in GLGet('IPduList'):
        cstr += 'static const ComSignal_type * const %s_SignalRefs[] = {\n'%(GAGet(pdu,'PduRef'))
        for sig in GLGet(pdu,'SignalList'):
            cstr += '\t&ComSignal[ COM_SID_%s ],\n'%(GAGet(sig,'Name'))
        for sig in GLGet(pdu,'GroupSignalList'):
            cstr += '\t&ComSignal[ COM_SID_%s ],\n'%(GAGet(sig,'Name'))
        cstr += '\tNULL\n};\n'        
    fp.write("""
//IPdu signal lists.
%s    
    """%(cstr))
    cstr = ''
    for pdu in GLGet('IPduList'):
        cstr += """
    {
        #if defined(__GTK__)
        .name = "%s",
        #endif
        .ComIPduCallout =  %s,
        .ArcIPduOutgoingId =  PDUR_ID_%s,
        .ComIPduSignalProcessing =  %s,
        .ComIPduSize =  8,
        .ComIPduDirection =  %s,
        .ComIPduGroupRef =  COM_IPDU_GROUP_%s,
        .ComTxIPdu ={
            .ComTxIPduMinimumDelayFactor =  %s,
            .ComTxIPduUnusedAreasDefault =  %s,
            .ComTxModeTrue ={
                .ComTxModeMode =   %s,
                .ComTxModeNumberOfRepetitions =   %s,
                .ComTxModeRepetitionPeriodFactor =   %s,
                .ComTxModeTimeOffsetFactor =   %s,
                .ComTxModeTimePeriodFactor =   %s,
            },
        },
        .ComIPduDataPtr =  %s_IPduBuffer,
        .ComIPduDeferredDataPtr =  NULL, // TODO: 
        .ComIPduSignalRef =  %s_SignalRefs,
        .ComIPduDynSignalRef =  NULL,
        .Com_Arc_EOL =  FALSE,
    },\n"""%(GAGet(pdu,'Name'),
             GAGet(pdu,'Callout'),
             GAGet(pdu,'PduRef'),
             GAGet(pdu,'RxSignalProcessing'),
             GAGet(pdu,'Direction'),
             GAGet(pdu,'PduGroupRef'),
             GAGet(pdu,'TxMinimumDelayFactor'),
             GAGet(pdu,'DefaultValueForUnusedArea'),
             GAGet(pdu,'TxMode'),
             GAGet(pdu,'NumberOfRepetions'),
             GAGet(pdu,'RepetionsPeriodFactor'),
             GAGet(pdu,'TimeOffsetFactor'),
             GAGet(pdu,'TimePeriodFactor'),
             GAGet(pdu,'PduRef'),
             GAGet(pdu,'PduRef'))
    
    fp.write("""
//I-PDU definitions
static const ComIPdu_type ComIPdu[] = {
%s
    {
        .Com_Arc_EOL =  TRUE
    }
};

const Com_ConfigType ComConfiguration = {
    .ComConfigurationId =  1,
    .ComIPdu =  ComIPdu,
    .ComIPduGroup =  ComIPduGroup,
    .ComSignal =  ComSignal,
#if(COM_N_GROUP_SIGNALS > 0)
    .ComGroupSignal =  ComGroupSignal
#else
    .ComGroupSignal =  NULL
#endif
};\n\n"""%(cstr))
    cstr = 'static Com_Arc_IPdu_type Com_Arc_IPdu[] = {\n'
    for pdu in GLGet('IPduList'):
        cstr += """
    { // %s
        .Com_Arc_TxIPduTimers ={
            .ComTxIPduNumberOfRepetitionsLeft =  0,
            .ComTxModeRepetitionPeriodTimer =  0,
            .ComTxIPduMinimumDelayTimer =  0,
            .ComTxModeTimePeriodTimer =  0
        },
        .Com_Arc_IpduStarted =  0
    },\n"""%(GAGet(pdu,'PduRef'))
    cstr += '};\n\n'
    cstr += 'Com_Arc_Signal_type Com_Arc_Signal[] = {\n'
    for pdu in GLGet('IPduList'):
        for sig in GLGet(pdu,'SignalList'):
            cstr += """
    { // %s
        .Com_Arc_DeadlineCounter =  0,
        .ComSignalUpdated =  0,
    },\n"""%(GAGet(sig,'Name'))
        for grpSig in GLGet(pdu,'GroupSignalList'):
            cstr += """
    { // %s
        .Com_Arc_DeadlineCounter =  0,
        .ComSignalUpdated =  0,
    },\n"""%(GAGet(sig,'Name'))
    cstr += '};\n\n'   
    fp.write(cstr)
    fp.write("""   
#if(COM_N_GROUP_SIGNALS > 0)
static Com_Arc_GroupSignal_type Com_Arc_GroupSignal[COM_N_GROUP_SIGNALS];
#endif

const Com_Arc_Config_type Com_Arc_Config = {
    .ComIPdu =  Com_Arc_IPdu,
    .ComSignal =  Com_Arc_Signal,
#if(COM_N_GROUP_SIGNALS > 0)
    .ComGroupSignal =  Com_Arc_GroupSignal
#else
    .ComGroupSignal =  NULL
#endif
};    
    """)
    fp.close()
    