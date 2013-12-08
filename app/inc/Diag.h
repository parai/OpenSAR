

#ifndef DIAG_H_
#define DIAG_H_
#include "Std_Types.h"
#include "Dcm.h"
#include "Dcm_Internal.h"

extern Std_ReturnType Diag_GetSeed (uint8 *securityAccessDataRecord, uint8 *seed, Dcm_NegativeResponseCodeType *errorCode);
extern Std_ReturnType Diag_CompKey (uint8 *key);

extern Std_ReturnType Diag_RequestServiceStart (Dcm_ProtocolType protocolID);
extern Std_ReturnType Diag_RequestServiceStop (Dcm_ProtocolType protocolID);
extern Std_ReturnType Diag_RequestServiceIndication(uint8 *requestData, uint16 dataSize);

#endif /* DIAG_H_ */
