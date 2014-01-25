
#ifndef GTKTP_H_
#define GTKTP_H_

#include "GtkCan.h"

extern void CanTp_Init(void);
extern Std_ReturnType CanTp_Transmit(uint8* data,uint16 size);
extern void CanTp_RxIndication(uint8* data,uint16 length);

#endif /* GTKTP_H_ */
