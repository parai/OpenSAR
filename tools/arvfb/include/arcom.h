
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


#endif /* ARCOM_H_ */
