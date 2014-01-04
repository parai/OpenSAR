

#ifndef DIAG_H_
#define DIAG_H_
#include "Std_Types.h"
#include "Dcm.h"
#include "Dcm_Internal.h"
extern Std_ReturnType Diag_GetSesChgPer(Dcm_SesCtrlType sesCtrlTypeActive,Dcm_SesCtrlType sesCtrlTypeNew);

extern Std_ReturnType Diag_GetSeed (uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType Diag_CompKey (uint8 *key);

extern Std_ReturnType Diag_RequestServiceStart (Dcm_ProtocolType protocolID);
extern Std_ReturnType Diag_RequestServiceStop (Dcm_ProtocolType protocolID);
extern Std_ReturnType Diag_RequestServiceIndication(uint8 *requestData, uint16 dataSize);

extern Std_ReturnType Diag_ConditionCheckRead (Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType Diag_ConditionCheckWrite(Dcm_NegativeResponseCodeType *errorCode);

extern Std_ReturnType Diag_ReadDID_010A_Cbk(uint8 *data);
extern Std_ReturnType Diag_WriteDID_010A_Cbk(uint8 *data, uint16 dataLength,Dcm_NegativeResponseCodeType *errorCode);


extern Std_ReturnType Diag_EraseFlashStart(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType Diag_EraseFlashStop(uint8 *inBuffer, uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType Diag_EraseFlashRequestResult(uint8 *outBuffer, Dcm_NegativeResponseCodeType *errorCode);

#endif /* DIAG_H_ */
