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
__all__ = ['GenDcm']

AlreadySecurityRef = []
AlreadySessionRef = []
__dir = '.'

def GenDcm(root,dir):
    global __dir,AlreadySecurityRef,AlreadySessionRef
    GLInit(root)
    AlreadySecurityRef = []
    AlreadySessionRef = []
    __dir = '%s'%(dir)
    GenH()
    GenC()
    print '>>> Gen Dcm DONE <<<'

def GenH():
    global __dir
    fp = open('%s/Dcm_Cfg.h'%(__dir),'w')
    fp.write(GHeader('Dcm'))
    General= GLGet('General')
    fp.write('#ifndef DCM_CFG_H_\n#define DCM_CFG_H_\n\n');
    #-------------------------------------------------------
    fp.write('#define DCM_VERSION_INFO_API              STD_%s\n'%(GAGet(General,'VersionInfoApi')))
    fp.write('#define DCM_DEV_ERROR_DETECT              STD_%s\n'%(GAGet(General,'DevelopmentErrorDetection')))
    fp.write('#define DCM_RESPOND_ALL_REQUEST           STD_ON  // Activate/Deactivate response on SID 0x40-0x7f and 0xc0-0xff.\n')
    fp.write('#define DCM_REQUEST_INDICATION_ENABLED    STD_ON  // Activate/Deactivate indication request mechanism.\n')
    fp.write('#define DCM_PAGEDBUFFER_ENABLED           STD_OFF // Enable/disable page buffer mechanism (currently only disabled supported)\n\n')
    fp.write('#define DCM_DSL_BUFFER_LIST_LENGTH        %s\n'%(len(GLGet('BufferList'))))
    lengthRx = lengthTx =0;
    for protocol in GLGet('ProtocolList'):
        for connection in GLGet(protocol,'ConnectionList'):
            lengthRx += len(GLGet(connection,'RxChannelList'));
            lengthTx += len(connection)-1;
    fp.write('#define DCM_DSL_TX_PDU_ID_LIST_LENGTH        %s\n'%(lengthTx));
    fp.write('#define DCM_DSL_RX_PDU_ID_LIST_LENGTH        %s\n\n'%(lengthRx));
    fp.write('#define DCM_MAIN_FUNCTION_PERIOD_TIME_MS     10\n\n');
    fp.write('#define DCM_LIMITNUMBER_PERIODDATA        %s   // MaxNumberofSimultaneousPeriodictransmissions\n'%(GAGet(General,'MaxPeriodDIDNumber')));
    fp.write('#define DCM_MAX_DDDSOURCE_NUMBER          8   // TODO: MaxSourcesforOneDynamicIdentifier\n');
    fp.write('#define DCM_MAX_DDD_NUMBER                8   // TODO\n\n');
    fp.write('#define DCM_PERIODICTRANSMIT_SLOW            %s\n'%(GAGet(General,'PeriodicDIDSlowModeTime')));
    fp.write('#define DCM_PERIODICTRANSMIT_MEDIUM          %s\n'%(GAGet(General,'PeriodicDIDMediumModeTime')));
    fp.write('#define DCM_PERIODICTRANSMIT_FAST            %s\n\n'%(GAGet(General,'PeriodicDIDFastModeTime')));
    id = 0;
    for protocol in GLGet('ProtocolList'):
        for connection in GLGet(protocol,'ConnectionList'):
            for rx in GLGet(connection,'RxChannelList'):
                fp.write('#define DCM_ID_%s %s\n'%(GAGet(rx,'PduRef'),id));
                id += 1;
    fp.write('\n')
    id = 0;
    for protocol in GLGet('ProtocolList'):
        for connection in GLGet(protocol,'ConnectionList'):
            tx = GLGet(connection,'TxChannel')
            if(tx != None):
                fp.write('#define DCM_ID_%s %s\n'%(GAGet(tx,'PduRef'),id));
                id += 1;
    fp.write("""//do add/subtract by hand.please
//#define USE_DEM
#define DCM_USE_SERVICE_DIAGNOSTICSESSIONCONTROL
#define DCM_USE_SERVICE_ECURESET
//#define DCM_USE_SERVICE_CLEARDIAGNOSTICINFORMATION
//#define DCM_USE_SERVICE_READDTCINFORMATION
#define DCM_USE_SERVICE_READDATABYIDENTIFIER
#define DCM_USE_SERVICE_READMEMORYBYADDRESS
#define DCM_USE_SERVICE_WRITEMEMORYBYADDRESS
#define DCM_USE_SERVICE_READSCALINGDATABYIDENTIFIER
#define DCM_USE_SERVICE_SECURITYACCESS
#define DCM_USE_SERVICE_WRITEDATABYIDENTIFIER
#define DCM_USE_SERVICE_ROUTINECONTROL
#define DCM_USE_SERVICE_TESTERPRESENT
//#define DCM_USE_SERVICE_CONTROLDTCSETTING
#define DCM_USE_SERVICE_READDATABYPERIODICIDENTIFIER
#define DCM_USE_SERVICE_DYNAMICALLYDEFINEDATAIDENTIFIER
#define DCM_USE_SERVICE_INPUTOUTPUTCONTROLBYIDENTIFIER
""")
    #-------------------------------------------------------
    fp.write('#endif /*DCM_CFG_H_*/\n\n')
    fp.close();

