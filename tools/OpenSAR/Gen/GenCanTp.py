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

__all__ = ['GenCanTp']

__dir = '.'

def GenCanTp(root,dir):
    global __dir
    GLInit(root)
    __dir = '%s'%(dir)
    GenH()
    GenC()
    print '>>> Gen CanTp DONE <<<'
    
def GenH():
    global __dir
    fp = open('%s/CanTp_Cfg.h'%(__dir),'w')
    fp.write(GHeader('CanTp'))
    General=GLGet('General')
    fp.write('#ifndef CANTP_CFG_H_\n#define CANTP_CFG_H_\n\n')
    fp.write('#include "CanTp_Types.h"\n\n')
    fp.write('#define CANTP_MAIN_FUNCTION_PERIOD_TIME_MS    10  /* ms TODO */\n')
    fp.write('#define CANTP_CONVERT_MS_TO_MAIN_CYCLES(x)     (x)/CANTP_MAIN_FUNCTION_PERIOD_TIME_MS\n\n')
    fp.write('#define CANTP_NSDU_CONFIG_LIST_SIZE %s\n'
             %(len(GLGet('RxSduList'))+len(GLGet('TxSduList'))))
    fp.write('#define CANTP_NSDU_RUNTIME_LIST_SIZE %s\n'
             %(len(GLGet('RxSduList'))+len(GLGet('TxSduList'))))
    fp.write('#define CANTP_RXID_LIST_SIZE %s\n\n'
             %(len(GLGet('RxSduList'))+len(GLGet('TxSduList'))))
    fp.write('#define FRTP_CANCEL_TRANSMIT_REQUEST     STD_OFF\n');
    fp.write('#define CANTP_VERSION_INFO_API STD_%s   /**< Build version info API */\n'
             %(GAGet(General,'VersionInfoApi')))
    fp.write('#define CANTP_DEV_ERROR_DETECT STD_%s\n\n'
             %(GAGet(General,'DevelopmentErrorDetection')))
    fp.write('/* CanTp Rx NSdu Id */\n')
    id = 0;
    for obj in GLGet('RxSduList'):
        fp.write('#define CANTP_ID_%-32s %s\n'%(GAGet(obj,'RxPduRef'),id));
        id += 1;
    fp.write('\n/* CanTp Tx NSdu Id */\n')
    for obj in GLGet('TxSduList'):
        fp.write('#define CANTP_ID_%-32s %s\n'%(GAGet(obj,'TxPduRef'),id));
        id += 1;
    fp.write('extern const CanTp_ConfigType CanTpConfig;\n'
            'extern const CanTp_NSduType CanTpNSduConfigList[];\n'
            'extern const CanTp_RxIdType CanTp_RxIdList[];\n')
    fp.write('\n#endif\n\n');
    fp.close()

