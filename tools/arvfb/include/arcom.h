
// In fact, arvfb is a Virtual Function Bus simulator, it is in charge of
// the data transition from one ECU to the other.
// So arcom only need to display the signals on the VFB, it is not in charge
// of signals' transition, but for debug purpose of AUTOSAR module,
// Somehow, arcom will both display signals and in charge of signals' transition.
#ifndef ARCOM_H_
#define ARCOM_H_


GtkWidget* ArCom(void);

void ArCom_Init(void);
void ArCom_Schedule(void);

void ArCom_RxIndication(const ArCanMsgType* armsg);

void ArCom_DefinePdu( char* Name,uint32 Identifier,uint8 BusID,boolean IsTxEnabled,uint32 Period);

void ArCom_DefineSignal( char* Name,uint8 StartBit,uint8 BitSize,uint32 DefaultValue);

#endif /* ARCOM_H_ */
