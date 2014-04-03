#include "app.h"

static uint32  diagRandomSeed = 0xDEADBEEF;
static Std_ReturnType GetSeed (uint8 *securityAccessDataRecord, uint8 *seed,
                        Dcm_NegativeResponseCodeType *errorCode)
{
	diagRandomSeed = (diagRandomSeed<<1u)-7;
    seed[0] = (uint8)(diagRandomSeed>>24u);
    seed[1] = (uint8)(diagRandomSeed>>16u);
    seed[2] = (uint8)(diagRandomSeed>>8u);
    seed[3] = (uint8)(diagRandomSeed);
    *errorCode = E_OK;
    return E_OK;
}
static Std_ReturnType CompKey (uint8 *key)
{
	uint32 diagKey,diagKey2;
	diagKey = ((uint32)key[0]<<24u)+((uint32)key[1]<<16u)+((uint32)key[2]<<8u)+((uint32)key[3]);
    diagKey2 = ((diagRandomSeed/7)<<3) - 111;
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
    return E_OK;
}
Std_ReturnType Diag_RequestServiceStop (Dcm_ProtocolType protocolID)
{
    return E_OK;
}
Std_ReturnType Diag_RequestServiceIndication(uint8 *requestData, uint16 dataSize)
{
    return E_OK;
}

Std_ReturnType Diag_GetSesChgPer(Dcm_SesCtrlType sesCtrlTypeActive,
                                            Dcm_SesCtrlType sesCtrlTypeNew)
{
    return E_OK;
}

Std_ReturnType Diag_GetSeedPRGS(uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode)
{
	return GetSeed(securityAccessDataRecord,seed,errorCode);
}
Std_ReturnType Diag_CompareKeyPRGS(uint8 *key)
{
	return CompKey(key);
}
Std_ReturnType Diag_GetSeedEXTDS(uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode)
{
	return GetSeed(securityAccessDataRecord,seed,errorCode);
}
Std_ReturnType Diag_CompareKeyEXTDS(uint8 *key){
	return CompKey(key);
}

Std_ReturnType Diag_DidGetDataLengthFingerPrint(uint16 *didLength)
{
	*didLength = 128;
	return E_OK;
}
Std_ReturnType Diag_DidConditionReadCheckFingerPrint(Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	return E_OK;
}
Std_ReturnType Diag_DidReadDataFingerPrint(uint8 *data)
{
	for(int i=0;i<128;i++)
	{
		data[i] = i;
	}
	return E_OK;
}
Std_ReturnType Diag_DidConditionCheckWriteFingerPrint(Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	return E_OK;
}
Std_ReturnType Diag_DidWriteDataFingerPrint(uint8 *data, uint16 dataLength, Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	printf("FingerPrint = [");
	for(int i=0;i<dataLength;i++)
	{
		printf("%-2X,",data[i]);
	}
	printf("]\n");
	return E_OK;
}


Std_ReturnType Diag_StartRoutineFLErase(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	printf("In %s()\n",__FUNCTION__);
	return E_OK;
}
Std_ReturnType Diag_RequestResultRoutineFLErase(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	printf("In %s()\n",__FUNCTION__);
	return E_OK;
}
Std_ReturnType Diag_StartRoutineFLCheckProgrmIntegrity(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	printf("In %s()\n",__FUNCTION__);
	return E_OK;
}
Std_ReturnType Diag_RequestResultRoutineFLCheckProgrmIntegrity(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
	*errorCode = DCM_E_POSITIVERESPONSE;
	printf("In %s()\n",__FUNCTION__);
	return E_OK;
}


Std_ReturnType Diag_GetSeed (uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode)
{
	return GetSeed(securityAccessDataRecord,seed,errorCode);
}
Std_ReturnType Diag_CompareSeed (uint8 *key)
{
	return E_OK;
}
Std_ReturnType Diag_StartProtocolCbk (Dcm_ProtocolType protocolID)
{
	return E_OK;
}
Std_ReturnType Diag_StopProtocolCbk (Dcm_ProtocolType protocolID)
{
	return E_OK;
}
Std_ReturnType Diag_ProtocolIndicationCbk(uint8 *requestData, uint16 dataSize)
{
	return E_OK;
}

Std_ReturnType Diag_EraseFlashStart(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
	return E_OK;
}
Std_ReturnType Diag_EraseFlashResult(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode)
{
	return E_OK;
}

Std_ReturnType Diag_DID1_ShortTermAdjustment(uint8 *controlOptionRecord, uint8 *controlEnableMaskRecord,
	uint8 *controlStatusRecord, Dcm_NegativeResponseCodeType *errorCode)
{
	return E_OK;
}



