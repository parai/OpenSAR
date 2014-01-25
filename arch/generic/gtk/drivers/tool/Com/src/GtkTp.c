#include "GtkCan.h"

// ======================== TYPE     =============================
typedef enum
{
	CANTP_ST_IDLE = 0,
	CANTP_ST_START_TO_SEND,
	CANTP_ST_SENDING,
	CANTP_ST_WAIT_FC,
	CANTP_ST_WAIT_CF,
	CANTP_ST_SEND_CF,
	CANTP_ST_SEND_FC
}GtkTpState_Type;

typedef struct
{
	guchar rxBuffer[1024*64];
	guint  rxLength;
	guint  rxIndex;
	guint  rxId;

	guchar txBuffer[1024*64];
	guint  txLength;
	guint  txIndex;
	guint  txId;

	uint8  BS;
	uint8  SN;	// sequence number
	uint8  STMin;

	GtkTpState_Type state;

}GtkTp_Type;
// ======================== MACRO    =============================
#define ISO15765_TPCI_MASK      0x30
#define ISO15765_TPCI_SF        0x00  /* Single Frame */
#define ISO15765_TPCI_FF        0x10  /* First Frame */
#define ISO15765_TPCI_CF        0x20  /* Consecutive Frame */
#define ISO15765_TPCI_FC        0x30  /* Flow Control */
#define ISO15765_TPCI_DL        0x7   /* Single frame data length mask */
#define ISO15765_TPCI_FS_MASK   0x0F  /* Flowcontrol status mask */


#define ISO15765_FLOW_CONTROL_STATUS_CTS        0
#define ISO15765_FLOW_CONTROL_STATUS_WAIT       1
#define ISO15765_FLOW_CONTROL_STATUS_OVFLW      2

#define cfgSTmin  10
#define cfgBS     8

// ======================== DATA     =============================
static GtkTp_Type sGtkTp;
static GTimer* pSysTimer;
static gboolean isSysTimerStarted;

// ======================== FUNCTION =============================
static void     gtk_tp_tx(GSocketConnection *connection);
static void     gtk_tp_rx(GtkCanMsg_Type* pMsg,gssize size);
static gboolean gtk_tp_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data);
static void     gtk_tp_server_init(void);
static void     gtk_tp_init(void);
static gboolean CanTp_MainFunction(gpointer data);


static void     StartTimer(void);
static void     StopTimer(void);
static gboolean IsTimerElapsed(gulong microseconds);

static void SendSF(GOutputStream * ostream);
static void SendFF(GOutputStream * ostream);
static void SendFC(GOutputStream * ostream);
static void SendCF(GOutputStream * ostream);
static void SendStart(GOutputStream * ostream);
static void HandleSF(uint8* data,uint16 size);
static void HandleFF(uint8* data,uint16 size);
static void HandleCF(uint8* data,uint16 size);
static void HandleFC(uint8* data,uint16 size);

// ======================== FUNCTION =============================
static void gtk_tp_tx(GSocketConnection *connection)
{
	GError* error;
	static const gchar dumy_ack = GTK_CAN_CMD_TX_ACK;
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	switch(sGtkTp.state)
	{
		case CANTP_ST_START_TO_SEND:
			SendStart(ostream);
			break;
		case CANTP_ST_SEND_FC:
			SendFC(ostream);
			break;
		case CANTP_ST_SEND_CF:
			SendCF(ostream);
			break;
		default:
			g_output_stream_write  (ostream,
									&dumy_ack, /* your message goes here */
									1, /* length of your message */
									NULL,
									&error);
			break;
	}

	// Timer process
	switch(sGtkTp.state)
	{
		case CANTP_ST_START_TO_SEND:
		case CANTP_ST_SEND_FC:
		case CANTP_ST_SEND_CF:
			StartTimer();
			break;
		default:
			break;
	}


}

