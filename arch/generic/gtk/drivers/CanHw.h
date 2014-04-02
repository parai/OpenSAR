/*
 * CanHw.h
 *
 *  Created on: 2014年4月2日
 *      Author: parai
 */

#ifndef CANHW_H_
#define CANHW_H_

// Bit maps in isrFlags
#define cCanIsrTx   (1u<<0)
#define cCanIsrRx   (1u<<1)
#define cCanIsrErr  (1u<<2)
#define cCanIsrWak  (1u<<3)

#define cCanMsgBoxSz 32


// bits in CANxRFLG:
// bits in CANxRIER:
#define BM_WUPI		0x80
#define BM_CSCI		0x40
#define BM_RSTAT1	0x20
#define BM_RSTAT0	0x10
#define BM_TSTAT1	0x08
#define BM_TSTAT0	0x04
#define BM_OVRI 	0x02
#define BM_RXF		0x01

// bits in CANxTFLG:
// bits in CANxTBSEL:
// Infact default 32 is in use
#define BM_TX2		0x04
#define BM_TX1		0x02
#define BM_TX0		0x01

typedef enum
{
	eCanMsgBoxIdle = 0,
	eCanMsgBoxBusy,
	eCanMsgBoxTxed,
	eCanMsgBoxRxed
}CanHwMsgBoxState_t;

typedef struct
{
	Can_IdType 		id;
	uint8      		dlc;
	uint8      		data[8];
	CanHwMsgBoxState_t state;
}CanHwMsgBox_t;
typedef struct
{
	uint8       dlc;
	uint32 		id;
	uint8       data[8];
	PduIdType   swPduHandle;
}GtkCanMsgBox_Type;

typedef struct
{
	uint8              head;
	uint8              tail;
	GtkCanMsgBox_Type  msgBox[cCanMsgBoxSz];
}GtkCanQueue_Type;

typedef struct{
	GtkCanQueue_Type rxQ;
	GtkCanQueue_Type txQ;
	CanHwMsgBox_t 	rxMsg[cCanMsgBoxSz];
	CanHwMsgBox_t 	txMsg[cCanMsgBoxSz];

	// Simulate MSCAN registers
	vuint8_t      RIER;
	vuint8_t      TIER;
	vuint8_t      IRQF;  // IRQ Flags

	boolean       isInit;
}CAN_HW_t;

extern CAN_HW_t Can_HwUnit[CAN_CONTROLLER_CNT];

static inline GtkCanMsgBox_Type* GtkCanGetBusyMsgBox(GtkCanQueue_Type* Q)
{
	GtkCanMsgBox_Type* pMsgBox = NULL;
	uint8 head = Q->head;
	if(head < (cCanMsgBoxSz-1))
	{
		head ++;
	}
	else
	{
		head = 0;
	}

	if(Q->head == Q->tail)
	{ // Empty
		pMsgBox = NULL;
	}
	else
	{
		pMsgBox = &(Q->msgBox[Q->head]);
		Q->head = head;
	}
	return pMsgBox;
}

static inline GtkCanMsgBox_Type* GtkCanGetEmptyMsgBox(GtkCanQueue_Type* Q)
{
	GtkCanMsgBox_Type* pMsgBox = NULL;
	uint8 tail = Q->tail;
	if(tail < (cCanMsgBoxSz-1))
	{
		tail ++;
	}
	else
	{
		tail = 0;
	}

	if(tail == Q->head)
	{ // FULL
		pMsgBox = NULL;
	}
	else
	{
		pMsgBox = &(Q->msgBox[Q->tail]);
		Q->tail = tail;
	}
	return pMsgBox;
}

extern void Can_SetPduHandle(uint8 ctrl,PduIdType swPduHandle);
void Can_0_IsrEntry(void);
void Can_1_IsrEntry(void);
void Can_2_IsrEntry(void);
void Can_3_IsrEntry(void);
void Can_4_IsrEntry(void);

void Can_0_RxIsr( void  );
void Can_1_RxIsr( void  );
void Can_2_RxIsr( void  );
void Can_3_RxIsr( void  );
void Can_4_RxIsr( void  );

void Can_0_TxIsr( void  );
void Can_1_TxIsr( void  );
void Can_2_TxIsr( void  );
void Can_3_TxIsr( void  );
void Can_4_TxIsr( void  );

void Can_0_ErrIsr( void  );
void Can_1_ErrIsr( void  );
void Can_2_ErrIsr( void  );
void Can_3_ErrIsr( void  );
void Can_4_ErrIsr( void  );

void Can_0_WakeIsr( void  );
void Can_1_WakeIsr( void  );
void Can_2_WakeIsr( void  );
void Can_3_WakeIsr( void  );
void Can_4_WakeIsr( void  );

#endif /* CANHW_H_ */
