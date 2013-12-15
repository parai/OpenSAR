

#include "Com.h"
#include "Com_Internal.h"
//#include <stdlib.h>
#if defined(USE_PDUR)
#include "PduR.h"
#endif

//Signal init values.
const uint8 vCom_IPdu1_TX_Signal0_InitValue[3] ={0x00, 0x00,0x00,};
const uint8 vCom_IPdu1_TX_SigGrp0_Signal0_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_TX_SigGrp0_Signal1_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_TX_SigGrp0_Signal2_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_TX_SigGrp0_Signal3_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_RX_Signal0_InitValue[3] ={0x00, 0x00,0x00,};
const uint8 vCom_IPdu1_RX_Signal1_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_RX_SigGrp0_Signal0_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_RX_SigGrp0_Signal1_InitValue[1] ={0x00, };
const uint8 vCom_IPdu1_RX_SigGrp0_Signal2_InitValue[1] ={0x00, };

//Group signal definitions
#if(COM_N_GROUP_SIGNALS > 0)
const ComGroupSignal_type ComGroupSignal[] = {
	{
		.ComBitPosition= 24,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_TX_SigGrp0_Signal0,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_TX_SigGrp0_Signal0_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= FALSE
	},
	{
		.ComBitPosition= 32,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_TX_SigGrp0_Signal1,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_TX_SigGrp0_Signal1_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= FALSE
	},
	{
		.ComBitPosition= 40,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_TX_SigGrp0_Signal2,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_TX_SigGrp0_Signal2_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= FALSE
	},
	{
		.ComBitPosition= 48,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_TX_SigGrp0_Signal3,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_TX_SigGrp0_Signal3_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= FALSE
	},
	{
		.ComBitPosition= 0,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_RX_SigGrp0_Signal0,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_RX_SigGrp0_Signal0_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= FALSE
	},
	{
		.ComBitPosition= 8,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_RX_SigGrp0_Signal1,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_RX_SigGrp0_Signal1_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= FALSE
	},
	{
		.ComBitPosition= 16,
		.ComBitSize= 8,
		.ComHandleId= vCom_IPdu1_RX_SigGrp0_Signal2,
		.ComSignalEndianess= COM_BIG_ENDIAN,
		.ComSignalInitValue= &vCom_IPdu1_RX_SigGrp0_Signal2_InitValue,
		.ComSignalType= UINT8_N,
		.Com_Arc_EOL= TRUE
	},
};

//SignalGroup GroupSignals lists.
const ComGroupSignal_type * const vCom_IPdu1_TX_SigGrp0_SignalRefs[] = {
	&ComGroupSignal[ vCom_IPdu1_TX_SigGrp0_Signal0 ],
	&ComGroupSignal[ vCom_IPdu1_TX_SigGrp0_Signal1 ],
	&ComGroupSignal[ vCom_IPdu1_TX_SigGrp0_Signal2 ],
	&ComGroupSignal[ vCom_IPdu1_TX_SigGrp0_Signal3 ],
	NULL
};
const ComGroupSignal_type * const vCom_IPdu1_RX_SigGrp0_SignalRefs[] = {
	&ComGroupSignal[ vCom_IPdu1_RX_SigGrp0_Signal0 ],
	&ComGroupSignal[ vCom_IPdu1_RX_SigGrp0_Signal1 ],
	&ComGroupSignal[ vCom_IPdu1_RX_SigGrp0_Signal2 ],
	NULL
};
#endif	 // COM_N_GROUP_SIGNALS

//IPdu buffers and signal group buffers
uint8 vCom_IPdu1_TX_IPduBuffer[8];
uint8 vCom_IPdu1_TX_SigGrp0_SignalGroupBuffer[8];
//I am not that clear,If problem,please modify by hand
const uint8 vCom_IPdu1_TX_SigGrp0_SignalGroupMaskBuffer[8] = {0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,};
uint8 vCom_IPdu1_RX_IPduBuffer[8];
uint8 vCom_IPdu1_RX_IPduDeferredRxBuffer[8];
uint8 vCom_IPdu1_RX_SigGrp0_SignalGroupBuffer[8];
//I am not that clear,If problem,please modify by hand
const uint8 vCom_IPdu1_RX_SigGrp0_SignalGroupMaskBuffer[8] = {0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,};

