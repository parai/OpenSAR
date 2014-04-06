
#include "arvfb.h"
// ========================== [ LOCAL MACROS    ] ===============================
#define MAX_SUPPORT_NODE  512

// ========================== [ LOCAL TYPES    ] ===============================

// ========================== [ LOCAL VARIANTS  ] ===============================
static GtkTextBuffer *pTextBuffer = NULL;
static GtkWidget     *pEntery     = NULL;
static ArPortType     Ports[MAX_SUPPORT_NODE] = {8000,};
static uint16         PortNumber  = 1;

static GTimer*        pTimer;

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
	//gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
	gtk_text_buffer_get_start_iter(pTextBuffer,&Iter);
	gtk_text_buffer_insert(pTextBuffer,&Iter,log_buf,length);
}

static char* FormatTime(gdouble time)
{
	int hour,minute,second;

	static char strTime[512];
	hour = (int)(time/3600);
	time = time - 3600*hour;
	minute = (int)(time/60);
	time = time - 60*minute;
	second = (int)time;
	time = time - second;
	sprintf(strTime,"%4d H:%2d M:%2d S:%7.3f MS",hour,minute,second,time*1000);
	return strTime;
}
static void TraceLog(uint16 port,ArCanMsgType* pMsg)
{
	static boolean is1stReceived=FALSE;
	static gchar log_buffer[512];
	int len = sprintf(log_buffer,"CANID=0x%-3x,DLC=%x, [",pMsg->Msg.Identifier,(guint)pMsg->Msg.DataLengthCode);
	for(int i=0;i<8;i++)
	{
		len += sprintf(&log_buffer[len],"%-2x,",(guint)pMsg->Msg.Data[i]);
	}
	gdouble elapsed = g_timer_elapsed(pTimer,NULL);
	if(FALSE == is1stReceived)
	{
		is1stReceived = TRUE;
		g_timer_start(pTimer);
		elapsed = 0;
	}
	len += sprintf(&log_buffer[len],"] %s from %-5d on bus %d\n",FormatTime(elapsed),port,pMsg->Msg.BusID);

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

// poll if there is a can message on port needed to be transimited.
static boolean Poll(ArPortType port,ArMsgType* pMsg)
{
	boolean rv;
	pMsg->Type    = MSG_ON_CAN;
	pMsg->Command = MSG_CMD_POLL;
	pMsg->Length  = 0;

	rv = ArvfbPoll(port,pMsg);

	return rv;
}

// forward the message to the port
static void Forward(ArPortType port,const ArMsgType* pMsg)
{
	ArMsgType armsg;
	memcpy(&armsg,pMsg,sizeof(ArMsgType));
	armsg.Command = MSG_CMD_SEND;
	ArvfbSend(port,&armsg);
}

// ======================== [    FUNCTIONS    ] ==============================================
void ArCan_Schedule(void)
{
	ArMsgType      Message;
	for(int i=0;i<PortNumber;i++)
	{
		if(Poll(Ports[i],&Message))
		{
			TraceLog(Ports[i],(ArCanMsgType*)&Message);
			for(int j=0;j<PortNumber;j++)
			{
				if(i!=j)
				{
					Forward(Ports[j],&Message);
				}
			}
			// Tester may do something
			Ardl_RxIndication(&Message);
		}
	}
}

void Can_Transmit(const ArMsgType *armsg)
{
	for(int i=0;i<PortNumber;i++)
	{
		Forward(Ports[i],armsg);
	}

	TraceLog(2013,(ArCanMsgType*)armsg);
}

void ArCan_Init(void)
{
	pTimer = g_timer_new();
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
		gtk_widget_set_size_request(swindow,800,500);
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

	return pBox;
}
