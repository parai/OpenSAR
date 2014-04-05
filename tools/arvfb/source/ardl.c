// Data Link Layer
#include "arvfb.h"

void Can_RxIndication(const ArCanMsgType* armsg)
{
	ArTp_RxIndication(armsg);
	ArCom_RxIndication(armsg);
}

void Ardl_RxIndication(const ArMsgType *armsg)
{
	switch(armsg->Type)
	{
		case MSG_ON_CAN:
			Can_RxIndication((const ArCanMsgType*)armsg);
			break;
		default:
			break;
	}
}


void Ardl_Transmit(const ArMsgType *armsg)
{
	switch(armsg->Type)
	{
		case MSG_ON_CAN:
			Can_Transmit(armsg);
			break;
		default:
			break;
	}
}
