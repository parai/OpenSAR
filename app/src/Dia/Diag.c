#include "app.h"

static uint32  diagRandomSeed = 0xDEADBEEF;
Std_ReturnType Diag_GetSeed (uint8 *securityAccessDataRecord, uint8 *seed,
                        Dcm_NegativeResponseCodeType *errorCode)
{
	diagRandomSeed = (diagRandomSeed<<1u)-7;
    printf("Diag_GetSeed(seed=0x%x).\r\n",diagRandomSeed);
    seed[0] = (uint8)(diagRandomSeed>>24u);
    seed[1] = (uint8)(diagRandomSeed>>16u);
    seed[2] = (uint8)(diagRandomSeed>>8u);
    seed[3] = (uint8)(diagRandomSeed);
    *errorCode = E_OK;
    return E_OK;
}
Std_ReturnType Diag_CompKey (uint8 *key)
{
	uint32 diagKey,diagKey2;
	diagKey = ((uint32)key[0]<<24u)+((uint32)key[1]<<16u)+((uint32)key[2]<<8u)+((uint32)key[3]);
    diagKey2 = ((diagRandomSeed/7)<<3) - 111;
    printf("Diag_CompKey(key=0x%x).\r\n",diagKey);
    if(diagKey == diagKey2)
    {
    	return E_OK;
    }
    else
    {
    	return E_NOT_OK;
    }
}

Std_ReturnType Diag_RequestServiceStart (Dcm_ProtocolType protocolID)
{
     printf("in Diag_RequestServiceStart().\r\n");
    return E_OK;
}
Std_ReturnType Diag_RequestServiceStop (Dcm_ProtocolType protocolID)
{
    printf("in Diag_RequestServiceStop().\r\n");
    return E_OK;
}
Std_ReturnType Diag_RequestServiceIndication(uint8 *requestData, uint16 dataSize)
{
    printf("in  Diag_RequestServiceIndication().\r\n");
    return E_OK;
}

Std_ReturnType Diag_GetSesChgPer(Dcm_SesCtrlType sesCtrlTypeActive,
                                            Dcm_SesCtrlType sesCtrlTypeNew)
{
    printf("in Diag_GetSesChgPer().\r\n");
    return E_OK;
}

Std_ReturnType Diag_ConditionCheckRead(Dcm_NegativeResponseCodeType *errorCode){
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  Diag_ConditionCheckRead().\r\n");
    return E_OK;
}
Std_ReturnType Diag_ConditionCheckWrite(Dcm_NegativeResponseCodeType *errorCode){
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  Diag_ConditionCheckWrite().\r\n");
    return E_OK;
}

Std_ReturnType Diag_ReadDID_010A_Cbk(uint8 *data){
    int i;
    printf("in  Diag_ReadDID_010A_Cbk().\r\n");
    for(i=0;i<128;i++)
    {
        data[i] = i;
    }
    return E_OK;
}
Std_ReturnType Diag_WriteDID_010A_Cbk(uint8 *data, uint16 dataLength,
                Dcm_NegativeResponseCodeType *errorCode){
    int i;
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  Diag_WriteDID_010A_Cbk().\r\n[");
    for(i=0;i<dataLength;i++)
    {
        printf("0x%x,",data[i]);
    }
    printf("]\r\n");
    return E_OK;
}
Std_ReturnType vDid_1_GetScalingInfo_Cbk(uint8 *scalingInfo,
                Dcm_NegativeResponseCodeType *errorCode){
    *errorCode = DCM_E_POSITIVERESPONSE;
    scalingInfo[0] = 0xEE;
    printf("in  vDid_1_GetScalingInfo_Cbk().\r\n");
    return E_OK;
}
Std_ReturnType vDid_1_FreezeCurrentState_cbk(uint8 *controlOptionRecord,
        uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord,
        Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  vDid_1_FreezeCurrentState_cbk().\r\n");
    return E_OK;
}
Std_ReturnType vDid_1_ResetToDefault_Cbk(uint8 *controlOptionRecord,
        uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord,
        Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  vDid_1_ResetToDefault_Cbk().\r\n");
    return E_OK;
}
Std_ReturnType vDid_1_ReturnControl_Cbk(uint8 *controlOptionRecord,
        uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord,
        Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  vDid_1_ReturnControl_Cbk().\r\n");
    return E_OK;
}
Std_ReturnType vDid_1__ShortTermAdj_cbk(uint8 *controlOptionRecord,
        uint8 *controlEnableMaskRecord, uint8 *controlStatusRecord,
        Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    printf("in  vDid_1__ShortTermAdj_cbk().\r\n");
    return E_OK;
}

Std_ReturnType vRoutine_1_Start(uint8 *inBuffer, uint8 *outBuffer,
                            Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    outBuffer[0] = 0xBF;
    printf("in  vRoutine_1_Start().\r\n[");
    return E_OK;
}
Std_ReturnType vRoutine_1_Stop(uint8 *inBuffer, uint8 *outBuffer,
                            Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    outBuffer[0] = 0xEF;
    printf("in  vRoutine_1_Stop().\r\n[");
    return E_OK;
}
Std_ReturnType vRoutine_1_RequestResult(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
    *errorCode = DCM_E_POSITIVERESPONSE;
    outBuffer[0] = 0xDB;
    printf("in  vRoutine_1_RequestResult().\r\n[");
    return E_OK;
}
