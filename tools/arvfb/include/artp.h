
#ifndef ARTP_H_
#define ARTP_H_

extern void CanTp_Init(void);
extern Std_ReturnType CanTp_Transmit(uint8* data,uint16 size);
extern void CanTp_RxIndication(uint8* data,uint16 length);
void ArTp_RxIndication(const ArCanMsgType* armsg);

#endif /* ARTP_H_ */
