
#ifndef ARTP_H_
#define ARTP_H_

void ArTp_Init(void);
Std_ReturnType CanTp_Transmit(uint8* data,uint16 size);
void CanTp_RxIndication(uint8* data,uint16 length);
void ArTp_RxIndication(const ArCanMsgType* armsg);

void ArTp_Schedule(void);

#endif /* ARTP_H_ */
