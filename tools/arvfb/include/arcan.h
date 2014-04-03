#ifndef AR_CAN_H_H_H
#define AR_CAN_H_H_H

#define MSG_CAN_LENGTH   (14)

typedef 	struct
{
	uint32 Type;
	uint32 Command;
	uint32 Length; // inherit from ArMsg
	struct{
		uint32 Identifier;
		uint8  Data[8];
		uint8  DataLengthCode;
		uint8  BusID;
	}Msg;
}ArCanMsgType;

GtkWidget* ArCan(void);
void Can_RxIndication(const ArCanMsgType* armsg);
void Can_Transmit(const ArMsgType *armsg);

#endif