//Signal definitions
const ComSignal_type ComSignal[] = {
	{
		.ComBitPosition =  0,
		.ComBitSize =  24,
		.ComErrorNotification =  NULL,
		.ComFirstTimeoutFactor =  10,
		.ComHandleId =  vCom_IPdu1_TX_Signal0,
		.ComNotification =  NULL,
		.ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_NONE,
		.ComSignalEndianess =  COM_BIG_ENDIAN,
		.ComSignalInitValue =  &vCom_IPdu1_TX_Signal0_InitValue,
		.ComSignalType =  UINT8_N,
		.ComTimeoutFactor =  10,
		.ComTimeoutNotification =  NULL,
		.ComTransferProperty =  TRIGGERED,
		.ComUpdateBitPosition =  56,
		.ComSignalArcUseUpdateBit =  TRUE,
		.Com_Arc_IsSignalGroup =  FALSE,
		.ComGroupSignal =  NULL,
		.Com_Arc_ShadowBuffer =  NULL,
		.Com_Arc_ShadowBuffer_Mask =  NULL,
		.ComIPduHandleId =  vCom_IPdu1_TX,
		.Com_Arc_EOL =  FALSE
	},
	{
		.ComBitPosition =  24,
		.ComBitSize =  32,
		.ComErrorNotification =  NULL,
		.ComFirstTimeoutFactor =  10,
		.ComHandleId =  vCom_IPdu1_TX_SigGrp0,
		.ComNotification =  NULL,
		.ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_NONE,
		.ComSignalEndianess =  COM_OPAQUE,
		.ComSignalInitValue =  NULL,
		.ComSignalType =  UINT8_N,
		.ComTimeoutFactor =  10,
		.ComTimeoutNotification =  NULL,
		.ComTransferProperty =  TRIGGERED,
		.ComUpdateBitPosition =  57,
		.ComSignalArcUseUpdateBit =  TRUE,
		.Com_Arc_IsSignalGroup =  TRUE,
		.ComGroupSignal =  vCom_IPdu1_TX_SigGrp0_SignalRefs,
		.Com_Arc_ShadowBuffer =  vCom_IPdu1_TX_SigGrp0_SignalGroupBuffer,
		.Com_Arc_ShadowBuffer_Mask =  vCom_IPdu1_TX_SigGrp0_SignalGroupMaskBuffer,
		.ComIPduHandleId =  vCom_IPdu1_TX,
		.Com_Arc_EOL =  FALSE
	},
	{
		.ComBitPosition =  0,
		.ComBitSize =  24,
		.ComErrorNotification =  NULL,
		.ComFirstTimeoutFactor =  10,
		.ComHandleId =  vCom_IPdu1_RX_Signal0,
		.ComNotification =  onSignal0Receive,
		.ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_NONE,
		.ComSignalEndianess =  COM_BIG_ENDIAN,
		.ComSignalInitValue =  &vCom_IPdu1_RX_Signal0_InitValue,
		.ComSignalType =  UINT8_N,
		.ComTimeoutFactor =  10,
		.ComTimeoutNotification =  NULL,
		.ComTransferProperty =  TRIGGERED,
		.ComUpdateBitPosition =  56,
		.ComSignalArcUseUpdateBit =  TRUE,
		.Com_Arc_IsSignalGroup =  FALSE,
		.ComGroupSignal =  NULL,
		.Com_Arc_ShadowBuffer =  NULL,
		.Com_Arc_ShadowBuffer_Mask =  NULL,
		.ComIPduHandleId =  vCom_IPdu1_RX,
		.Com_Arc_EOL =  FALSE
	},
	{
		.ComBitPosition =  24,
		.ComBitSize =  8,
		.ComErrorNotification =  NULL,
		.ComFirstTimeoutFactor =  10,
		.ComHandleId =  vCom_IPdu1_RX_Signal1,
		.ComNotification =  onSignal1Receive,
		.ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_NONE,
		.ComSignalEndianess =  COM_BIG_ENDIAN,
		.ComSignalInitValue =  &vCom_IPdu1_RX_Signal1_InitValue,
		.ComSignalType =  UINT8_N,
		.ComTimeoutFactor =  10,
		.ComTimeoutNotification =  NULL,
		.ComTransferProperty =  TRIGGERED,
		.ComUpdateBitPosition =  57,
		.ComSignalArcUseUpdateBit =  TRUE,
		.Com_Arc_IsSignalGroup =  FALSE,
		.ComGroupSignal =  NULL,
		.Com_Arc_ShadowBuffer =  NULL,
		.Com_Arc_ShadowBuffer_Mask =  NULL,
		.ComIPduHandleId =  vCom_IPdu1_RX,
		.Com_Arc_EOL =  FALSE
	},
	{
		.ComBitPosition =  32,
		.ComBitSize =  24,
		.ComErrorNotification =  NULL,
		.ComFirstTimeoutFactor =  10,
		.ComHandleId =  vCom_IPdu1_RX_SigGrp0,
		.ComNotification =  onSingnalGroup0Receive,
		.ComRxDataTimeoutAction =  COM_TIMEOUT_DATA_ACTION_NONE,
		.ComSignalEndianess =  COM_OPAQUE,
		.ComSignalInitValue =  NULL,
		.ComSignalType =  UINT8_N,
		.ComTimeoutFactor =  10,
		.ComTimeoutNotification =  NULL,
		.ComTransferProperty =  TRIGGERED,
		.ComUpdateBitPosition =  58,
		.ComSignalArcUseUpdateBit =  TRUE,
		.Com_Arc_IsSignalGroup =  TRUE,
		.ComGroupSignal =  vCom_IPdu1_RX_SigGrp0_SignalRefs,
		.Com_Arc_ShadowBuffer =  vCom_IPdu1_RX_SigGrp0_SignalGroupBuffer,
		.Com_Arc_ShadowBuffer_Mask =  vCom_IPdu1_RX_SigGrp0_SignalGroupMaskBuffer,
		.ComIPduHandleId =  vCom_IPdu1_RX,
		.Com_Arc_EOL =  TRUE
	},
};