static void gtk_tp_rx(GtkCanMsg_Type* pMsg,gssize size)
{
	if((sGtkTp.rxId == pMsg->id) && (sizeof(GtkCanMsg_Type)==size))
	{
		switch((pMsg->data[0]&ISO15765_TPCI_MASK))
		{
			case (ISO15765_TPCI_SF):
				HandleSF(pMsg->data,pMsg->dlc);
				break;
			case (ISO15765_TPCI_FF):
				HandleFF(pMsg->data,pMsg->dlc);
				break;
			case (ISO15765_TPCI_CF):
				HandleCF(pMsg->data,pMsg->dlc);
				break;
			case (ISO15765_TPCI_FC):
				HandleFC(pMsg->data,pMsg->dlc);
				break;
			default:
				break;
		}
		// Timer process
		switch((pMsg->data[0]&ISO15765_TPCI_MASK))
		{
			case (ISO15765_TPCI_SF):
			case (ISO15765_TPCI_FF):
			case (ISO15765_TPCI_CF):
			case (ISO15765_TPCI_FC):
				StartTimer();
				break;
			default:
				break;
		}
	}
}
static gboolean gtk_tp_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data)
{
	GError * error = NULL;
	GtkCanMsg_Type gtkCanMsg;
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	gssize size = g_input_stream_read(istream,&gtkCanMsg,sizeof(GtkCanMsg_Type),NULL,&error);
	switch(gtkCanMsg.cmd)
	{
		case GTK_CAN_CMD_TX:
			gtk_tp_tx(connection);
			break;
		case GTK_CAN_CMD_RX:
			gtk_tp_rx(&gtkCanMsg,size);
			break;
		default:
			g_print("CAN:Error Command!\n");
			break;
	}
	return FALSE;
}

static void gtk_tp_server_init(void)
{
	GError * error = NULL;
	GSocketService * pGSocketService;

	pGSocketService = g_socket_service_new ();
	/* connect to the port */
	g_socket_listener_add_inet_port (pGSocketService,
								8999, /* your port goes here */
								NULL,
								&error);

	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("GtkUds: Init Error <%s>\n!",error->message);
	}
	else
	{
		g_print("GtkUds: Listener on localhost:%d\n",8999);
	}

	/* listen to the 'incoming' signal */
	g_signal_connect (pGSocketService,
				"incoming",
				G_CALLBACK (gtk_tp_incoming_callback),
				NULL);

	/* start the socket service */
	g_socket_service_start (pGSocketService);
}

static void gtk_tp_init(void)
{
	memset(&sGtkTp,0,sizeof(sGtkTp));

	// Config
	sGtkTp.rxId = 0x732;
	sGtkTp.txId = 0x731;

	gtk_tp_server_init();
}

static gboolean CanTp_MainFunction(gpointer data)
{
	switch(sGtkTp.state)
	{
		case CANTP_ST_IDLE:
			break;
		case CANTP_ST_START_TO_SEND:
		case CANTP_ST_SENDING:
		case CANTP_ST_WAIT_FC:
		case CANTP_ST_WAIT_CF:
		case CANTP_ST_SEND_CF:
		case CANTP_ST_SEND_FC:
			if(IsTimerElapsed(5000))
			{
				sGtkTp.state = CANTP_ST_IDLE;
				StopTimer();
			}
			break;
		default:
			break;
	}
	return TRUE;
}


static void StartTimer(void)
{
	g_timer_start(pSysTimer);
	isSysTimerStarted = TRUE;
}

