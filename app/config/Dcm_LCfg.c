
#include "Std_Types.h"
#include "Dcm.h"
#include "Dcm_Internal.h"
//#if defined(USE_PDUR)
#include "PduR.h"
//#endif

#include "Diag.h"

#define DCM_SECURITY_EOL_INDEX ((sizeof(DspSecurityList)/sizeof(Dcm_DspSecurityRowType))-1)
#define DCM_SESSION_EOL_INDEX  ((sizeof(DspSessionList)/sizeof(Dcm_DspSessionRowType))-1)
#define DCM_DID_LIST_EOL_INDEX 1

extern Std_ReturnType vRoutine_1_Start(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vRoutine_1_Stop(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType vRoutine_1_RequestResult(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);

const Dcm_DspSecurityRowType DspSecurityList[] = {
	{ // Default Security Level, that is for each session
		.DspSecurityLevel =  0,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  4,
		.DspSecurityKeySize =  4,
		.GetSeed =  NULL,	// as each session entered, it is already in this level, cause send NRC
		.CompareKey =  NULL,
		.Arc_EOL =  FALSE
	},
	{ // Level 5 for Default Session
		.DspSecurityLevel =  5,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  4,
		.DspSecurityKeySize =  4,
		.GetSeed =  Diag_GetSeed,
		.CompareKey =  Diag_CompKey,
		.Arc_EOL =  FALSE
	},
	{ // Level 10 for Program Session
		.DspSecurityLevel =  10,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  4,
		.DspSecurityKeySize =  4,
		.GetSeed =  Diag_GetSeed,
		.CompareKey =  Diag_CompKey,
		.Arc_EOL =  FALSE
	},
	{ // Level 15 for Extended Session
		.DspSecurityLevel =  15,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  4,
		.DspSecurityKeySize =  4,
		.GetSeed =  Diag_GetSeed,
		.CompareKey =  Diag_CompKey,
		.Arc_EOL =  FALSE
	},
	{ // Level 20 for Safety System Session
		.DspSecurityLevel =  20,
		.DspSecurityDelayTimeOnBoot =  0,//Value is not configurable
		.DspSecurityNumAttDelay =  0, 	//Value is not configurable
		.DspSecurityDelayTime =  0, 	//Value is not configurable
		.DspSecurityNumAttLock =  0, 	//Value is not configurable
		.DspSecurityADRSize =  0,
		.DspSecuritySeedSize =  4,
		.DspSecurityKeySize =  4,
		.GetSeed =  Diag_GetSeed,
		.CompareKey =  Diag_CompKey,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

const Dcm_DspSecurityType DspSecurity = {
    .DspSecurityRow =  DspSecurityList
};
const Dcm_DspSessionRowType DspSessionList[] = {
	{ //Session_default
		.DspSessionLevel =  1,
		.DspSessionP2ServerMax =  5000,
		.DspSessionP2StarServerMax =  5000,
		.Arc_EOL =  FALSE
	},
	{ //Session_program
		.DspSessionLevel =  2,
		.DspSessionP2ServerMax =  5000,
		.DspSessionP2StarServerMax =  5000,
		.Arc_EOL =  FALSE
	},
	{ //Session_extended
		.DspSessionLevel =  3,
		.DspSessionP2ServerMax =  5000,
		.DspSessionP2StarServerMax =  5000,
		.Arc_EOL =  FALSE
	},
	{ //Session_safety_system
		.DspSessionLevel =  4,
		.DspSessionP2ServerMax =  5000,
		.DspSessionP2StarServerMax =  5000,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	},
};

const Dcm_DspSessionType DspSession = {
    .DspSessionRow =  DspSessionList,
};

const Dcm_DspSecurityRowType *UnProtected_SecurityList[] = {
	// All Level Include the Default
	&DspSecurityList[0],	// Level 0, no security access before needed
	&DspSecurityList[1],    // Level 5
	&DspSecurityList[2],    // Level 10
	&DspSecurityList[3],	// Level 15
	&DspSecurityList[4],	// Level 20
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
const Dcm_DspSecurityRowType *RWDID_SecurityList[] = {
	&DspSecurityList[2],    // Level 10
	&DspSecurityList[3],	// Level 15
	&DspSecurityList[4],	// Level 20
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
const Dcm_DspSessionRowType *ALL_SessionList[] = {
	&DspSessionList[0],	// Session Default
	&DspSessionList[1],	// Session Program
	&DspSessionList[2],	// Session Extended
	&DspSessionList[3],	// Session Safety System
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
const Dcm_DspSessionRowType *RWDID_SessionList[] = {
	&DspSessionList[1],	// Session Program
	&DspSessionList[2],	// Session Extended
	&DspSessionList[3],	// Session Safety System
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
const Dcm_DspDidReadType RWDID_DIDRead = {
    .DspDidReadSessionRef =  ALL_SessionList,
    .DspDidReadSecurityLevelRef =  UnProtected_SecurityList
};

const Dcm_DspDidWriteType RWDID_DIDWrite = {
    .DspDidWriteSessionRef =  ALL_SessionList,
    .DspDidWriteSecurityLevelRef =  UnProtected_SecurityList
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
	{
		 .DspDidDynamicllyDefined =  FALSE,
		 .DspDidFixedLength =  TRUE,
		 .DspDidScalingInfoSize =  1,
		{ // DspDidAccess
			 .DspDidRead  =&RWDID_DIDRead,
			 .DspDidWrite = &RWDID_DIDWrite,
			 .DspDidControl = NULL,
		}
	}, 
};

extern const Dcm_DspDidType DspDidList[];
const Dcm_DspDidType* NONE_RWDID_RefList[] =
{
    &DspDidList[DCM_DID_LIST_EOL_INDEX]  //add did ref by hand please,If you need it
};


const Dcm_DspDidType DspDidList[] = { 
	{ // RWDID,
		.DspDidUsePort =  FALSE,//.Value is not configurable
		.DspDidIdentifier =  0x010A,	// see ISO 14229:2006(E)  p101
		.DspDidInfoRef =  &DspDidInfoList[0], // Fixed Length
		.DspDidRef =  NONE_RWDID_RefList,	// TODO:
		.DspDidSize = 128,
		.DspDidReadDataLengthFnc =  NULL,		// So no need to get length
		.DspDidConditionCheckReadFnc =  Diag_ConditionCheckRead,
		.DspDidReadDataFnc =  Diag_ReadDID_010A_Cbk,
		.DspDidConditionCheckWriteFnc =  Diag_ConditionCheckWrite,
		.DspDidWriteDataFnc =  Diag_WriteDID_010A_Cbk,
		.DspDidGetScalingInfoFnc =  NULL,
		.DspDidFreezeCurrentStateFnc =  NULL,
		.DspDidResetToDefaultFnc =  NULL,
		.DspDidReturnControlToEcuFnc =  NULL,
		.DspDidShortTermAdjustmentFnc =  NULL,
		.DspDidControlRecordSize =  NULL,
		.Arc_EOL =  FALSE
	},
	{
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

const Dcm_DsdServiceType DIAG_P2PorP2A_serviceList[] = {
	{
		 .DsdSidTabServiceId = SID_DIAGNOSTIC_SESSION_CONTROL,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_SECURITY_ACCESS,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_READ_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = RWDID_SecurityList,
		 .DsdSidTabSessionLevelRef  = RWDID_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_WRITE_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = FALSE,
		 .DsdSidTabSecurityLevelRef = RWDID_SecurityList,
		 .DsdSidTabSessionLevelRef  = RWDID_SessionList,
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
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
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
	{
		 .Arc_EOL =  TRUE
	}
};

const Dcm_DsdServiceTableType DsdServiceTable[] = {	
	{
		 .DsdSidTabId =  0,
		 .DsdService =  DIAG_P2PorP2A_serviceList,
		 .Arc_EOL =  FALSE
	},
	{
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
		.StartProtocol =  Diag_RequestServiceStart,  // must at least config one
		.StopProtocol =   Diag_RequestServiceStop,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

const Dcm_DslServiceRequestIndicationType DCMServiceRequestIndicationList[] = {
	{
		.Indication =  Diag_RequestServiceIndication, // can ignore this configure when turn DCM_REQUEST_INDICATION_ENABLED STD_OFF
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

extern const Dcm_DslMainConnectionType DslMainConnectionList[];

const Dcm_DslProtocolRxType DcmDslProtocolRxList[] = {
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2P_REQ],
		.DslProtocolAddrType =  DCM_PROTOCOL_PHYSICAL_ADDR_TYPE,
		.DcmDslProtocolRxPduId =  PDUR_DIAG_P2P_REQ,
		.DcmDslProtocolRxTesterSourceAddr_v4 =  0,		//.Value is not configurable
		.DcmDslProtocolRxChannelId_v4 =  0,				//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2A_REQ],
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
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2P_ACK],
		.DcmDslProtocolTxPduId =  PDUR_DIAG_P2P_ACK,
		.DcmDslProtocolDcmTxPduId = DCM_DIAG_P2P_ACK,
		.Arc_EOL =  FALSE
	},
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2A_ACK],
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
		.DslConnectionParent =  &DslConnectionList[DCM_DIAG_P2P_REQ],
		.DslPeriodicTransmissionConRef =  NULL,		//.Value is not configurable
		.DslROEConnectionRef =  NULL,				//.Value is not configurable
		.DslProtocolRx =  NULL,						//.Value is not configurable
		.DslProtocolTx =  &DcmDslProtocolTxList[DCM_DIAG_P2P_ACK],
	},
	{
		.DslConnectionParent =  &DslConnectionList[DCM_DIAG_P2A_REQ],
		.DslPeriodicTransmissionConRef =  NULL,		//.Value is not configurable
		.DslROEConnectionRef =  NULL,				//.Value is not configurable
		.DslProtocolRx =  NULL,						//.Value is not configurable
		.DslProtocolTx =  &DcmDslProtocolTxList[DCM_DIAG_P2A_ACK],
	},
};

extern const Dcm_DslProtocolRowType DslProtocolRowList[];

const Dcm_DslConnectionType DslConnectionList[] = {
	{
		.DslProtocolRow =  &DslProtocolRowList[DCM_DIAG_P2P_REQ],
		.DslMainConnection =  &DslMainConnectionList[DCM_DIAG_P2P_REQ],
		.DslPeriodicTransmission =  NULL,	//.Value is not configurable
		.DslResponseOnEvent =  NULL,	//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.DslProtocolRow =  &DslProtocolRowList[DCM_DIAG_P2A_REQ],
		.DslMainConnection =  &DslMainConnectionList[DCM_DIAG_P2A_REQ],
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
		.DslProtocolRxBufferID =  &DcmDslBufferList[2*DCM_DIAG_P2P_REQ+0],
		.DslProtocolTxBufferID =  &DcmDslBufferList[2*DCM_DIAG_P2P_REQ+1],
		.DslProtocolSIDTable =  &DsdServiceTable[0],
		.DslProtocolTimeLimit =  &ProtocolTimingList[0],
		.DslConnection =  DslConnectionList,
		.DslRunTimeProtocolParameters =  &dcmDslRuntimeVariables[DCM_DIAG_P2P_REQ],
		.Arc_EOL =  FALSE
	},
	{
		.DslProtocolID =  DCM_UDS_ON_CAN,
		.DslProtocolIsParallelExecutab =  FALSE, // not supported
		.DslProtocolPreemptTimeout =  0,	// not supported
		.DslProtocolPriority =  0,	// not supported
		.DslProtocolTransType =  DCM_PROTOCOL_TRANS_TYPE_1,
		.DslProtocolRxBufferID =  &DcmDslBufferList[2*DCM_DIAG_P2A_REQ+0],
		.DslProtocolTxBufferID =  &DcmDslBufferList[2*DCM_DIAG_P2A_REQ+1],
		.DslProtocolSIDTable =  &DsdServiceTable[0],
		.DslProtocolTimeLimit =  &ProtocolTimingList[0],
		.DslConnection =  DslConnectionList,
		.DslRunTimeProtocolParameters =  &dcmDslRuntimeVariables[DCM_DIAG_P2A_REQ],
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
		.TimStrP2ServerMax =  5000,		// ms
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
		 .GetSesChgPermission =  Diag_GetSesChgPer,
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
