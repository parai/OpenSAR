#include "GtkCan.h"
#include "Std_Types.h"
// ======================== TYPE     =============================
typedef enum
{
	CANTP_ST_IDLE = 0,
	CANTP_ST_RECEIVING,
	CANTP_ST_BUSY,
	CANTP_ST_START_TO_SEND,
	CANTP_ST_SENDING,
	CANTP_ST_SEND_FINISHED,
	CANTP_ST_WAIT_FC,
	CANTP_ST_SEND_CF
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

// ======================== DATA     =============================
static GtkTp_Type sGtkTp;
static GTimer* pSysTimer;
static gboolean isSysTimerStarted;

static guint    sFL_Step = 0;
static gboolean isFL_Busy = FALSE;

// ======================== FUNCTION =============================
static void     gtk_uds_tx(GSocketConnection *connection);
static void     gtk_uds_rx(GtkCanMsg_Type* pMsg,gssize size);
static gboolean gtk_uds_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data);
static void     gtk_uds_server_init(void);
static void     gtk_uds_init(void);
static gboolean CanTp_MainFunction(gpointer data);


static void     FL_Response(uint8* data,uint16 size);
static void     FL_Session(void);
static gboolean FlashLoader(gpointer data);


static void     StartTimer(void);
static void     StopTimer(void);
static gboolean IsTimerElapsed(gulong microseconds);

static void SendSF(GOutputStream * ostream);
static void SendFF(GOutputStream * ostream);
static void SendStart(GOutputStream * ostream);
static void HandleSF(uint8* data,uint16 size);
static void HandleFF(uint8* data,uint16 size);
static void HandleBusy(GtkCanMsg_Type* pMsg);
static Std_ReturnType CanTp_Transmit(uint8* data,uint16 size);
static void CanTp_RxIndication(uint8* data,uint16 length);


// ======================== FUNCTION =============================
static void gtk_uds_tx(GSocketConnection *connection)
{
	GError* error;
	GtkCanMsg_Type gtkCanMsg;
	static const gchar dumy_ack = GTK_CAN_CMD_TX_ACK;
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	switch(sGtkTp.state)
	{
		case CANTP_ST_START_TO_SEND:
			SendStart(ostream);
			break;
		default:
			g_output_stream_write  (ostream,
									&dumy_ack, /* your message goes here */
									1, /* length of your message */
									NULL,
									&error);
			break;
	}
}

static void gtk_uds_rx(GtkCanMsg_Type* pMsg,gssize size)
{
	if((sGtkTp.rxId == pMsg->id) && (sizeof(GtkCanMsg_Type)==size))
	{
		switch(sGtkTp.state)
		{
			case CANTP_ST_BUSY:
				HandleBusy(pMsg);
				break;
			default:
				break;
		}
	}
}
static gboolean gtk_uds_incoming_callback  (GSocketService *service,
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
			gtk_uds_tx(connection);
			break;
		case GTK_CAN_CMD_RX:
			gtk_uds_rx(&gtkCanMsg,size);
			break;
		default:
			g_print("CAN:Error Command!\n");
			break;
	}
	return FALSE;
}

static void gtk_uds_server_init(void)
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
				G_CALLBACK (gtk_uds_incoming_callback),
				NULL);

	/* start the socket service */
	g_socket_service_start (pGSocketService);
}

static void gtk_uds_init(void)
{
	memset(&sGtkTp,0,sizeof(sGtkTp));

	sGtkTp.rxId = 0x732;
	sGtkTp.txId = 0x731;

	gtk_uds_server_init();
}

static gboolean CanTp_MainFunction(gpointer data)
{
	switch(sGtkTp.state)
	{
		case CANTP_ST_IDLE:
			break;
		case CANTP_ST_RECEIVING:
			break;
		case CANTP_ST_BUSY:
			break;
		case CANTP_ST_START_TO_SEND:
			break;
		case CANTP_ST_SENDING:
			break;
		case CANTP_ST_SEND_FINISHED:
			break;
		case CANTP_ST_WAIT_FC:
			break;
		case CANTP_ST_SEND_CF:
			break;
		default:
			break;
	}
	return TRUE;
}

static void FL_Session(void)
{
	Std_ReturnType ercd;
	static const uint8 data[2] = {0x10,0x02};
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		g_print("FL: Change to Program Session");
		isFL_Busy = TRUE;
	}
}
// CALLBACK of CanTP
static void CanTp_RxIndication(uint8* data,uint16 length)
{
	FL_Response(data,length);
}
static void FL_Response(uint8* data,uint16 size)
{
	if(0==sFL_Step)
	{
		if(data[0]==0x50)
		{
			g_print(" OK!\n");
		}
		else
		{
			g_print(" FAILED!\n");
		}
	}
	isFL_Busy = FALSE;
	sFL_Step ++;
}
static gboolean FlashLoader(gpointer data)
{
	if(!isFL_Busy)
	{
		if(0 == sFL_Step)
		{
			FL_Session();
		}
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

		elapsed = g_timer_elapsed(pSysTimer,NULL); // unit in
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
	int i;
	gtkCanMsg.cmd = GTK_CAN_CMD_TX_ACK;
	gtkCanMsg.id  = sGtkTp.txId;
	gtkCanMsg.data[0] = sGtkTp.txLength|ISO15765_TPCI_SF;  // SF
	for(i=0;i<sGtkTp.txLength;i++)
	{
		gtkCanMsg.data[1+i] = sGtkTp.txBuffer[i];
	}
	for(;i<8;i++)
	{
		gtkCanMsg.data[1+i] = 0x55;
	}
	g_output_stream_write  (ostream,
							&gtkCanMsg,
							sizeof(GtkCanMsg_Type),
							NULL,
							&error);
	sGtkTp.state = CANTP_ST_BUSY;
	g_print("%s()\n",__FUNCTION__);
}
static void SendFF(GOutputStream * ostream)
{

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

	CanTp_RxIndication(sGtkTp.rxBuffer,sGtkTp.rxLength);
}
static void HandleFF(uint8* data,uint16 size)
{

}
static void HandleBusy(GtkCanMsg_Type* pMsg)
{
	if(ISO15765_TPCI_SF==(pMsg->data[0]&ISO15765_TPCI_MASK))
	{
		HandleSF(pMsg->data,pMsg->dlc);
	}
}
static Std_ReturnType CanTp_Transmit(uint8* data,uint16 size)
{
	Std_ReturnType ercd = E_OK;
	if(CANTP_ST_IDLE==sGtkTp.state)
	{
		g_assert(NULL != data);
		g_assert(0!=size);
		memcpy(sGtkTp.txBuffer,data,size);
		sGtkTp.txLength = size;
		sGtkTp.state = CANTP_ST_START_TO_SEND;
	}
	else
	{
		return E_NOT_OK;
	}
	return ercd;
}
int main (int argc, char *argv[])
{
	g_type_init ();

	gtk_uds_init();

	g_idle_add(CanTp_MainFunction,NULL);
	g_idle_add(FlashLoader,NULL);

	isSysTimerStarted = FALSE;
	pSysTimer = g_timer_new();

	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	return 0;

}