//I-PDU group definitions
const ComIPduGroup_type ComIPduGroup[] = {
	{
		.ComIPduGroupHandleId =  vCom_IPduGrp0,
		.Com_Arc_EOL =  TRUE
	},
};

//IPdu signal lists.
const ComSignal_type * const vCom_IPdu1_TX_SignalRefs[] = {
	&ComSignal[ vCom_IPdu1_TX_Signal0 ],
	&ComSignal[ vCom_IPdu1_TX_SigGrp0 ],
	NULL
};
const ComSignal_type * const vCom_IPdu1_RX_SignalRefs[] = {
	&ComSignal[ vCom_IPdu1_RX_Signal0 ],
	&ComSignal[ vCom_IPdu1_RX_Signal1 ],
	&ComSignal[ vCom_IPdu1_RX_SigGrp0 ],
	NULL
};
//I-PDU definitions
const ComIPdu_type ComIPdu[] = {
	{// vCom_IPdu1_TX
		.ComIPduCallout =  NULL,
		.ArcIPduOutgoingId =  0x000,
		.ComIPduSignalProcessing =  DEFERRED,
		.ComIPduSize =  8,
		.ComIPduDirection =  SEND,
		.ComIPduGroupRef =  vCom_IPduGrp0,
		.ComTxIPdu ={
			.ComTxIPduMinimumDelayFactor =  1,
			.ComTxIPduUnusedAreasDefault =  0,
			.ComTxModeTrue ={
				.ComTxModeMode =   DIRECT,
				.ComTxModeNumberOfRepetitions =   0,
				.ComTxModeRepetitionPeriodFactor =   10,
				.ComTxModeTimeOffsetFactor =   20,
				.ComTxModeTimePeriodFactor =   10,
			},
		},
		.ComIPduDataPtr =  vCom_IPdu1_TX_IPduBuffer,
		.ComIPduDeferredDataPtr =  NULL,
		.ComIPduSignalRef =  vCom_IPdu1_TX_SignalRefs,
		.ComIPduDynSignalRef =  NULL,
		.Com_Arc_EOL =  FALSE,
	},
	{// vCom_IPdu1_RX
		.ComIPduCallout =  NULL,
		.ArcIPduOutgoingId =  0x0000,
		.ComIPduSignalProcessing =  DEFERRED,
		.ComIPduSize =  8,
		.ComIPduDirection =  RECEIVE,
		.ComIPduGroupRef =  vCom_IPduGrp0,
		.ComTxIPdu ={
			.ComTxIPduMinimumDelayFactor =  1,
			.ComTxIPduUnusedAreasDefault =  0,
			.ComTxModeTrue ={
				.ComTxModeMode =   PERIODIC,
				.ComTxModeNumberOfRepetitions =   0,
				.ComTxModeRepetitionPeriodFactor =   10,
				.ComTxModeTimeOffsetFactor =   20,
				.ComTxModeTimePeriodFactor =   10,
			},
		},
		.ComIPduDataPtr =  vCom_IPdu1_RX_IPduBuffer,
		.ComIPduDeferredDataPtr =  vCom_IPdu1_RX_IPduDeferredRxBuffer,
		.ComIPduSignalRef =  vCom_IPdu1_RX_SignalRefs,
		.ComIPduDynSignalRef =  NULL,
		.Com_Arc_EOL =  FALSE,
	},
};