def GenC():
    fp = open('%s/Dcm_LCfg.c'%(__dir),'w')
    fp.write(GHeader('Dcm'))
    fp.write("""#include "Std_Types.h"
#include "Dcm.h"
#include "Dcm_Internal.h"
#if defined(USE_PDUR)
#include "PduR.h"
#endif\n\n""");
    Index = 0
    for sec in GLGet('SecurityList'):
        fp.write('#define INDEX_OF_SECURITY_%-32s %s\n'%(GAGet(sec,'Name'),Index));
        Index += 1
    fp.write('#define DCM_SECURITY_EOL_INDEX %s\n\n'%(len(GLGet('SecurityList'))));
    Index = 0
    for ses in GLGet('SessionList'):
        fp.write('#define INDEX_OF_SESSION_%-32s %s\n'%(GAGet(ses,'Name'),Index));
        Index += 1
    fp.write('#define DCM_SESSION_EOL_INDEX %s\n\n'%(len(GLGet('SessionList'))));
    Index = 0
    for didInfo in GLGet('DIDInfoList'):
        fp.write('#define INDEX_OF_DIDINFO_%-32s %s\n'%(GAGet(didInfo,'Name'),Index))
        Index += 1
    fp.write('#define DCM_DID_LIST_EOL_INDEX %s\n\n'%(len(GLGet('DIDList'))));
    Index = 0
    for rtninfo in GLGet('RoutineInfoList'):
        fp.write('#define INDEX_OF_RCINFO_%-32s %s\n'%(GAGet(rtninfo,'Name'),Index))
        Index += 1
    fp.write('\n')
    Index = 0
    for ll in GLGet('BufferList'):
        fp.write('#define INDEX_OF_BUF_%-32s %s\n'%(GAGet(ll,'Name'),Index))
        Index += 1
    fp.write('\n')
    Index = 0
    for ll in GLGet('TimingList'):
        fp.write('#define INDEX_OF_TL_%-32s %s\n'%(GAGet(ll,'Name'),Index))
        Index += 1
    fp.write('\n')    
    Index = 0
    for ll in GLGet('ServiceTableList'):
        fp.write('#define INDEX_OF_ST_%-32s %s\n'%(GAGet(ll,'Name'),Index))
        Index += 1
    fp.write('\n')     
    for did in GLGet('DIDList'):
        if(GAGet(did,'ReadDataLengthCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint16 *didLength);\n'%(GAGet(did,'ReadDataLengthCbk')))
        if(GAGet(did,'ReadConditionCheckCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'ReadConditionCheckCbk')))
        if(GAGet(did,'ReadCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *data);\n'%(GAGet(did,'ReadCbk')))
        if(GAGet(did,'WriteConditionCheckCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'WriteConditionCheckCbk')))
        if(GAGet(did,'WriteCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *data, uint16 dataLength, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'WriteCbk')))
        if(GAGet(did,'GetScalingInfoCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *scalingInfo, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'GetScalingInfoCbk')))
        if(GAGet(did,'FreezeCurrentStateCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'FreezeCurrentStateCbk')))
        if(GAGet(did,'ResetToDefaultCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'ResetToDefaultCbk')))
        if(GAGet(did,'ReturnControlToEcu') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'ReturnControlToEcu')))
        if(GAGet(did,'ShortTermAdjustment') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(did,'ShortTermAdjustment')))
    fp.write('/* --------------------------------------------------------- */\n\n')
    AlreadyL = []
    for sec in GLGet('SecurityList'):
        if(GAGet(sec,'GetSeedCallback') != 'NULL'):
            try:
                index = AlreadyL.index(GAGet(sec,'GetSeedCallback'))
            except:
                AlreadyL.append(GAGet(sec,'GetSeedCallback'))
                fp.write('extern Std_ReturnType %s (uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(sec,'GetSeedCallback')));
        if(GAGet(sec,'CompareKeyCallback') != 'NULL'):
            try:
                index = AlreadyL.index(GAGet(sec,'CompareKeyCallback'))
            except:
                AlreadyL.append(GAGet(sec,'CompareKeyCallback'))
                fp.write('extern Std_ReturnType %s (uint8 *key);\n'%(GAGet(sec,'CompareKeyCallback')));
    for req in GLGet('RequestServiceList'):
        if(GAGet(req,'StartProtocolCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s (Dcm_ProtocolType protocolID);\n'%(GAGet(req,'StartProtocolCbk')))
        if(GAGet(req,'StopProtocolCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s (Dcm_ProtocolType protocolID);\n'%(GAGet(req,'StopProtocolCbk')))
        if(GAGet(req,'ProtocolIndicationCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *requestData, uint16 dataSize);\n\n'%(GAGet(req,'ProtocolIndicationCbk')));
    for sesc in GLGet('SessionControlList'):
        if(GAGet(sesc,'GetSessionChangePermissionCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(Dcm_SesCtrlType sesCtrlTypeActive, Dcm_SesCtrlType sesCtrlTypeNew);\n'%(GAGet(sesc,'GetSessionChangePermissionCbk')));
    for rtn in GLGet('RoutineList'):
        if(GAGet(rtn,'StartRoutineCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(rtn,'StartRoutineCbk')));
        if(GAGet(rtn,'StopRoutineCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(rtn,'StopRoutineCbk')));
        if(GAGet(rtn,'RoutineResultCbk') != 'NULL'):
            fp.write('extern Std_ReturnType %s(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);\n'%(GAGet(rtn,'RoutineResultCbk')));
    # -=-=-==------- Security Level
    cstr = 'const Dcm_DspSecurityRowType DspSecurityList[] = {\n'
    for sec in GLGet('SecurityList'):
        cstr += '\t{ // %s\n'%(GAGet(sec,'Name'));
        cstr += '\t\t.DspSecurityLevel =  %s,\n'%(GAGet(sec,'Identifier'));
        cstr += '\t\t.DspSecurityDelayTimeOnBoot =  0,/* Value is not configurable */\n';
        cstr += '\t\t.DspSecurityNumAttDelay =  0,     /* Value is not configurable */\n';
        cstr += '\t\t.DspSecurityDelayTime =  0,     /* Value is not configurable */\n';
        cstr += '\t\t.DspSecurityNumAttLock =  0,     /* Value is not configurable */\n';
        cstr += '\t\t.DspSecurityADRSize =  0, // TODO:\n';
        cstr += '\t\t.DspSecuritySeedSize =  %s,\n'%(GAGet(sec,'SeedSize'));
        cstr += '\t\t.DspSecurityKeySize =  %s,\n'%(GAGet(sec,'KeySize'));
        cstr += '\t\t.GetSeed =  %s,\n'%(GAGet(sec,'GetSeedCallback'));
        cstr += '\t\t.CompareKey =  %s,\n'%(GAGet(sec,'CompareKeyCallback'));
        cstr += '\t\t.Arc_EOL =  FALSE\n';
        cstr += '\t},\n';
    cstr += '\t{ \n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n';
    cstr += '};\n\n'
    fp.write(cstr);
    fp.write("""const Dcm_DspSecurityType DspSecurity = {
    .DspSecurityRow = DspSecurityList
};\n\n""");
    #-------------------------- Session -------------
    cstr = 'const Dcm_DspSessionRowType DspSessionList[] = {\n';
    for ses in GLGet('SessionList'):
        cstr +='\t{ //%s\n'%(GAGet(ses,'Name'));
        cstr +='\t\t.DspSessionLevel = %s,\n'%(GAGet(ses,'Identifier'));
        cstr +='\t\t.DspSessionP2ServerMax = %s,\n'%(GAGet(ses,'P2ServerMaxTimeMs'));
        cstr +='\t\t.DspSessionP2StarServerMax =  %s,\n'%(GAGet(ses,'P2StartServerMaxTimeMs'));
        cstr +='\t\t.Arc_EOL =  %s\n'%('FALSE');
        cstr +='\t},\n';
    cstr +='\t{ \n'
    cstr +='\t\t.Arc_EOL = %s\n'%('TRUE');
    cstr +='\t},\n';
    cstr += '};\n\n';
    fp.write(cstr);
    fp.write("""const Dcm_DspSessionType DspSession = {
    .DspSessionRow = DspSessionList,
};\n\n""")
    #----------------- Service Table ---------------
    for sertbl in GLGet('ServiceTableList'):
        for ser in GLGet(sertbl,'ServiceList'):
            GenSecurityRef(fp,GLGet(ser,'SecurityList'))
    for sertbl in GLGet('ServiceTableList'):
        for ser in GLGet(sertbl,'ServiceList'):
            GenSessionRef(fp,GLGet(ser,'SessionList'))
    fp.write('\n')
    #------------------DID Control Record
    if(len(GLGet('DIDControlRecordList'))>0):
        for rec in GLGet('DIDControlRecordList'):
            fp.write('const Dcm_DspDidControlRecordSizesType %s_SizeInfo = {\n'%(GAGet(rec,'Name')));
            fp.write('\t.DspDidControlEnableMaskRecordSize =  %s,\n'%(GAGet(rec,'EnableMaskSize')))
            fp.write('\t.DspDidControlOptionRecordSize = %s,\n'%(GAGet(rec,'OptionSize')))
            fp.write('\t.DspDidControlStatusRecordSize = %s,\n'%(GAGet(rec,'StatusSize')))
            fp.write('};\n')
    #----------------- DID INFOs ----------------
    for didInfo in GLGet('DIDInfoList'):
        #-------------- read access
        if(GLGet(didInfo,'ReadAccess') != []):
            ReadAccess = GLGet(didInfo,'ReadAccess')
            str1 = str2 = 'NULL';
            if(len(GLGet(ReadAccess,'SessionList'))):
                GenSessionRef(fp,GLGet(ReadAccess,'SessionList'))
                str1 = GetSessionRefName(GLGet(ReadAccess,'SessionList'))
            #----
            if(len(GLGet(ReadAccess,'SecurityList'))):
                GenSecurityRef(fp,GLGet(ReadAccess,'SecurityList'))
                str2 = GetSecurityRefName(GLGet(ReadAccess,'SecurityList'))
            fp.write("""const Dcm_DspDidReadType %s_didRead = {
    .DspDidReadSessionRef =  %s,
    .DspDidReadSecurityLevelRef =  %s
};\n\n"""%(GAGet(didInfo,'Name'), str1, str2));
        #----------- write access ------
        if(GLGet(didInfo,'WriteAccess') != []):
            WriteAccess = GLGet(didInfo,'WriteAccess')
            str1 = str2 = 'NULL';
            if(len(GLGet(WriteAccess,'SessionList'))):
                GenSessionRef(fp,GLGet(WriteAccess,'SessionList'))
                str1 = GetSessionRefName(GLGet(WriteAccess,'SessionList'))
            #----
            if(len(GLGet(WriteAccess,'SecurityList'))):
                GenSecurityRef(fp,GLGet(WriteAccess,'SecurityList'))
                str2 = GetSecurityRefName(GLGet(WriteAccess,'SecurityList'))
            fp.write("""const Dcm_DspDidWriteType %s_didWrite = {
    .DspDidWriteSessionRef =  %s,
    .DspDidWriteSecurityLevelRef =  %s
};\n\n"""%(GAGet(didInfo,'Name'), str1, str2));
        #----------- Control access -----------------
        if(GLGet(didInfo,'ControlAccess') != []):
            ControlAccess = GLGet(didInfo,'ControlAccess')
            str1 = str2 = 'NULL';
            if(len(GLGet(ControlAccess,'SessionList'))):
                GenSessionRef(fp,GLGet(ControlAccess,'SessionList'))
                str1 = GetSessionRefName(GLGet(ControlAccess,'SessionList'))
            #----
            if(len(GLGet(ControlAccess,'SecurityList'))):
                GenSecurityRef(fp,GLGet(ControlAccess,'SecurityList'))
                str2 = GetSecurityRefName(GLGet(ControlAccess,'SecurityList'))
            str3 = GAGet(ControlAccess,'FreezeCurrentStateControlRef');
            if(str3 != 'TBD'):
                str3 = '&%s_SizeInfo'%(str3);
            else:
                str3 = 'NULL'
            str4 = GAGet(ControlAccess,'ResetToDefaultRef')
            if(str4 != 'TBD'):
                str4 = '&%s_SizeInfo'%(str4);
            else:
                str4 = 'NULL'
            str5 = GAGet(ControlAccess,'ReturnControlToEcuRef')
            if(str5 != 'TBD'):
                str5 = '&%s_SizeInfo'%(str5);
            else:
                str5 = 'NULL'
            str6 = GAGet(ControlAccess,'ShortTermAdjustmentRef')
            if(str6 != 'TBD'):
                str6 = '&%s_SizeInfo'%(str6);
            else:
                str6 = 'NULL'
            fp.write("""const Dcm_DspDidControlType %s_didControl = {
    .DspDidControlSessionRef =  %s,
    .DspDidControlSecurityLevelRef =  %s,
    .DspDidFreezeCurrentState =  %s,
    .DspDidResetToDefault =  %s,
    .DspDidReturnControlToEcu =  %s,
    .DspDidShortTermAdjustment =  %s
};\n\n"""%(GAGet(didInfo,'Name'), str1, str2, str3, str4, str5, str6));
    cstr = 'const Dcm_DspDidInfoType DspDidInfoList[] = {\n';
    for didInfo in GLGet('DIDInfoList'):
        cstr += '\t{ // %s\n'%(GAGet(didInfo,'Name'));
        cstr += '\t\t.DspDidDynamicllyDefined =  %s,\n'%(GAGet(didInfo,'DynamicDefined'));
        cstr += '\t\t.DspDidFixedLength =  %s,\n'%(GAGet(didInfo,'FixedLength'));
        cstr += '\t\t.DspDidScalingInfoSize =  %s,\n'%(GAGet(didInfo,'ScalingInfoSize'));
        cstr += '\t\t.DspDidAccess = {\n';
        if(GLGet(didInfo,'ReadAccess') != []):
            cstr += '\t\t\t.DspDidRead = &%s_didRead,\n'%(GAGet(didInfo,'Name'));
        else:
            cstr += '\t\t\t.DspDidRead = NULL,\n';
        if(GLGet(didInfo,'WriteAccess') != []):
            cstr += '\t\t\t.DspDidWrite = &%s_didWrite,\n'%(GAGet(didInfo,'Name'));
        else:
            cstr += '\t\t\t.DspDidWrite = NULL,\n';
        if(GLGet(didInfo,'ControlAccess') != []):
            cstr += '\t\t\t.DspDidControl = &%s_didControl,\n'%(GAGet(didInfo,'Name'));
        else:
            cstr += '\t\t\t.DspDidControl = NULL,\n';
        cstr += '\t\t}\n';
        cstr += '\t}, \n';
    cstr += '};\n\n'
    fp.write(cstr);
    #------------------ DIDs ----------
    fp.write('extern const Dcm_DspDidType DspDidList[];\n');
    for did in GLGet('DIDList'):
        fp.write("""const Dcm_DspDidType* %s_dididRefList[] =
{    // TODO
&DspDidList[DCM_DID_LIST_EOL_INDEX]  //add did ref by hand please,If you need it
};\n"""%(GAGet(did,'Name')));
    cstr = 'const Dcm_DspDidType DspDidList[] = { \n';
    for did in GLGet('DIDList'):
        cstr += '\t{ // %s,\n'%(GAGet(did,'Name'));
        cstr += '\t\t.DspDidUsePort =  FALSE,/* Value is not configurable */\n'
        cstr += '\t\t.DspDidIdentifier =  %s,\n'%(GAGet(did,'Identifier'))
        cstr += '\t\t.DspDidInfoRef =  &DspDidInfoList[INDEX_OF_DIDINFO_%s],\n'%(GAGet(did,'DIDInfoRef'));
        cstr += '\t\t.DspDidRef =  %s_dididRefList,\n'%(GAGet(did,'Name'));
        cstr += '\t\t.DspDidSize =  %s,\n'%(GAGet(did,'Size').replace('TBD','0xDB/*Dynamic*/'));
        cstr += '\t\t.DspDidReadDataLengthFnc =  %s,\n'%(GAGet(did,'ReadDataLengthCbk'));
        cstr += '\t\t.DspDidConditionCheckReadFnc =  %s,\n'%(GAGet(did,'ReadConditionCheckCbk'));
        cstr += '\t\t.DspDidReadDataFnc = %s,\n'%(GAGet(did,'ReadCbk'));
        cstr += '\t\t.DspDidConditionCheckWriteFnc =  %s,\n'%(GAGet(did,'WriteConditionCheckCbk'));
        cstr += '\t\t.DspDidWriteDataFnc =  %s,\n'%(GAGet(did,'WriteCbk'));
        cstr += '\t\t.DspDidGetScalingInfoFnc =  %s,\n'%(GAGet(did,'GetScalingInfoCbk'));
        cstr += '\t\t.DspDidFreezeCurrentStateFnc = %s,\n'%(GAGet(did,'FreezeCurrentStateCbk'));
        cstr += '\t\t.DspDidResetToDefaultFnc =  %s,\n'%(GAGet(did,'ResetToDefaultCbk'));
        cstr += '\t\t.DspDidReturnControlToEcuFnc =  %s,\n'%(GAGet(did,'ReturnControlToEcu'));
        cstr += '\t\t.DspDidShortTermAdjustmentFnc =  %s,\n'%(GAGet(did,'ShortTermAdjustment'));
        #TODO: ?? what does DspDidControlRecordSize means
        cstr += '\t\t.DspDidControlRecordSize =  NULL,\n';
        cstr += '\t\t.Arc_EOL =  %s\n'%('FALSE');
        cstr += '\t},\n'
    cstr += '\t{ '
    cstr += '\t\t.Arc_EOL =  %s\n'%('TRUE');
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    #---------------------- Routines -------
    fp.write("""/************************************************************************
*                            Routine control                                *
************************************************************************/\n\n""");
    for rtninfo in GLGet('RoutineInfoList'):
        start = GLGet(rtninfo,'Start')
        if(start != []):
            fp.write("""const Dcm_DspStartRoutineType %s_start = {
    .DspStartRoutineCtrlOptRecSize = %s,
    .DspStartRoutineStsOptRecSize =  %s
};\n"""%(GAGet(rtninfo,'Name'), GAGet(start,'RecordSizeOfRequest'), GAGet(start,'RecordSizeOfResponse')));
        stop = GLGet(rtninfo,'Start')
        if(stop != []):
            fp.write("""const Dcm_DspRoutineStopType %s_stop = {
    .DspStopRoutineCtrlOptRecSize = %s,
    .DspStopRoutineStsOptRecSize = %s,
};\n"""%(GAGet(rtninfo,'Name'), GAGet(stop,'RecordSizeOfRequest'), GAGet(stop,'RecordSizeOfResponse')));
        rsl = GLGet(rtninfo,'Result')
        if(rsl != []):
            fp.write("""const Dcm_DspRoutineRequestResType %s_result = {
    .DspReqResRtnCtrlOptRecSize = %s
};\n"""%(GAGet(rtninfo,'Name'), GAGet(rsl,'RecordSizeOfResponse')))
        # sss 
        GenSessionRef(fp,GLGet(rtninfo,'SessionList'))
        GenSecurityRef(fp,GLGet(rtninfo,'SecurityList'))
        
    cstr = 'const Dcm_DspRoutineInfoType DspRoutineInfoList[] = {\n'
    for rtninfo in GLGet('RoutineInfoList'):
        str1= GetSessionRefName(GLGet(rtninfo,'SessionList'))
        str2= GetSecurityRefName(GLGet(rtninfo,'SecurityList'))
        
        cstr += '\t{//%s\n'%(GAGet(rtninfo,'Name'));
        cstr += '\t\t.DspRoutineAuthorization={\n'
        cstr += '\t\t\t.DspRoutineSessionRef =  %s,\n'%(str1)
        cstr += '\t\t\t.DspRoutineSecurityLevelRef = %s,\n'%(str2)
        cstr += '\t\t},\n'
        cstr += '\t\t.DspStartRoutine = &%s_start,\n'%(GAGet(rtninfo,'Name'));
        if(GLGet(rtninfo,'Stop') != []):
            cstr += '\t\t.DspRoutineStop = &%s_stop,\n'%(GAGet(rtninfo,'Name'));
        else:
            cstr += '\t\t.DspRoutineStop =  NULL,\n'
        if(GLGet(rtninfo,'Result') != []):
            cstr += '\t\t.DspRoutineRequestRes = &%s_result,\n'%(GAGet(rtninfo,'Name'));
        else:
            cstr += '\t\t.DspRoutineRequestRes =  NULL\n'
        cstr += '\t},\n'
    cstr += '};\n\n'
    fp.write(cstr);
    cstr = 'const Dcm_DspRoutineType  DspRoutineList[] = {\n'
    for rtn in GLGet('RoutineList'):
        cstr += '\t{//%s\n'%(GAGet(rtn,'Name'));
        cstr += '\t\t.DspRoutineUsePort = %s,\n'%('FALSE')
        cstr += '\t\t.DspRoutineIdentifier = %s,\n'%(GAGet(rtn,'Identifier'))
        cstr += '\t\t.DspRoutineInfoRef = &DspRoutineInfoList[INDEX_OF_RCINFO_%s],\n'%(GAGet(rtn,'RoutineInfoRef'))
        cstr += '\t\t.DspStartRoutineFnc = %s,\n'%(GAGet(rtn,'StartRoutineCbk'))
        cstr += '\t\t.DspStopRoutineFnc =  %s,\n'%(GAGet(rtn,'StopRoutineCbk'))
        cstr += '\t\t.DspRequestResultRoutineFnc =  %s,\n'%(GAGet(rtn,'RoutineResultCbk'))
        cstr += '\t\t.Arc_EOL = FALSE\n'
        cstr += '\t},\n'
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    #--------------------- Memory ------------------------------------
    fp.write("""/************************************************************************
*                            Memory Info                                 *
************************************************************************/\n\n""")
    #---------------------- DSP
    fp.write("""const Dcm_DspType Dsp = {
    .DspMaxDidToRead =  0xDB, // TODO
    .DspDid =  DspDidList,
    .DspDidInfo = DspDidInfoList,
    .DspEcuReset = NULL,
    .DspPid =  NULL,
    .DspReadDTC =  NULL,
    .DspRequestControl =  NULL,
    .DspRoutine = DspRoutineList,
    .DspRoutineInfo = DspRoutineInfoList,
    .DspSecurity =  &DspSecurity,
    .DspSession =  &DspSession,
    .DspTestResultByObdmid =  NULL,
    .DspVehInfo = NULL
};\n\n""");
    # ------------------------------- DSD
    fp.write("""/************************************************************************
*                                    DSD                                    *
************************************************************************/\n\n""");
    for sertbl in GLGet('ServiceTableList'):
        for ser in GLGet(sertbl,'ServiceList'):
            GenSecurityRef(fp,GLGet(ser,'SecurityList'))
            GenSessionRef(fp,GLGet(ser,'SessionList'))
    for sertbl in GLGet('ServiceTableList'):
        cstr = 'const Dcm_DsdServiceType %s_serviceList[] = {\n'%(GAGet(sertbl,'Name'));
        for ser in GLGet(sertbl,'ServiceList'):
            cstr += '\t{ \n'
            cstr += '\t\t.DsdSidTabServiceId = SID_%s,\n'%(GAGet(ser,'Name'));
            cstr += '\t\t.DsdSidTabSubfuncAvail = %s,\n'%(GAGet(ser,'SubFunctionSupported').upper());
            cstr += '\t\t.DsdSidTabSecurityLevelRef = %s,\n'%(GetSecurityRefName(GLGet(ser,'SecurityList')));
            cstr += '\t\t.DsdSidTabSessionLevelRef = %s,\n'%(GetSessionRefName(GLGet(ser,'SessionList')));
            cstr += '\t\t.Arc_EOL = FALSE\n'
            cstr += '\t},\n';
        cstr += '\t{ \n'
        cstr += '\t\t.Arc_EOL = TRUE\n'
        cstr += '\t}\n';
        cstr += '};\n\n'
        fp.write(cstr);
    cstr = 'const Dcm_DsdServiceTableType DsdServiceTable[] = {    \n';
    id = 0;
    for sertbl in GLGet('ServiceTableList'):
        cstr += '\t{ // %s\n'%(GAGet(sertbl,'Name'));
        cstr += '\t\t.DsdSidTabId = %s,\n'%(id);
        cstr += '\t\t.DsdService = %s_serviceList,\n'%(GAGet(sertbl,'Name'));
        cstr += '\t\t.Arc_EOL = FALSE\n'
        cstr += '\t},\n'
        id += 1;
    cstr += '\t{ \n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    fp.write("""const Dcm_DsdType Dsd = {
    .DsdServiceTable = DsdServiceTable
};\n\n""");
    #----------------------- DSL
    fp.write("""/************************************************************************
*                                    DSL                                    *
************************************************************************/\n\n""")
    #--------------- buffer 
    cstr = 'const Dcm_DslBufferType DcmDslBufferList[DCM_DSL_BUFFER_LIST_LENGTH] = {\n'
    id = 0;
    for buf in GLGet('BufferList'):
        fp.write("""uint8 %s[%s];
Dcm_DslBufferRuntimeType rxBufferParams_%s =
{
    .status =  NOT_IN_USE
};\n"""%(GAGet(buf,'Name'), GAGet(buf,'Size'), GAGet(buf,'Name')));
        cstr += '\t{\n'
        cstr += '\t\t.DslBufferID =  %s,// TODO\n'%(id);
        cstr += '\t\t.DslBufferSize = %s,/* ?Value is not configurable */\n'%(GAGet(buf,'Size'));
        cstr += '\t\t.pduInfo={\n';
        cstr += '\t\t\t.SduDataPtr = %s,\n'%(GAGet(buf,'Name'));
        cstr += '\t\t\t.SduLength =  %s,\n'%(GAGet(buf,'Size'));
        cstr += '\t\t},\n'
        cstr += '\t\t.externalBufferRuntimeData = &rxBufferParams_%s\n'%(GAGet(buf,'Name'));
        cstr += '\t},\n';
        id += 1;
    cstr += '};\n\n'
    fp.write(cstr);
    #----------------- Request Service
    cstr = 'const Dcm_DslCallbackDCMRequestServiceType DCMRequestServiceList[] = {\n'
    for reqser in GLGet('RequestServiceList'):
        cstr += '\t{ // %s\n'%(GAGet(reqser,'Name'));
        cstr += '\t\t.StartProtocol = %s,\n'%(GAGet(reqser,'StartProtocolCbk'));
        cstr += '\t\t.StopProtocol = %s,\n'%(GAGet(reqser,'StopProtocolCbk'));
        cstr += '\t\t.Arc_EOL = FALSE\n'
        cstr += '\t},\n'
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n';
    fp.write(cstr);
    cstr = 'const Dcm_DslServiceRequestIndicationType DCMServiceRequestIndicationList[] = {\n'
    for reqser in  GLGet('RequestServiceList'):
        if(GAGet(reqser,'ProtocolIndicationCbk') != 'NULL'):
            cstr += '\t{ // %s\n'%(GAGet(reqser,'Name'));
            cstr += '\t\t.Indication =  %s,\n'%(GAGet(reqser,'ProtocolIndicationCbk'));
            cstr += '\t\t.Arc_EOL =  FALSE\n'
            cstr += '\t},\n'
    cstr += '\t{ \n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n';
    fp.write(cstr);
    #--------------------- Protocol
    fp.write('extern const Dcm_DslMainConnectionType DslMainConnectionList[];\n\n');
    #here I am confused witn Main Connection and Connection.
    #So I assume the first configured one was main connection.
    cstr = 'const Dcm_DslProtocolRxType DcmDslProtocolRxList[] = {\n';
    cid = 0;
    for pro in GLGet('ProtocolList'):
        for con in GLGet(pro,'ConnectionList'):
            for rx in GLGet(con,'RxChannelList'):
                cstr += '\t{// %s->%s->%s\n'%(GAGet(pro,'Name'), GAGet(con,'Name'), GAGet(rx,'Name'));
                cstr += '\t\t.DslMainConnectionParent =  &DslMainConnectionList[%s],\n'%(cid);
                cstr += '\t\t.DslProtocolAddrType = DCM_PROTOCOL_%s_ADDR_TYPE,\n'%(GAGet(rx,'AddressingType'));
                cstr += '\t\t.DcmDslProtocolRxPduId = PDUR_ID_%s,\n'%(GAGet(rx,'PduRef'));
                cstr += '\t\t.DcmDslProtocolRxTesterSourceAddr_v4 =  0,       /* Value is not configurable */\n'
                cstr += '\t\t.DcmDslProtocolRxChannelId_v4 =  0,                /* Value is not configurable */\n'
                cstr += '\t\t.Arc_EOL =  FALSE\n'
                cstr += '\t},\n'
        cid += 1;
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    #---tx
    cstr = 'const Dcm_DslProtocolTxType DcmDslProtocolTxList[] = {\n';
    cid = 0;
    for pro in GLGet('ProtocolList'):
        for con in GLGet(pro,'ConnectionList'):
            tx = GLGet(con,'TxChannel')
            if(tx != []):
                cstr += '\t{// %s->%s->%s\n'%(GAGet(pro,'Name'), GAGet(con,'Name'), GAGet(tx,'Name'));
                cstr += '\t\t.DslMainConnectionParent =  &DslMainConnectionList[%s],\n'%(cid);
                cstr += '\t\t.DcmDslProtocolTxPduId =  PDUR_ID_%s,\n'%(GAGet(tx,'PduRef'));
                cstr += '\t\t.Arc_EOL =  FALSE\n'
                cstr += '\t},\n'
        cid += len(GLGet(con,'RxChannelList'));
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    #--- 
    fp.write('extern const Dcm_DslConnectionType DslConnectionList[];\n\n');
    cstr = 'const Dcm_DslMainConnectionType DslMainConnectionList[] = {\n'
    cid = tid =0; # help me to record the index for connection and transmit Pdu ID
    for pro in GLGet('ProtocolList'):
        for con in GLGet(pro,'ConnectionList'):
            cstr += '\t{//%s->%s\n'%(GAGet(pro,'Name'), GAGet(con,'Name'));
            cstr += '\t\t.DslConnectionParent =  &DslConnectionList[%s],\n'%(cid);
            cstr += '\t\t.DslPeriodicTransmissionConRef =  NULL,        /* Value is not configurable */\n'
            cstr += '\t\t.DslROEConnectionRef =  NULL,                /* Value is not configurable */\n'
            cstr += '\t\t.DslProtocolRx =  NULL,                        /* Value is not configurable */\n'
            cstr += '\t\t.DslProtocolTx =  &DcmDslProtocolTxList[%s],\n'%(tid);
            tid += 1 
            cstr += '\t},\n'
            cid += 1;
    cstr += '};\n\n'
    fp.write(cstr);
    #-----------
    fp.write('extern const Dcm_DslProtocolRowType DslProtocolRowList[];\n\n')
    pid = cid =0;
    cstr = 'const Dcm_DslConnectionType DslConnectionList[] = {\n'
    for pro in GLGet('ProtocolList'):
        for con in GLGet(pro,'ConnectionList'):
            cstr += '\t{//%s->%s\n'%(GAGet(pro,'Name'), GAGet(con,'Name'));
            cstr += '\t\t.DslProtocolRow = &DslProtocolRowList[%s],\n'%(pid);
            cstr += '\t\t.DslMainConnection = &DslMainConnectionList[%s],\n'%(cid);
            cstr += '\t\t.DslPeriodicTransmission = NULL,    /* Value is not configurable */\n'
            cstr += '\t\t.DslResponseOnEvent = NULL,    /* Value is not configurable */\n'
            cstr += '\t\t.Arc_EOL =  %s\n'%('FALSE');
            cstr += '\t},\n'
            cid += 1;
        pid += 1;  
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = %s\n'%('TRUE');
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    # now I am really very confused by the relation between Connection and Main Connection, ???
    #----------------
    fp.write('extern const Dcm_DslProtocolTimingRowType ProtocolTimingList[];\n\n');
    fp.write('Dcm_DslRunTimeProtocolParametersType dcmDslRuntimeVariables[%s];\n'%(len(GLGet('ProtocolList'))))
    cstr = 'const Dcm_DslProtocolRowType DslProtocolRowList[]= {\n'
    id = 0;
    for pro in GLGet('ProtocolList'):
        cstr += '\t{//%s\n'%(GAGet(pro,'Name'))
        cstr += '\t\t.DslProtocolID = DCM_%s,\n'%((GAGet(pro,'ProtocolID')))
        cstr += '\t\t.DslProtocolIsParallelExecutab = FALSE, // not supported\n'
        cstr += '\t\t.DslProtocolPreemptTimeout = 0,    // not supported\n'
        cstr += '\t\t.DslProtocolPriority = 0,    // not supported\n'
        cstr += '\t\t.DslProtocolTransType = DCM_PROTOCOL_TRANS_%s,\n'%((GAGet(pro,'TransmissionType')))
        cstr += '\t\t.DslProtocolRxBufferID = &DcmDslBufferList[INDEX_OF_BUF_%s],\n'%(GAGet(pro,'RxBufferRef'))
        cstr += '\t\t.DslProtocolTxBufferID = &DcmDslBufferList[INDEX_OF_BUF_%s],\n'%(GAGet(pro,'TxBufferRef'))
        cstr += '\t\t.DslProtocolSIDTable = &DsdServiceTable[INDEX_OF_ST_%s],\n'%(GAGet(pro,'ServiceTableRef'))
        cstr += '\t\t.DslProtocolTimeLimit = &ProtocolTimingList[INDEX_OF_TL_%s],\n'%(GAGet(pro,'TimingLimitRef'))
        cstr += '\t\t.DslConnection =  %s,\n'%('DslConnectionList')
        cstr += '\t\t.DslRunTimeProtocolParameters = &dcmDslRuntimeVariables[%s],\n'%(id)
        cstr += '\t\t.Arc_EOL = %s\n'%('FALSE')
        cstr += '\t},\n'
        id += 1;
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = %s\n'%('TRUE')
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    fp.write("""const Dcm_DslProtocolType DslProtocol = {
    .DslProtocolRxGlobalList = DcmDslProtocolRxList,
    .DslProtocolTxGlobalList = DcmDslProtocolTxList,
    .DslProtocolRowList = DslProtocolRowList
};\n\n""")
    cstr = 'const Dcm_DslProtocolTimingRowType ProtocolTimingList[] = {\n'
    for tim in GLGet('TimingList'):
        cstr += '\t{//%s\n'%(GAGet(tim,'Name'));
        cstr += '\t\t.TimStrP2ServerMax = %s,\n'%(GAGet(tim,'P2ServerMaxTimeMs'))
        cstr += '\t\t.TimStrP2ServerMin = %s,\n'%(GAGet(tim,'P2ServerMinTimeMs'))
        cstr += '\t\t.TimStrP2StarServerMax = 0,        /* Value is not configurable */\n'
        cstr += '\t\t.TimStrP2StarServerMin = 0,        /* Value is not configurable */\n'
        cstr += '\t\t.TimStrS3Server = %s,\n'%(GAGet(tim,'S3ServerTimeMs'))
        cstr += '\t\t.Arc_EOL = FALSE\n'
        cstr +='\t},\n'
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL =  TRUE\n'
    cstr +='\t},\n'
    cstr += '};\n\n'
    fp.write(cstr);
    fp.write("""const Dcm_DslProtocolTimingType ProtocolTiming = {
/* DslProtocolTimingRow = */ ProtocolTimingList
};\n\n""")
    #--------------
    cstr = 'const Dcm_DslSessionControlType SessionControlList[] = {\n'
    for ses in GLGet('SessionControlList'):
        cstr += '\t{//%s\n'%(GAGet(ses,'Name'))
        cstr += '\t\t.GetSesChgPermission = %s,\n'%(GAGet(ses,'GetSessionChangePermissionCbk'))
        cstr += '\t\t.ChangeIndication = NULL,\n'
        cstr += '\t\t.ConfirmationRespPend = NULL,\n'
        cstr += '\t\t.Arc_EOL = FALSE\n'
        cstr += '\t},\n'
    cstr += '\t{\n'
    cstr += '\t\t.Arc_EOL = TRUE\n'
    cstr += '\t}\n'
    cstr += '};\n\n'
    fp.write(cstr);
    # also I am not sure, so ... ??????
    fp.write("""const Dcm_DslDiagRespType DiagResp = {
    .DslDiagRespForceRespPendEn = TRUE,
    .DslDiagRespMaxNumRespPend =  8    // TODO
};\n""")

    fp.write("""
const Dcm_DslType Dsl = {
    .DslBuffer =  DcmDslBufferList,
    .DslCallbackDCMRequestService = DCMRequestServiceList,
    .DslDiagResp = &DiagResp,
    .DslProtocol =  &DslProtocol,
    .DslProtocolTiming = &ProtocolTiming,
    .DslServiceRequestIndication = DCMServiceRequestIndicationList,
    .DslSessionControl = SessionControlList
};

const Dcm_ConfigType DCM_Config = {
    .Dsp = &Dsp,
    .Dsd = &Dsd,
    .Dsl = &Dsl
};
""");
    #------------------------------------------------------------------
    fp.close();

def GetSecurityRefName(SecurityList):
    SecurityRef = []
    for sec in SecurityList:
        SecurityRef.append(GAGet(sec,'Name'))
    SecurityRef.sort()
    SecurityRef.sort()
    cstr = 'SecurityList'
    for ss in SecurityRef:
        cstr += '_%s'%(ss)
    if(len(SecurityRef)>0):
        return cstr
    else:
        return 'NULL'
def GenSecurityRef(fp,SecurityList):
    global AlreadySecurityRef
    SecurityRef = []
    for sec in SecurityList:
        SecurityRef.append(GAGet(sec,'Name'))
    SecurityRef.sort()
    Name = GetSecurityRefName(SecurityList)
    try:
        if(len(SecurityRef)>0):
            index = AlreadySecurityRef.index(Name)
    except:
        AlreadySecurityRef.append(Name)
        cstr = 'const Dcm_DspSecurityRowType *%s[] = {\n'%(Name);
        for ss in SecurityRef:
            cstr += '\t&DspSecurityList[INDEX_OF_SECURITY_%s],\n'%(ss);
        cstr += '\t&DspSecurityList[DCM_SECURITY_EOL_INDEX]\n};\n'
        fp.write(cstr)
        

def GetSessionRefName(SessionList):
    SessionRef = []
    for ses in SessionList:
        SessionRef.append(GAGet(ses,'Name'))
    SessionRef.sort()
    cstr = 'SessionList'
    for ss in SessionRef:
        cstr += '_%s'%(ss)
    if(len(SessionRef)>0):
        return cstr
    else:
        return 'NULL'

def GenSessionRef(fp,SessionList):
    global AlreadySessionRef
    SessionRef = []
    for ses in SessionList:
        SessionRef.append(GAGet(ses,'Name'))
    SessionRef.sort()
    Name = GetSessionRefName(SessionList)
    try:
        if(len(SessionRef)>0):
            index = AlreadySessionRef.index(Name)
    except:
        AlreadySessionRef.append(Name)
        cstr = 'const Dcm_DspSessionRowType *%s[] = {\n'%(Name);
        for ss in SessionRef:
            cstr += '\t&DspSessionList[INDEX_OF_SESSION_%s],\n'%(ss);
        cstr += '\t&DspSessionList[DCM_SECURITY_EOL_INDEX]\n};\n'
        fp.write(cstr)