
#include "Std_Types.h"
#include "Dcm.h"
#include "Dcm_Internal.h"
//#if defined(USE_PDUR)
#include "PduR.h"
//#endif

#define DCM_SECURITY_EOL_INDEX 2
#define DCM_SESSION_EOL_INDEX 4
#define DCM_DID_LIST_EOL_INDEX 1

extern Std_ReturnType vDid_1_ReadDataLength_Cbk(uint16 *didLength);
extern Std_ReturnType vDid_1_ConditionCheckRead_Cbk(Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1_ReadData_Cbk(uint8 *data);
extern Std_ReturnType vDid_1_ConditionCheckWrite_Cbk(Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1_WriteData_Cbk(uint8 *data, uint16 dataLength, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1_GetScalingInfo_Cbk(uint8 *scalingInfo, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1_FreezeCurrentState_cbk(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1_ResetToDefault_Cbk(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1_ReturnControl_Cbk(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vDid_1__ShortTermAdj_cbk(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode);


extern Std_ReturnType vSecurityLevel_1_GetSeed (uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vSecurityLevel_1_CompKey (uint8 *key);
extern Std_ReturnType vSecurityLevel_Locked_GetSeed (uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vSecurityLevel_Locked_CompKey (uint8 *key);
extern Std_ReturnType vRequestService_1_Start (Dcm_ProtocolType protocolID);
extern Std_ReturnType vRequestService_1_Stop (Dcm_ProtocolType protocolID);
extern Std_ReturnType vRequestService_1_Indication(uint8 *requestData, uint16 dataSize);

extern Std_ReturnType vSessionControl_1_GetSesChgPer(Dcm_SesCtrlType sesCtrlTypeActive, Dcm_SesCtrlType sesCtrlTypeNew);
extern Std_ReturnType vRoutine_1_Start(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vRoutine_1_Stop(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vRoutine_1_RequestResult(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);

const Dcm_DspSecurityRowType DspSecurityList[] = {
	{ // vSecurityLevel_1
		.DspSecurityLevel =  1,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  1,
		.DspSecurityKeySize =  1,
		.GetSeed =  vSecurityLevel_1_GetSeed,
		.CompareKey =  vSecurityLevel_1_CompKey,
		.Arc_EOL =  FALSE
	},
	{ // vSecurityLevel_Locked
		.DspSecurityLevel =  0,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  1,
		.DspSecurityKeySize =  1,
		.GetSeed =  vSecurityLevel_Locked_GetSeed,
		.CompareKey =  vSecurityLevel_Locked_CompKey,
		.Arc_EOL =  FALSE
	},
	{ // Dummy For EOL
		.DspSecurityLevel =  0xBD,
		.DspSecurityDelayTimeOnBoot =  0,//.Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//.Value is not configurable
		.DspSecurityDelayTime =  0, 	//.Value is not configurable
		.DspSecurityNumAttLock =  0, 	//.Value is not configurable
		.DspSecurityADRSize =  0xBD,
		.DspSecuritySeedSize =  0xBD,
		.DspSecurityKeySize =  0xBD,
		.GetSeed =  NULL,
		.CompareKey =  NULL,
		.Arc_EOL =  TRUE
	}
};

const Dcm_DspSecurityType DspSecurity = {
    .DspSecurityRow =  DspSecurityList
};
const Dcm_DspSessionRowType DspSessionList[] = {
	{ //vSession_default
		.DspSessionLevel =  1,
		.DspSessionP2ServerMax =  10,
		.DspSessionP2StarServerMax =  10,
		.Arc_EOL =  FALSE
	},
	{ //vSession_program
		.DspSessionLevel =  2,
		.DspSessionP2ServerMax =  10,
		.DspSessionP2StarServerMax =  10,
		.Arc_EOL =  FALSE
	},
	{ //vSession_extended
		.DspSessionLevel =  3,
		.DspSessionP2ServerMax =  10,
		.DspSessionP2StarServerMax =  10,
		.Arc_EOL =  FALSE
	},
	{ //vSession_safety_system
		.DspSessionLevel =  4,
		.DspSessionP2ServerMax =  10,
		.DspSessionP2StarServerMax =  10,
		.Arc_EOL =  FALSE
	},
	{ //Dummy Session For EOL
		.DspSessionLevel =  0xBD,
		.DspSessionP2ServerMax =  0xBD,
		.DspSessionP2StarServerMax =  0xBD,
		.Arc_EOL =  TRUE
	},
};

const Dcm_DspSessionType DspSession = {
    .DspSessionRow =  DspSessionList,
};

const Dcm_DspSecurityRowType *vServiceTable_1_vService0_session_control_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService1_security_access_SecurityList[] = {
	&DspSecurityList[1],//vSecurityLevel_Locked
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService2_read_data_by_id_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService3_write_data_by_id_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService4_routine_control_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService5_read_scaling_data_by_id_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService6_test_present_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService7_ecu_reset_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService8_ddd_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService9_read_period_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService10_io_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService11_read_memory_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vServiceTable_1_vService12_write_memory_SecurityList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService0_session_control_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService1_security_access_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService2_read_data_by_id_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService3_write_data_by_id_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService4_routine_control_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService5_read_scaling_data_by_id_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService6_test_present_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService7_ecu_reset_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService8_ddd_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService9_read_period_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService10_io_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService11_read_memory_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *vServiceTable_1_vService12_write_memory_SessionList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};

const Dcm_DspDidControlRecordSizesType vDidCtrlRecord_1 = {
	.DspDidControlEnableMaskRecordSize =  0,
	.DspDidControlOptionRecordSize =  0,
	.DspDidControlStatusRecordSize =  0,
};
const Dcm_DspSessionRowType *vDcmDidInfo_1_read_sessionRefList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vDcmDidInfo_1_read_securityRefList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspDidReadType vDcmDidInfo_1_didRead = {
    .DspDidReadSessionRef =  vDcmDidInfo_1_read_sessionRefList,
    .DspDidReadSecurityLevelRef =  vDcmDidInfo_1_read_securityRefList
};

const Dcm_DspSessionRowType *vDcmDidInfo_1_write_sessionRefList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vDcmDidInfo_1_write_securityRefList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspDidWriteType vDcmDidInfo_1_didWrite = {
    .DspDidWriteSessionRef =  vDcmDidInfo_1_write_sessionRefList,
    .DspDidWriteSecurityLevelRef =  vDcmDidInfo_1_write_securityRefList
};

const Dcm_DspSessionRowType *vDcmDidInfo_1_control_sessionRefList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vDcmDidInfo_1_control_securityRefList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspDidControlType vDcmDidInfo_1_didControl = {
    .DspDidControlSessionRef =  vDcmDidInfo_1_control_sessionRefList,
    .DspDidControlSecurityLevelRef =  vDcmDidInfo_1_control_securityRefList,
    .DspDidFreezeCurrentState =  &vDidCtrlRecord_1,
    .DspDidResetToDefault =  &vDidCtrlRecord_1,
    .DspDidReturnControlToEcu =  &vDidCtrlRecord_1,
    .DspDidShortTermAdjustment =  NULL
};

const Dcm_DspDidInfoType DspDidInfoList[] = {
	{ // vDcmDidInfo_1
		 .DspDidDynamicllyDefined =  FALSE,
		 .DspDidFixedLength =  FALSE,
		 .DspDidScalingInfoSize =  1,
		{ // DspDidAccess
			 .DspDidRead  =&vDcmDidInfo_1_didRead,
			 .DspDidWrite = &vDcmDidInfo_1_didWrite,
			 .DspDidControl = &vDcmDidInfo_1_didControl,
		}
	}, 
};

extern const Dcm_DspDidType DspDidList[];
const Dcm_DspDidType* vDid_1_dididRefList[] =
{
    &DspDidList[DCM_DID_LIST_EOL_INDEX]  //add did ref by hand please,If you need it
};

const Dcm_DspDidType DspDidList[] = { 
	{ // vDid_1,
		.DspDidUsePort =  FALSE,//.Value is not configurable
		.DspDidIdentifier =  0x0999,
		.DspDidInfoRef =  &DspDidInfoList[0], //vDcmDidInfo_1
		.DspDidRef =  vDid_1_dididRefList,
		.DspDidSize =  64,
		.DspDidReadDataLengthFnc =  vDid_1_ReadDataLength_Cbk,
		.DspDidConditionCheckReadFnc =  vDid_1_ConditionCheckRead_Cbk,
		.DspDidReadDataFnc =  vDid_1_ReadData_Cbk,
		.DspDidConditionCheckWriteFnc =  vDid_1_ConditionCheckWrite_Cbk,
		.DspDidWriteDataFnc =  vDid_1_WriteData_Cbk,
		.DspDidGetScalingInfoFnc =  vDid_1_GetScalingInfo_Cbk,
		.DspDidFreezeCurrentStateFnc =  vDid_1_FreezeCurrentState_cbk,
		.DspDidResetToDefaultFnc =  vDid_1_ResetToDefault_Cbk,
		.DspDidReturnControlToEcuFnc =  vDid_1_ReturnControl_Cbk,
		.DspDidShortTermAdjustmentFnc =  vDid_1__ShortTermAdj_cbk,
		.DspDidControlRecordSize =  NULL,
		.Arc_EOL =  FALSE
	},
	{ // Dummy for EOL,
		.DspDidUsePort =  FALSE,//.Value is not configurable
		.DspDidIdentifier =  0xDB,
		.DspDidInfoRef =  NULL,
		.DspDidRef =  NULL, //I cann't understand
		.DspDidSize =  0xDB,
		.DspDidReadDataLengthFnc =  NULL,
        .DspDidConditionCheckReadFnc =  NULL,
        .DspDidReadDataFnc =  NULL,
        .DspDidConditionCheckWriteFnc =  NULL,
        .DspDidWriteDataFnc =  NULL,
        .DspDidGetScalingInfoFnc =  NULL,
        .DspDidFreezeCurrentStateFnc =  NULL,
        .DspDidResetToDefaultFnc =  NULL,
        .DspDidReturnControlToEcuFnc =  NULL,
        .DspDidShortTermAdjustmentFnc =  NULL,
        .DspDidControlRecordSize =  NULL,
		.Arc_EOL =  TRUE
	}
};

//************************************************************************
//*							Routine control								*
//***********************************************************************

const Dcm_DspStartRoutineType vDcmRoutineInfo_1_start = {
    .DspStartRoutineCtrlOptRecSize =  0,
    .DspStartRoutineStsOptRecSize =  1
};
const Dcm_DspRoutineStopType vDcmRoutineInfo_1_stop = {
    .DspStopRoutineCtrlOptRecSize =  0,
    .DspStopRoutineStsOptRecSize =  1,
};
const Dcm_DspSessionRowType *vDcmRoutineInfo_1_sessionRefList[] = {
	&DspSessionList[0],//vSession_default
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSecurityRowType *vDcmRoutineInfo_1_securityRefList[] = {
	&DspSecurityList[0],//vSecurityLevel_1
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspRoutineRequestResType vDcmRoutineInfo_1_request = {
    .DspReqResRtnCtrlOptRecSize =  1
};
const Dcm_DspRoutineInfoType DspRoutineInfoList[] = {
	{//vDcmRoutineInfo_1
		{//DspRoutineAuthorization
			 .DspRoutineSessionRef =  vDcmRoutineInfo_1_sessionRefList,
			 .DspRoutineSecurityLevelRef =  vDcmRoutineInfo_1_securityRefList,
		},
		 .DspStartRoutine =  &vDcmRoutineInfo_1_start,
		 .DspRoutineStop =  &vDcmRoutineInfo_1_stop,
		 .DspRoutineRequestRes =  &vDcmRoutineInfo_1_request,
	},
};

const Dcm_DspRoutineType  DspRoutineList[] = {
	{//vRoutine_1
		 .DspRoutineUsePort =  FALSE,
		 .DspRoutineIdentifier =  0x0888,
		 .DspRoutineInfoRef =  &DspRoutineInfoList[0],//vDcmRoutineInfo_1
		 .DspStartRoutineFnc =  vRoutine_1_Start,
		 .DspStopRoutineFnc =  vRoutine_1_Stop,
		 .DspRequestResultRoutineFnc =  vRoutine_1_RequestResult,
		 .Arc_EOL = FALSE
	},
	{//Dummy For EOL
		 .DspRoutineUsePort =  FALSE,
		 .DspRoutineIdentifier =  0xDB,
		 .DspRoutineInfoRef =  NULL,
		 .DspStartRoutineFnc =  NULL,
		 .DspStopRoutineFnc =  NULL,
		 .DspRequestResultRoutineFnc =  NULL,
		 .Arc_EOL = TRUE
	}
};

//************************************************************************
//*							Memory Info             					*
//***********************************************************************

const Dcm_DspType Dsp = {
    .DspMaxDidToRead =  0x99,//????
    .DspDid =  DspDidList,
    .DspDidInfo =  DspDidInfoList,
    .DspEcuReset =  NULL,
    .DspPid =  NULL,
    .DspReadDTC =  NULL,
    .DspRequestControl =  NULL,
    .DspRoutine =  DspRoutineList,
    .DspRoutineInfo =  DspRoutineInfoList,
    .DspSecurity =  &DspSecurity,
    .DspSession =  &DspSession,
    .DspTestResultByObdmid =  NULL,
    .DspVehInfo =  NULL
};

//************************************************************************
//*									DSD									*
//***********************************************************************

const Dcm_DsdServiceType vServiceTable_1_serviceList[] = {
	{ // vService0_session_control
		 .DsdSidTabServiceId = SID_DIAGNOSTIC_SESSION_CONTROL,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService0_session_control_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService0_session_control_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService1_security_access
		 .DsdSidTabServiceId = SID_SECURITY_ACCESS,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService1_security_access_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService1_security_access_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService2_read_data_by_id
		 .DsdSidTabServiceId = SID_READ_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService2_read_data_by_id_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService2_read_data_by_id_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService3_write_data_by_id
		 .DsdSidTabServiceId = SID_WRITE_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService3_write_data_by_id_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService3_write_data_by_id_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService4_routine_control
		 .DsdSidTabServiceId = SID_ROUTINE_CONTROL,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService4_routine_control_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService4_routine_control_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService5_read_scaling_data_by_id
		 .DsdSidTabServiceId = SID_READ_SCALING_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService5_read_scaling_data_by_id_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService5_read_scaling_data_by_id_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService6_test_present
		 .DsdSidTabServiceId = SID_TESTER_PRESENT,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService6_test_present_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService6_test_present_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService7_ecu_reset
		 .DsdSidTabServiceId = SID_ECU_RESET,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService7_ecu_reset_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService7_ecu_reset_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService8_ddd
		 .DsdSidTabServiceId = SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService8_ddd_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService8_ddd_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService9_read_period
		 .DsdSidTabServiceId = SID_READ_DATA_BY_PERIODIC_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService9_read_period_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService9_read_period_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService10_io
		 .DsdSidTabServiceId = SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService10_io_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService10_io_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService11_read_memory
		 .DsdSidTabServiceId = SID_READ_MEMORY_BY_ADDRESS,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService11_read_memory_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService11_read_memory_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // vService12_write_memory
		 .DsdSidTabServiceId = SID_WRITE_MEMORY_BY_ADDRESS,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = vServiceTable_1_vService12_write_memory_SecurityList,
		 .DsdSidTabSessionLevelRef = vServiceTable_1_vService12_write_memory_SessionList,
		 .Arc_EOL =  FALSE
	},
	{ // Dummy For EOL
		 .DsdSidTabServiceId = 0xDB,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = NULL,
		 .DsdSidTabSessionLevelRef = NULL,
		 .Arc_EOL =  TRUE
	}
};

const Dcm_DsdServiceTableType DsdServiceTable[] = {	
	{ // vServiceTable_1
		 .DsdSidTabId =  0,
		 .DsdService =  vServiceTable_1_serviceList,
		 .Arc_EOL =  FALSE
	},
	{ // Dummy For EOL
		 .DsdSidTabId =  0xDB,
		 .DsdService =  NULL,
		 .Arc_EOL =  TRUE
	}
};

const Dcm_DsdType Dsd = {
    .DsdServiceTable =  DsdServiceTable
};

//************************************************************************
//*									DSL									*
//***********************************************************************

uint8 rxDcmBuffer_DiagP2P[512];
Dcm_DslBufferRuntimeType rxBufferParams_DiagP2P =
{
	.status =  NOT_IN_USE
};
uint8 txDcmBuffer_DiagP2P[512];
Dcm_DslBufferRuntimeType txBufferParams_DiagP2P =
{
	.status =  NOT_IN_USE
};
uint8 rxDcmBuffer_DiagP2A[512];
Dcm_DslBufferRuntimeType rxBufferParams_DiagP2A =
{
	.status =  NOT_IN_USE
};
uint8 txDcmBuffer_DiagP2A[512];
Dcm_DslBufferRuntimeType txBufferParams_DiagP2A =
{
	.status =  NOT_IN_USE
};
const Dcm_DslBufferType DcmDslBufferList[DCM_DSL_BUFFER_LIST_LENGTH] = {
	{
		.DslBufferID =  0,    //? I am not that clear.
		.DslBufferSize =  512,//.?Value is not configurable
		.pduInfo.SduDataPtr =  rxDcmBuffer_DiagP2P,
		.pduInfo.SduLength =  512,
		.externalBufferRuntimeData =  &rxBufferParams_DiagP2P
	},
	{
		.DslBufferID =  1,//? I am not that clear.
		.DslBufferSize =  512,//.?Value is not configurable
		.pduInfo.SduDataPtr =  txDcmBuffer_DiagP2P,
		.pduInfo.SduLength =  512,
		.externalBufferRuntimeData =  &txBufferParams_DiagP2P
	},
	{
		.DslBufferID =  0,//? I am not that clear.
		.DslBufferSize =  512,//.?Value is not configurable
		.pduInfo.SduDataPtr =  rxDcmBuffer_DiagP2A,
		.pduInfo.SduLength =  512,
		.externalBufferRuntimeData =  &rxBufferParams_DiagP2P
	},
	{
		.DslBufferID =  1,//? I am not that clear.
		.DslBufferSize =  512,//.?Value is not configurable
		.pduInfo.SduDataPtr =  txDcmBuffer_DiagP2A,
		.pduInfo.SduLength =  512,
		.externalBufferRuntimeData =  &txBufferParams_DiagP2A
	}
};

const Dcm_DslCallbackDCMRequestServiceType DCMRequestServiceList[] = {
	{ // vRequestService_1
		.StartProtocol =  vRequestService_1_Start,
		.StopProtocol =  vRequestService_1_Stop,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

const Dcm_DslServiceRequestIndicationType DCMServiceRequestIndicationList[] = {
	{
		.Indication =  vRequestService_1_Indication,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

extern const Dcm_DslMainConnectionType DslMainConnectionList[];

const Dcm_DslProtocolRxType DcmDslProtocolRxList[] = {
	{
		.DslMainConnectionParent =  &DslMainConnectionList[0],
		.DslProtocolAddrType =  DCM_PROTOCOL_PHYSICAL_ADDR_TYPE,
		.DcmDslProtocolRxPduId =  PDUR_DIAG_P2P_REQ,
		.DcmDslProtocolRxTesterSourceAddr_v4 =  0,		//.Value is not configurable
		.DcmDslProtocolRxChannelId_v4 =  0,				//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.DslMainConnectionParent =  &DslMainConnectionList[1],
		.DslProtocolAddrType =  DCM_PROTOCOL_PHYSICAL_ADDR_TYPE,
		.DcmDslProtocolRxPduId =  PDUR_DIAG_P2A_REQ,
		.DcmDslProtocolRxTesterSourceAddr_v4 =  0,		//.Value is not configurable
		.DcmDslProtocolRxChannelId_v4 =  0,				//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

const Dcm_DslProtocolTxType DcmDslProtocolTxList[] = {
	{
		.DslMainConnectionParent =  &DslMainConnectionList[0],
		.DcmDslProtocolTxPduId =  PDUR_DIAG_P2P_ACK,
		.DcmDslProtocolDcmTxPduId = DCM_DIAG_P2P_ACK,
		.Arc_EOL =  FALSE
	},
	{
		.DslMainConnectionParent =  &DslMainConnectionList[1],
		.DcmDslProtocolTxPduId =  PDUR_DIAG_P2A_ACK,
		.DcmDslProtocolDcmTxPduId = DCM_DIAG_P2A_ACK,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

extern const Dcm_DslConnectionType DslConnectionList[];

const Dcm_DslMainConnectionType DslMainConnectionList[] = {
	{
		.DslConnectionParent =  &DslConnectionList[0],
		.DslPeriodicTransmissionConRef =  NULL,		//.Value is not configurable
		.DslROEConnectionRef =  NULL,				//.Value is not configurable
		.DslProtocolRx =  NULL,						//.Value is not configurable
		.DslProtocolTx =  &DcmDslProtocolTxList[0],
	},
	{
		.DslConnectionParent =  &DslConnectionList[1],
		.DslPeriodicTransmissionConRef =  NULL,		//.Value is not configurable
		.DslROEConnectionRef =  NULL,				//.Value is not configurable
		.DslProtocolRx =  NULL,						//.Value is not configurable
		.DslProtocolTx =  &DcmDslProtocolTxList[1],
	},
};

extern const Dcm_DslProtocolRowType DslProtocolRowList[];

const Dcm_DslConnectionType DslConnectionList[] = {
	{
		.DslProtocolRow =  &DslProtocolRowList[0],
		.DslMainConnection =  &DslMainConnectionList[0],
		.DslPeriodicTransmission =  NULL,	//.Value is not configurable
		.DslResponseOnEvent =  NULL,	//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.DslProtocolRow =  &DslProtocolRowList[1],
		.DslMainConnection =  &DslMainConnectionList[1],
		.DslPeriodicTransmission =  NULL,	//.Value is not configurable
		.DslResponseOnEvent =  NULL,	//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

extern const Dcm_DslProtocolTimingRowType ProtocolTimingList[];

Dcm_DslRunTimeProtocolParametersType dcmDslRuntimeVariables[2]; // for DIAG_P2P and DIAG_P2A
const Dcm_DslProtocolRowType DslProtocolRowList[]= {
	{
		.DslProtocolID =  DCM_UDS_ON_CAN,
		.DslProtocolIsParallelExecutab =  FALSE, // not supported
		.DslProtocolPreemptTimeout =  0,	// not supported
		.DslProtocolPriority =  0,	// not supported
		.DslProtocolTransType =  DCM_PROTOCOL_TRANS_TYPE_1,
		.DslProtocolRxBufferID =  &DcmDslBufferList[0],//DIAG_P2P
		.DslProtocolTxBufferID =  &DcmDslBufferList[1],
		.DslProtocolSIDTable =  &DsdServiceTable[0],
		.DslProtocolTimeLimit =  &ProtocolTimingList[0],//vTiming_1
		.DslConnection =  DslConnectionList,
		.DslRunTimeProtocolParameters =  &dcmDslRuntimeVariables[0],
		.Arc_EOL =  FALSE
	},
	{
		.DslProtocolID =  DCM_UDS_ON_CAN,
		.DslProtocolIsParallelExecutab =  FALSE, // not supported
		.DslProtocolPreemptTimeout =  0,	// not supported
		.DslProtocolPriority =  0,	// not supported
		.DslProtocolTransType =  DCM_PROTOCOL_TRANS_TYPE_1,
		.DslProtocolRxBufferID =  &DcmDslBufferList[2],//DIAG_P2P
		.DslProtocolTxBufferID =  &DcmDslBufferList[3],
		.DslProtocolSIDTable =  &DsdServiceTable[0],
		.DslProtocolTimeLimit =  &ProtocolTimingList[0],
		.DslConnection =  DslConnectionList,
		.DslRunTimeProtocolParameters =  &dcmDslRuntimeVariables[1],
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL = TRUE
	}
};

const Dcm_DslProtocolType DslProtocol = {
    .DslProtocolRxGlobalList =  DcmDslProtocolRxList,
    .DslProtocolTxGlobalList =  DcmDslProtocolTxList,
    .DslProtocolRowList =  DslProtocolRowList
};

const Dcm_DslProtocolTimingRowType ProtocolTimingList[] = {
	{
		.TimStrP2ServerMax =  5000,
		.TimStrP2ServerMin =  3000,
		.TimStrP2StarServerMax =  0,		//.Value is not configurable
		.TimStrP2StarServerMin =  0,		//.Value is not configurable
		.TimStrS3Server =  5000,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	},
};

const Dcm_DslProtocolTimingType ProtocolTiming = {
    .DslProtocolTimingRow =  ProtocolTimingList
};

const Dcm_DslSessionControlType SessionControlList[] = {
	{//vSessionControl_1
		 .GetSesChgPermission =  vSessionControl_1_GetSesChgPer,
		 .ChangeIndication =  NULL,
		 .ConfirmationRespPend =  NULL,
		 .Arc_EOL =  FALSE
	},
	{
		 .Arc_EOL =  TRUE
	}
};

const Dcm_DslDiagRespType DiagResp = {
    .DslDiagRespForceRespPendEn =  TRUE,
    .DslDiagRespMaxNumRespPend =  10
};

const Dcm_DslType Dsl = {
    .DslBuffer =  DcmDslBufferList,
    .DslCallbackDCMRequestService =  DCMRequestServiceList,
    .DslDiagResp =  &DiagResp,
    .DslProtocol =  &DslProtocol,
    .DslProtocolTiming =  &ProtocolTiming,
    .DslServiceRequestIndication =  DCMServiceRequestIndicationList,
    .DslSessionControl =  SessionControlList
};

const Dcm_ConfigType DCM_Config = {
    .Dsp =  &Dsp,
    .Dsd =  &Dsd,
    .Dsl =  &Dsl
};