const Com_ConfigType ComConfiguration = {
	.ComConfigurationId =  1,
	.ComIPdu =  ComIPdu,
	.ComIPduGroup =  ComIPduGroup,
	.ComSignal =  ComSignal,
#if(COM_N_GROUP_SIGNALS > 0)
	.ComGroupSignal =  ComGroupSignal
#else
	.ComGroupSignal =  NULL
#endif
};

Com_Arc_IPdu_type Com_Arc_IPdu[] = {
	{ // vCom_IPdu1_TX
		.Com_Arc_TxIPduTimers ={
			.ComTxIPduNumberOfRepetitionsLeft =  0,
			.ComTxModeRepetitionPeriodTimer =  0,
			.ComTxIPduMinimumDelayTimer =  0,
			.ComTxModeTimePeriodTimer =  0
		},
		.Com_Arc_IpduStarted =  0
	},
	{ // vCom_IPdu1_RX
		.Com_Arc_TxIPduTimers ={
			.ComTxIPduNumberOfRepetitionsLeft =  0,
			.ComTxModeRepetitionPeriodTimer =  0,
			.ComTxIPduMinimumDelayTimer =  0,
			.ComTxModeTimePeriodTimer =  0
		},
		.Com_Arc_IpduStarted =  0
	},
};

Com_Arc_Signal_type Com_Arc_Signal[] = {
	{ // vCom_IPdu1_TX_Signal0
		.Com_Arc_DeadlineCounter =  0,
		.ComSignalUpdated =  0,
	},
	{ // vCom_IPdu1_TX_SigGrp0
		.Com_Arc_DeadlineCounter =  0,
		.ComSignalUpdated =  0,
	},
	{ // vCom_IPdu1_RX_Signal0
		.Com_Arc_DeadlineCounter =  0,
		.ComSignalUpdated =  0,
	},
	{ // vCom_IPdu1_RX_Signal1
		.Com_Arc_DeadlineCounter =  0,
		.ComSignalUpdated =  0,
	},
	{ // vCom_IPdu1_RX_SigGrp0
		.Com_Arc_DeadlineCounter =  0,
		.ComSignalUpdated =  0,
	},
};

#if(COM_N_GROUP_SIGNALS > 0)
Com_Arc_GroupSignal_type Com_Arc_GroupSignal[COM_N_GROUP_SIGNALS];
#endif

const Com_Arc_Config_type Com_Arc_Config = {
	.ComIPdu =  Com_Arc_IPdu,
	.ComSignal =  Com_Arc_Signal,
#if(COM_N_GROUP_SIGNALS > 0)
	.ComGroupSignal =  Com_Arc_GroupSignal
#else
    .ComGroupSignal =  NULL
#endif
};

