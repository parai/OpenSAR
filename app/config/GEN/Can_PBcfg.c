

#include "Can.h"

static const Can_FilterMaskType vCanFilterMask0=
{
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
	{0,0,0,0,0,0,0,0},
	CAN_IDAM_2_32BIT
};

static const Can_HardwareObjectType CAN_CTRL_0_HOHCfgData[]=
{
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_0_HTH,
		CAN_OBJECT_TYPE_TRANSMIT,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_0_HRH,
		CAN_OBJECT_TYPE_RECEIVE,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		.Can_Arc_EOL = TRUE,
	}
};

static const Can_HardwareObjectType CAN_CTRL_1_HOHCfgData[]=
{
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_1_HTH,
		CAN_OBJECT_TYPE_TRANSMIT,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_1_HRH,
		CAN_OBJECT_TYPE_RECEIVE,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		.Can_Arc_EOL = TRUE,
	}
};

static const Can_HardwareObjectType CAN_CTRL_2_HOHCfgData[]=
{
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_2_HRH,
		CAN_OBJECT_TYPE_RECEIVE,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_2_HTH,
		CAN_OBJECT_TYPE_TRANSMIT,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		.Can_Arc_EOL = TRUE,
	}
};

static const Can_HardwareObjectType CAN_CTRL_3_HOHCfgData[]=
{
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_3_HRH,
		CAN_OBJECT_TYPE_RECEIVE,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_3_HTH,
		CAN_OBJECT_TYPE_TRANSMIT,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		.Can_Arc_EOL = TRUE,
	}
};

static const Can_HardwareObjectType CAN_CTRL_4_HOHCfgData[]=
{
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_4_HRH,
		CAN_OBJECT_TYPE_RECEIVE,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		CAN_ARC_HANDLE_TYPE_BASIC,
		CAN_ID_TYPE_STANDARD,
		0x00,
		CAN_CTRL_4_HTH,
		CAN_OBJECT_TYPE_TRANSMIT,
		&vCanFilterMask0,
		0x00000000,/* mbMask */
		FALSE
	},
	{
		.Can_Arc_EOL = TRUE,
	}
};

 const Can_ControllerConfigType  Can_ControllerCfgData[]=
{
	{
		CAN_CTRL_0,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		250, /* baudrate(kbs) */
		0, /* propseg(SJW) */
		12, /* seg1 */
		1, /* seg2 */
		CAN_CTRL_0_HOHCfgData,
		FALSE	/* LoopBack */
	},
	{
		CAN_CTRL_1,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		250, /* baudrate(kbs) */
		0, /* propseg(SJW) */
		12, /* seg1 */
		1, /* seg2 */
		CAN_CTRL_1_HOHCfgData,
		FALSE	/* LoopBack */
	},
	{
		CAN_CTRL_2,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		250, /* baudrate(kbs) */
		0, /* propseg(SJW) */
		12, /* seg1 */
		1, /* seg2 */
		CAN_CTRL_2_HOHCfgData,
		FALSE	/* LoopBack */
	},
	{
		CAN_CTRL_3,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		250, /* baudrate(kbs) */
		0, /* propseg(SJW) */
		12, /* seg1 */
		1, /* seg2 */
		CAN_CTRL_3_HOHCfgData,
		FALSE	/* LoopBack */
	},
	{
		CAN_CTRL_4,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		CAN_ARC_PROCESS_TYPE_INTERRUPT,
		250, /* baudrate(kbs) */
		0, /* propseg(SJW) */
		12, /* seg1 */
		1, /* seg2 */
		CAN_CTRL_4_HOHCfgData,
		FALSE	/* LoopBack */
	},
};
extern void CanIf_RxIndication(uint8 Hrh, Can_IdType CanId, uint8 CanDlc,const uint8 *CanSduPtr);
extern void CanIf_TxConfirmation(PduIdType canTxPduId);
const Can_CallbackType CanCallbackConfigData = {
     NULL, //CanIf_CancelTxConfirmation,
     CanIf_RxIndication,
     NULL,//CanIf_ControllerBusOff,
     CanIf_TxConfirmation,
     NULL, //CanIf_ControllerWakeup,
     NULL //CanIf_Arc_Error,
 };
 const Can_ConfigSetType Can_ConfigSetData ={Can_ControllerCfgData,&CanCallbackConfigData};
 const Can_ConfigType Can_ConfigData ={&Can_ConfigSetData};
