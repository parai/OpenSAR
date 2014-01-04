
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


static const Dcm_DspSecurityRowType DspSecurityList[] = {
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

static const Dcm_DspSecurityType DspSecurity = {
    .DspSecurityRow =  DspSecurityList
};
static const Dcm_DspSessionRowType DspSessionList[] = {
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

static const Dcm_DspSessionType DspSession = {
    .DspSessionRow =  DspSessionList,
};

static const Dcm_DspSecurityRowType *UnProtected_SecurityList[] = {
	// All Level Include the Default
	&DspSecurityList[0],	// Level 0, no security access before needed
	&DspSecurityList[1],    // Level 5
	&DspSecurityList[2],    // Level 10
	&DspSecurityList[3],	// Level 15
	&DspSecurityList[4],	// Level 20
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
static const Dcm_DspSecurityRowType *RWDID_SecurityList[] = {
	&DspSecurityList[2],    // Level 10
	&DspSecurityList[3],	// Level 15
	&DspSecurityList[4],	// Level 20
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
static const Dcm_DspSecurityRowType *RC_SecurityList[] = {
	&DspSecurityList[2],    // Level 10
	&DspSecurityList[3],	// Level 15
	&DspSecurityList[4],	// Level 20
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};

static const Dcm_DspSessionRowType *ALL_SessionList[] = {
	&DspSessionList[0],	// Session Default
	&DspSessionList[1],	// Session Program
	&DspSessionList[2],	// Session Extended
	&DspSessionList[3],	// Session Safety System
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
static const Dcm_DspSessionRowType *RWDID_SessionList[] = {
	&DspSessionList[1],	// Session Program
	&DspSessionList[2],	// Session Extended
	&DspSessionList[3],	// Session Safety System
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};

static const Dcm_DspSessionRowType *RC_SessionList[] = {
	&DspSessionList[1],	// Session Program
	&DspSessionList[2],	// Session Extended
	&DspSessionList[3],	// Session Safety System
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};

static const Dcm_DspDidReadType RWDID_DIDRead = {
    .DspDidReadSessionRef =  ALL_SessionList,
    .DspDidReadSecurityLevelRef =  UnProtected_SecurityList
};

static const Dcm_DspDidWriteType RWDID_DIDWrite = {
    .DspDidWriteSessionRef =  ALL_SessionList,
    .DspDidWriteSecurityLevelRef =  UnProtected_SecurityList
};

static const Dcm_DspDidInfoType DspDidInfoList[] = {
	{	// RWDID_010A
		.DspDidDynamicllyDefined =  FALSE,
		.DspDidFixedLength =  TRUE,
		.DspDidScalingInfoSize =  1,
		.DspDidAccess.DspDidRead  = &RWDID_DIDRead,
		.DspDidAccess.DspDidWrite = &RWDID_DIDWrite,
		.DspDidAccess.DspDidControl = NULL,
	}, 
};

extern const Dcm_DspDidType DspDidList[];
static const Dcm_DspDidType* NONE_RWDID_RefList[] =
{
    &DspDidList[DCM_DID_LIST_EOL_INDEX]  //add did ref by hand please,If you need it
};


const Dcm_DspDidType DspDidList[] = { 
	{ // RWDID,
		.DspDidUsePort =  FALSE,//Value is not configurable
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

static const Dcm_DspStartRoutineType EraseFlash_start = {
    .DspStartRoutineCtrlOptRecSize =  1,	// Flash Block Id
    .DspStartRoutineStsOptRecSize =  1		// Condition
};
static const Dcm_DspRoutineRequestResType EraseFlash_request = {
    .DspReqResRtnCtrlOptRecSize =  1	    // Erase Result
};
static const Dcm_DspSessionRowType *EraseFlash_sessionRefList[] = {
	&DspSessionList[1],//PRGS
	&DspSessionList[DCM_SESSION_EOL_INDEX]
};
static const Dcm_DspSecurityRowType *EraseFlash_securityRefList[] = {
	&DspSecurityList[2],//PRGS_SEC
	&DspSecurityList[DCM_SECURITY_EOL_INDEX]
};
static const Dcm_DspRoutineInfoType DspRoutineInfoList[] = {
	{//EraseFlash
		.DspRoutineAuthorization.DspRoutineSessionRef =  EraseFlash_sessionRefList,
		.DspRoutineAuthorization.DspRoutineSecurityLevelRef =  EraseFlash_securityRefList,
		.DspStartRoutine =  &EraseFlash_start,
		.DspRoutineStop =  NULL,
		.DspRoutineRequestRes =  &EraseFlash_request,
	},
};

const Dcm_DspRoutineType  DspRoutineList[] = {
	{//EraseFlash
		 .DspRoutineUsePort =  FALSE,
		 .DspRoutineIdentifier =  0x020A,
		 .DspRoutineInfoRef =  &DspRoutineInfoList[0],
		 .DspStartRoutineFnc =  Diag_EraseFlashStart,
		 .DspStopRoutineFnc =   NULL,
		 .DspRequestResultRoutineFnc =  Diag_EraseFlashRequestResult,
		 .Arc_EOL = FALSE
	},
	{
		 .Arc_EOL = TRUE
	}
};

//************************************************************************
//*							Memory Info             					*
//***********************************************************************
static const Dcm_DspMemoryRangeInfo DspMemoryRangeInfoList[]=
{
	{
		.MemoryAddressHigh = 0xFFFF,
		.MemoryAddressLow  = 0x0000,
		.pSecurityLevel    = UnProtected_SecurityList,
		.Arc_EOL           = FALSE
	},
	{
		.Arc_EOL           = TRUE
	}
};
static const Dcm_DspMemoryIdInfo DspMemoryIdInfoList[] =
{
	{
		.MemoryIdValue = 0xFF, // Reserved this ID for upload/download
		.pReadMemoryInfo  = DspMemoryRangeInfoList,  // For Upload
		.pWriteMemoryInfo = DspMemoryRangeInfoList,  // For Download
		.Arc_EOL = FALSE
	},
	{
		.Arc_EOL = TRUE
	}
};
static const Dcm_DspMemoryType DspMemory =
{
	.DcmDspUseMemoryId = TRUE,
	.DspMemoryIdInfo   = DspMemoryIdInfoList
};
static const Dcm_DspType Dsp = {
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
    .DspVehInfo =  NULL,
    .DspMemory  = &DspMemory
};

//************************************************************************
//*									DSD									*
//***********************************************************************

static const Dcm_DsdServiceType DIAG_P2PorP2A_serviceList[] = {
	{
		 .DsdSidTabServiceId = SID_DIAGNOSTIC_SESSION_CONTROL,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_SECURITY_ACCESS,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_READ_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = RWDID_SecurityList,
		 .DsdSidTabSessionLevelRef  = RWDID_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_WRITE_DATA_BY_IDENTIFIER,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = RWDID_SecurityList,
		 .DsdSidTabSessionLevelRef  = RWDID_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_ROUTINE_CONTROL,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = RC_SecurityList,
		 .DsdSidTabSessionLevelRef = RC_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_TESTER_PRESENT,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_ECU_RESET,
		 .DsdSidTabSubfuncAvail = True,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_REQUEST_DOWNLOAD,
		 .DsdSidTabSubfuncAvail = False,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_REQUEST_UPLOAD,
		 .DsdSidTabSubfuncAvail = False,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_REQUEST_TRANSFER_EXIT,
		 .DsdSidTabSubfuncAvail = False,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .DsdSidTabServiceId = SID_TRANSFER_DATA,
		 .DsdSidTabSubfuncAvail = False,
		 .DsdSidTabSecurityLevelRef = UnProtected_SecurityList,
		 .DsdSidTabSessionLevelRef = ALL_SessionList,
		 .Arc_EOL =  FALSE
	},
	{
		 .Arc_EOL =  TRUE
	}
};

static const Dcm_DsdServiceTableType DsdServiceTable[] = {
	{
		 .DsdSidTabId =  0,
		 .DsdService =  DIAG_P2PorP2A_serviceList,
		 .Arc_EOL =  FALSE
	},
	{
		 .Arc_EOL =  TRUE
	}
};

static const Dcm_DsdType Dsd = {
    .DsdServiceTable =  DsdServiceTable
};

//************************************************************************
//*									DSL									*
//***********************************************************************

static uint8 rxDcmBuffer_DiagP2P[512];
static Dcm_DslBufferRuntimeType rxBufferParams_DiagP2P =
{
	.status =  NOT_IN_USE
};
static uint8 txDcmBuffer_DiagP2P[512];
static Dcm_DslBufferRuntimeType txBufferParams_DiagP2P =
{
	.status =  NOT_IN_USE
};
static uint8 rxDcmBuffer_DiagP2A[512];
static Dcm_DslBufferRuntimeType rxBufferParams_DiagP2A =
{
	.status =  NOT_IN_USE
};
static uint8 txDcmBuffer_DiagP2A[512];
static Dcm_DslBufferRuntimeType txBufferParams_DiagP2A =
{
	.status =  NOT_IN_USE
};
static const Dcm_DslBufferType DcmDslBufferList[DCM_DSL_BUFFER_LIST_LENGTH] = {
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
		.externalBufferRuntimeData =  &rxBufferParams_DiagP2A
	},
	{
		.DslBufferID =  1,//? I am not that clear.
		.DslBufferSize =  512,//.?Value is not configurable
		.pduInfo.SduDataPtr =  txDcmBuffer_DiagP2A,
		.pduInfo.SduLength =  512,
		.externalBufferRuntimeData =  &txBufferParams_DiagP2A
	}
};

static const Dcm_DslCallbackDCMRequestServiceType DCMRequestServiceList[] = {
	{ // vRequestService_1
		.StartProtocol =  Diag_RequestServiceStart,  // must at least config one
		.StopProtocol =   Diag_RequestServiceStop,
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

static const Dcm_DslServiceRequestIndicationType DCMServiceRequestIndicationList[] = {
	{
		.Indication =  Diag_RequestServiceIndication, // can ignore this configure when turn DCM_REQUEST_INDICATION_ENABLED STD_OFF
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

extern const Dcm_DslMainConnectionType DslMainConnectionList[];

static const Dcm_DslProtocolRxType DcmDslProtocolRxList[] = {
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2P_REQ],
		.DslProtocolAddrType =  DCM_PROTOCOL_PHYSICAL_ADDR_TYPE,
		.DcmDslProtocolRxPduId =  CANTP_DIAG_P2P_REQ,
		.DcmDslProtocolRxTesterSourceAddr_v4 =  0,		//.Value is not configurable
		.DcmDslProtocolRxChannelId_v4 =  0,				//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2A_REQ],
		.DslProtocolAddrType =  DCM_PROTOCOL_PHYSICAL_ADDR_TYPE,
		.DcmDslProtocolRxPduId =  CANTP_DIAG_P2A_REQ,
		.DcmDslProtocolRxTesterSourceAddr_v4 =  0,		//.Value is not configurable
		.DcmDslProtocolRxChannelId_v4 =  0,				//.Value is not configurable
		.Arc_EOL =  FALSE
	},
	{
		.Arc_EOL =  TRUE
	}
};

static const Dcm_DslProtocolTxType DcmDslProtocolTxList[] = {
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2P_ACK],
		.DcmDslProtocolTxPduId =  CANTP_DIAG_P2P_ACK,
		.DcmDslProtocolDcmTxPduId = DCM_DIAG_P2P_ACK,
		.Arc_EOL =  FALSE
	},
	{
		.DslMainConnectionParent =  &DslMainConnectionList[DCM_DIAG_P2A_ACK],
		.DcmDslProtocolTxPduId =  CANTP_DIAG_P2A_ACK,
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

static Dcm_DslRunTimeProtocolParametersType dcmDslRuntimeVariables[2]; // for DIAG_P2P and DIAG_P2A
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

static const Dcm_DslProtocolType DslProtocol = {
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

static const Dcm_DslSessionControlType SessionControlList[] = {
	{
		 .GetSesChgPermission =  Diag_GetSesChgPer,
		 .ChangeIndication =  NULL,
		 .ConfirmationRespPend =  NULL,
		 .Arc_EOL =  FALSE
	},
	{
		 .Arc_EOL =  TRUE
	}
};

static const Dcm_DslDiagRespType DiagResp = {
    .DslDiagRespForceRespPendEn =  TRUE,
    .DslDiagRespMaxNumRespPend =  10
};

static const Dcm_DslType Dsl = {
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