static void StopTimer(void)
{
	g_timer_stop(pSysTimer);
	isSysTimerStarted = FALSE;
}
static gboolean IsTimerElapsed(gulong microseconds)
{
	gboolean rv;
	if(isSysTimerStarted)
	{
		gdouble elapsed;
		gulong  elapsed_microseconds;

		elapsed = g_timer_elapsed(pSysTimer,NULL);
		elapsed_microseconds = (elapsed*1000);

		if(elapsed_microseconds>microseconds)
		{
			rv = TRUE;
		}
		else
		{
			rv = FALSE;
		}
	}
	else
	{
		rv = TRUE;
	}
	return rv;
}
static void SendSF(GOutputStream * ostream)
{
	GError *error;
	GtkCanMsg_Type gtkCanMsg;
	gtkCanMsg.cmd = GTK_CAN_CMD_TX_ACK;
	gtkCanMsg.id  = sGtkTp.txId;
	gtkCanMsg.dlc = 8;
	gtkCanMsg.data[0] = sGtkTp.txLength|ISO15765_TPCI_SF;

	memcpy(&(gtkCanMsg.data[1]),sGtkTp.txBuffer,sGtkTp.txLength);

	g_output_stream_write  (ostream,
							&gtkCanMsg,
							sizeof(GtkCanMsg_Type),
							NULL,
							&error);
	sGtkTp.state = CANTP_ST_IDLE;
}
static void SendFF(GOutputStream * ostream)
{
	GError *error;
	GtkCanMsg_Type gtkCanMsg;
	gtkCanMsg.cmd = GTK_CAN_CMD_TX_ACK;
	gtkCanMsg.id  = sGtkTp.txId;
	gtkCanMsg.dlc = 8;
	gtkCanMsg.data[0] = ((sGtkTp.txLength>>8)&0x0F)|ISO15765_TPCI_FF;
	gtkCanMsg.data[1] = (sGtkTp.txLength&0xFF);
	memcpy(&(gtkCanMsg.data[2]),sGtkTp.txBuffer,6);
	sGtkTp.txIndex = 6;
	g_output_stream_write  (ostream,
							&gtkCanMsg,
							sizeof(GtkCanMsg_Type),
							NULL,
							&error);
	sGtkTp.state = CANTP_ST_WAIT_FC;
	sGtkTp.SN = 0;
}
static void SendCF(GOutputStream * ostream)
{
	GError *error;
	GtkCanMsg_Type gtkCanMsg;
	uint16 lsize = sGtkTp.txLength-sGtkTp.txIndex;
	if(lsize>7)
	{
		lsize=7;
	}
	gtkCanMsg.cmd = GTK_CAN_CMD_TX_ACK;
	gtkCanMsg.id  = sGtkTp.txId;
	gtkCanMsg.dlc = 8;
	sGtkTp.SN++;
	if(sGtkTp.SN>15)
	{
		sGtkTp.SN=0;
	}
	gtkCanMsg.data[0] = ISO15765_TPCI_CF|sGtkTp.SN;

	memcpy(&(gtkCanMsg.data[1]),&(sGtkTp.txBuffer[sGtkTp.txIndex]),lsize);
	sGtkTp.txIndex+=lsize;

	g_output_stream_write  (ostream,
							&gtkCanMsg,
							sizeof(GtkCanMsg_Type),
							NULL,
							&error);

	if(sGtkTp.txIndex>=sGtkTp.txLength)
	{
		sGtkTp.state = CANTP_ST_IDLE;
	}
	else
	{
		if(sGtkTp.BS > 1)
		{
			sGtkTp.BS--;
			if (1 == sGtkTp.BS)
			{
				sGtkTp.state = CANTP_ST_WAIT_FC;
			}
		}
	}
}
static void SendFC(GOutputStream * ostream)
{
	GError *error;
	GtkCanMsg_Type gtkCanMsg;
	gtkCanMsg.cmd = GTK_CAN_CMD_TX_ACK;
	gtkCanMsg.id  = sGtkTp.txId;
	gtkCanMsg.dlc = 8;
	gtkCanMsg.data[0] = ISO15765_TPCI_FC;
	gtkCanMsg.data[1] =cfgBS;
	gtkCanMsg.data[2] =cfgSTmin;

	for(int i=3;i<8;i++)
	{
		gtkCanMsg.data[i] = 0x55;
	}

	g_output_stream_write  (ostream,
							&gtkCanMsg,
							sizeof(GtkCanMsg_Type),
							NULL,
							&error);
	if(0==cfgBS)
	{
		sGtkTp.BS=0;
	}
	else
	{
		sGtkTp.BS=cfgBS+1;
	}
	sGtkTp.state = CANTP_ST_WAIT_CF;
}