def GenC():
    global __dir
    fp = open('%s/CanTp_Cfg.c'%(__dir),'w')
    fp.write(GHeader('CanTp'))
    fp.write("""#include "CanTp.h"
#if defined(USE_CANIF)
#include "CanIf.h"
#endif
#if defined(USE_PDUR)
#include "PduR.h"
#endif

const CanTp_GeneralType CanTpGeneralConfig =
{
    .main_function_period =  10,
};
//NSa TODO:
const CanTp_NSaType CanTpNSaConfig =
{
    .CanTpNSa =  0,
};

const CanTp_NTaType CanTpNTaConfig =
{
    .CanTpNTa =  0,
};\n\n""")
    cstr = 'const CanTp_NSduType CanTpNSduConfigList[] =\n{\n'
    for obj in GLGet('RxSduList'):
        cstr +="""
    {
        .direction =  ISO15765_RECEIVE,
        .listItemType =  CANTP_NOT_LAST_ENTRY,
        .configData.CanTpRxNSdu.CanTp_FcPduId =  CANTP_ID_%s,
        .configData.CanTpRxNSdu.CanIf_FcPduId =  CANIF_ID_%s,
        .configData.CanTpRxNSdu.PduR_PduId =  PDUR_ID_%s,
        .configData.CanTpRxNSdu.CanTpAddressingFormant =  CANTP_STANDARD,
        .configData.CanTpRxNSdu.CanTpBs =   %s,
        .configData.CanTpRxNSdu.CanTpNar =  %s,
        .configData.CanTpRxNSdu.CanTpNbr =  %s,
        .configData.CanTpRxNSdu.CanTpNcr =  %s,
        .configData.CanTpRxNSdu.CanTpRxChannel =  CANTP_ID_%s,
        .configData.CanTpRxNSdu.CanTpRxDI =  8,
        .configData.CanTpRxNSdu.CanTpRxPaddingActivation =  CANTP_%s,
        .configData.CanTpRxNSdu.CanTpRxTaType =  CANTP_PHYSICAL,
        .configData.CanTpRxNSdu.CanTpWftMax =  %s,
        .configData.CanTpRxNSdu.CanTpSTmin  =  %s,
        .configData.CanTpRxNSdu.CanTpNSa =  &CanTpNSaConfig,
        .configData.CanTpRxNSdu.CanTpNTa =  &CanTpNTaConfig,
    },\n"""%(GAGet(obj,'RxPduRef'),
           GAGet(obj,'TxFcPduRef'),
           GAGet(obj,'RxPduRef'),
           GAGet(obj,'BS'),
           GAGet(obj,'Nar'),
           GAGet(obj,'Nbr'),
           GAGet(obj,'Ncr'),
           GAGet(obj,'RxPduRef'),
           GAGet(obj,'Padding'),
           GAGet(obj,'MaxBusyTransmission'),
           GAGet(obj,'STmin'))
    Index = -1
    for obj in GLGet('TxSduList'):
        Index += 1
        if(Index == len(GLGet('TxSduList'))):
            isLast='CANTP_END_OF_LIST'
        else:
            isLast='CANTP_NOT_LAST_ENTRY'
        cstr +="""
    {
        .direction =  IS015765_TRANSMIT,
        .listItemType =  %s,
        .configData.CanTpTxNSdu.CanTp_FcPduId =  CANTP_ID_%s,
        .configData.CanTpTxNSdu.CanIf_PduId   =  CANIF_ID_%s,
        .configData.CanTpTxNSdu.PduR_PduId =  PDUR_ID2_%s,
        .configData.CanTpTxNSdu.CanTpAddressingMode =  CANTP_STANDARD,
        .configData.CanTpTxNSdu.CanTpNas =  %s,
        .configData.CanTpTxNSdu.CanTpNbs =  %s,
        .configData.CanTpTxNSdu.CanTpNcs =  %s,
        .configData.CanTpTxNSdu.CanTpTxChannel =  CANTP_ID_%s,
        .configData.CanTpTxNSdu.CanTpTxDI =  8,
        .configData.CanTpTxNSdu.CanTpTxPaddingActivation =  CANTP_%s,
        .configData.CanTpTxNSdu.CanTpTxTaType =  CANTP_PHYSICAL,
        .configData.CanTpTxNSdu.CanTpNSa =  &CanTpNSaConfig,
        .configData.CanTpTxNSdu.CanTpNTa =  &CanTpNTaConfig,
    },\n"""%(isLast,
           GAGet(obj,'RxFcPduRef'),
           GAGet(obj,'TxPduRef'),
           GAGet(obj,'TxPduRef'),
           GAGet(obj,'Nas'),
           GAGet(obj,'Nbs'),
           GAGet(obj,'Ncs'),
           GAGet(obj,'TxPduRef'),
           GAGet(obj,'Padding'))
    cstr += '};\n\n'
    fp.write(cstr)
    cstr = 'const CanTp_RxIdType CanTp_RxIdList[] = \n{\n'
    for obj in GLGet('RxSduList'):
        cstr += """
    {
        .CanTpAddressingMode =  CANTP_STANDARD,
        .CanTpNSduIndex =  CANTP_ID_%s,
        .CanTpReferringTxIndex =  CANTP_ID_%s,
    },\n"""%(GAGet(obj,'RxPduRef'),
           GAGet(obj,'TxFcPduRef'))
    for obj in GLGet('TxSduList'):
        cstr += """
    {
        .CanTpAddressingMode =  CANTP_STANDARD,
        .CanTpNSduIndex =  CANTP_ID_%s,
        .CanTpReferringTxIndex =  CANTP_ID_%s,
    },\n"""%(GAGet(obj,'TxPduRef'),
           GAGet(obj,'RxFcPduRef'),)
    cstr += '};\n\n'
    fp.write(cstr)
    fp.write("""const CanTp_ConfigType CanTpConfig =
{
    .CanTpGeneral     =      &CanTpGeneralConfig,
    .CanTpNSduList =     CanTpNSduConfigList,
    .CanTpRxIdList =    CanTp_RxIdList
};\n\n""")
    fp.close();