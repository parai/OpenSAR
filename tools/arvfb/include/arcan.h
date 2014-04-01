#ifndef AR_CAN_H_H_H
#define AR_CAN_H_H_H

typedef 	struct
{
	uint32 Type;
	uint32 Command;
	uint32 Length; // inherit from ArMsg
	struct{
		uint32 Identifier;
		uint8  DataLengthCode;
		uint8  Data[8];
	}Msg;
}ArCanMsgType;

GtkWidget* ArCan(void);

#endif