static void SendStart(GOutputStream * ostream)
{
	if(sGtkTp.txLength<7)
	{
		SendSF(ostream);
	}
	else
	{
		SendFF(ostream);
	}
}
static void HandleSF(uint8* data,uint16 size)
{
	uint16 lenSF = data[0]&ISO15765_TPCI_DL;
	memcpy(sGtkTp.rxBuffer,&data[1],lenSF);
	sGtkTp.rxLength = lenSF;
	sGtkTp.state = CANTP_ST_IDLE;

	sGtkTp.SN    = 0;

	CanTp_RxIndication(sGtkTp.rxBuffer,sGtkTp.rxLength);
}
static void HandleFF(uint8* data,uint16 size)
{
	g_assert(8==size);
	sGtkTp.rxLength = ((uint16)(data[0]&0x0F)<<8) + (uint16)data[1];
	memcpy(sGtkTp.rxBuffer,&data[2],6);
	sGtkTp.rxIndex=6;
	sGtkTp.state = CANTP_ST_SEND_FC;
}
static void HandleCF(uint8* data,uint16 size)
{
	uint16 lsize = sGtkTp.rxLength-sGtkTp.rxIndex;
	if(lsize>7)
	{
		lsize=7;
	}
	sGtkTp.SN++;
	if(sGtkTp.SN>15)
	{
		sGtkTp.SN=0;
	}
	if((data[0]&0x0F) == sGtkTp.SN)
	{
		memcpy(&(sGtkTp.rxBuffer[sGtkTp.rxIndex]),&data[1],lsize);
		sGtkTp.rxIndex+=lsize;

		if(sGtkTp.rxIndex>=sGtkTp.rxLength)
		{
			CanTp_RxIndication(sGtkTp.rxBuffer,sGtkTp.rxLength);
			sGtkTp.state = CANTP_ST_IDLE;
		}
		else
		{
			if(sGtkTp.BS > 1)
			{
				sGtkTp.BS--;
			    if (1 == sGtkTp.BS)
			    {
			    	sGtkTp.state = CANTP_ST_SEND_FC;
			    }
			}
		}
	}
	else
	{	// Error
		sGtkTp.state = CANTP_ST_IDLE;
	}

}
static void HandleFC(uint8* data,uint16 size)
{
	switch (data[0]&ISO15765_TPCI_FS_MASK) //CTS
	{
		case ISO15765_FLOW_CONTROL_STATUS_CTS:
			if(data[1] != 0)
			{
				sGtkTp.BS = data[1] + 1;
			}
			else
			{
				sGtkTp.BS = 0; // receive all without FC
			}
			sGtkTp.STMin = data[2];
			sGtkTp.state = CANTP_ST_SEND_CF;
			break;
		case ISO15765_FLOW_CONTROL_STATUS_WAIT:
			sGtkTp.state = CANTP_ST_WAIT_FC;
			break;
		case ISO15765_FLOW_CONTROL_STATUS_OVFLW:
			sGtkTp.state = CANTP_ST_IDLE;
			break;
		default:
			break;
	}
}

void CanTp_Init(void)
{
	gtk_tp_init();

	g_idle_add(CanTp_MainFunction,NULL);

	isSysTimerStarted = FALSE;
	pSysTimer = g_timer_new();

	StopTimer();
}
Std_ReturnType CanTp_Transmit(uint8* data,uint16 size)
{
	Std_ReturnType ercd = E_OK;
	if(CANTP_ST_IDLE==sGtkTp.state)
	{
		g_assert(NULL != data);
		g_assert((0!=size) && (size<4096));
		memcpy(sGtkTp.txBuffer,data,size);
		sGtkTp.txLength = size;
		sGtkTp.state = CANTP_ST_START_TO_SEND;

		StartTimer();
	}
	else
	{
		return E_NOT_OK;
	}
	return ercd;
}

