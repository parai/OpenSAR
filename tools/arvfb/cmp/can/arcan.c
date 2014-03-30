
#include "arvfb.h"
// ========================== [ LOCAL MACROS    ] ===============================
#define MAX_SUPPORT_NODE  512
// Commands
#define CMD_POLL      ((guchar)0x00)
#define CMD_FORWARD   ((guchar)0x01)

#define CMD_POLL_ACK    ((guchar)0x80)
#define CMD_FORWARD_ACK ((guchar)0x81)


// ========================== [ LOCAL TYPES    ] ===============================
typedef struct
{
	uint8      cmd;
	uint8      dlc;
	uint32 	   id;
	uint8      data[8];
	uint8      bus;      // Identifier of this CAN NODE on the bus
	uint16     reserved;
}ArCanMessage_Type;

// ========================== [ LOCAL VARIANTS  ] ===============================
static GtkTextBuffer *pTextBuffer = NULL;
static GtkWidget     *pEntery     = NULL;
static uint16         NodeIdentifiers[MAX_SUPPORT_NODE] = {8000,};
static uint16         NodeNumber  = 1;

static GTimer*        pTimer;


static const guchar   cmdPoll = CMD_POLL;

// ========================== [ LOCAL FUNCTIONS ] ===============================
static void Trace(const gchar* format,...)
{
	va_list args;
	unsigned long length;
	static char log_buf[1024];
	va_start(args, format);
	length = vsprintf(log_buf,format,args);
	va_end(args);

	GtkTextIter Iter;
	gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
	gtk_text_buffer_insert(pTextBuffer,&Iter,log_buf,length);
}

static void TraceLog(uint16 port,ArCanMessage_Type* pMsg)
{
	static boolean is1stReceived=FALSE;
	static gchar log_buffer[512];
	int len = sprintf(log_buffer,"CANID=0x%-3x,DLC=%x, [",pMsg->id,(guint)pMsg->dlc);
	for(int i=0;i<8;i++)
	{
		len += sprintf(&log_buffer[len],"%-2x,",(guint)pMsg->data[i]);
	}
	gdouble elapsed = g_timer_elapsed(pTimer,NULL);
	if(FALSE == is1stReceived)
	{
		is1stReceived = TRUE;
		g_timer_start(pTimer);
		elapsed = 0;
	}
	len += sprintf(&log_buffer[len],"] %8.4fs from %-5d\n",elapsed,port);

	Trace(log_buffer);
}

static void
text_changed_cb (GtkEntry   *entry,
                 GParamSpec *pspec,
                 GtkWidget  *button)
{
  gboolean has_text;

  has_text = gtk_entry_get_text_length (entry) > 0;

  if(has_text)
  {
	  Trace( gtk_entry_get_text(entry) );
  }
}

static void Init(void)
{
	pTimer = g_timer_new();
}
// poll if there is a can message on port needed to be transimited.
static gboolean Poll(uint16 port,ArCanMessage_Type* pMsg)
{
	gboolean rv= FALSE;
	GError * error = NULL;

	/* create a new connection */
	GSocketConnection * connection = NULL;
	GSocketClient * client = g_socket_client_new();

	/* connect to the host */
	connection = g_socket_client_connect_to_host (client,
										   (gchar*)"localhost",
										    port, /* your port goes here */
											NULL,
											&error);

	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("SERVER: <%s>!\n",error->message);
		g_object_unref(client);
		return FALSE;
	}

	/* use the connection */
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	g_output_stream_write  (ostream,
						  &cmdPoll, /* your message goes here */
						  1, /* length of your message */
						  NULL,
						  &error);
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	ArCanMessage_Type Msg;
	gssize size = g_input_stream_read(istream,&Msg,sizeof(ArCanMessage_Type),NULL,&error);
	if(size == sizeof(ArCanMessage_Type))
	{
		g_assert(pMsg != NULL);
		TraceLog(port,&Msg);
		memcpy(pMsg,&Msg,sizeof(ArCanMessage_Type));
		rv = TRUE;
	}
	else
	{
		rv = FALSE;
	}
	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("SERVER: <%s>!\n",error->message);
	}
	g_object_unref(connection);
	g_object_unref(client);
	return rv;
}

// forward the message to the port
static void Forward(uint16 port,ArCanMessage_Type* pMsg)
{
	GError * error = NULL;

	/* create a new connection */
	GSocketConnection * connection = NULL;
	GSocketClient * client = g_socket_client_new();

	/* connect to the host */
	connection = g_socket_client_connect_to_host (client,
										   (gchar*)"localhost",
										    port, /* your port goes here */
											NULL,
											&error);

	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("SERVER: <%s>!\n",error->message);
		g_object_unref(client);
		return;
	}

	pMsg->cmd = CMD_FORWARD;
	/* use the connection */
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	g_output_stream_write  (ostream,
						  pMsg, /* your message goes here */
						  sizeof(ArCanMessage_Type), /* length of your message */
						  NULL,
						  &error);
	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("SERVER: <%s>!\n",error->message);
	}
	g_object_unref(connection);
	g_object_unref(client);
}

static gboolean Idle(gpointer data)
{
	ArCanMessage_Type      Message;
	for(int i=0;i<NodeNumber;i++)
	{
		if(Poll(NodeIdentifiers[i],&Message))
		{
			for(int j=0;j<NodeNumber;j++)
			{
				if(i!=j)
				{
					Forward(NodeIdentifiers[j],&Message);
				}
			}
		}
	}
	return TRUE;
}
GtkWidget* ArCan(void)
{
	GtkWidget* pBox;

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	// Bus Slection
	{
		GtkWidget* pBox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
		gtk_box_pack_start (GTK_BOX (pBox2), gtk_label_new("Bus:"), TRUE, TRUE, 0);
		pEntery = gtk_entry_new();
		g_signal_connect (pEntery, "notify::text",
		                  G_CALLBACK (text_changed_cb), NULL);
		gtk_box_pack_start (GTK_BOX (pBox2), pEntery, TRUE, TRUE, 0);

		gtk_box_pack_start (GTK_BOX (pBox), pBox2, TRUE, TRUE, 0);
	}

	// Trace
	{
		GtkWidget *swindow;
		GtkWidget *textview;
		GtkTextIter Iter;
		swindow = gtk_scrolled_window_new (NULL, NULL);
		gtk_widget_set_size_request(swindow,800,600);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swindow),
									  GTK_POLICY_AUTOMATIC,
									  GTK_POLICY_AUTOMATIC);
		gtk_box_pack_start (GTK_BOX (pBox), swindow, TRUE, TRUE, 0);
		textview = gtk_text_view_new ();
		//gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
		gtk_text_view_set_editable(GTK_TEXT_VIEW (textview),FALSE);
		gtk_container_add (GTK_CONTAINER (swindow), textview);
		pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
		gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
	}

	// TaskInit
	Init();
	g_timeout_add(1,Idle,NULL);

	return pBox;
}
