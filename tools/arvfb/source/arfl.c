#include "arvfb.h"
// ======================== TYPES    =============================

enum
{
	FL_STEP_SESSION,
	FL_STEP_SECURITY_REQUEST_SEED,
	FL_STEP_SECURITY_SEND_KEY,
	FL_STEP_READ_FINGER_PRINT,
	FL_STEP_WRITE_FINGER_PRINT,
	FL_STEP_STOP
};
// ======================== DATA     =============================
static guint    sFL_Step = 0;
static gboolean isFL_Busy = FALSE;
static uint32   FL_Seed = 0;
static uint8    FL_Level = 6;

static GtkTextBuffer *pFL_TextBuffer = NULL;

// ======================== FUNCTION =============================
static void     FL_Response(uint8* data,uint16 size);
static void     FL_Session(void);
static void     FL_SecurityRequestSeed(void);
static void     FL_SecuritySendKey(void);
static void     FL_ReadFingerPrint(void);
static void     FL_WriteFingerPrint(void);

static void Trace(gchar* Log,...)
{
	va_list args;
	unsigned long length;
	static char log_buf[1024];
	va_start(args, Log);
	length = vsprintf(log_buf,Log,args);
	va_end(args);

	GtkTextIter Iter;
	gtk_text_buffer_get_end_iter(pFL_TextBuffer,&Iter);
	gtk_text_buffer_insert(pFL_TextBuffer,&Iter,log_buf,length);
}

static void Stop(void)
{
	sFL_Step = FL_STEP_STOP;
}

static void Start(void)
{
	isFL_Busy = FALSE;
	sFL_Step = FL_STEP_SESSION;
}

static void on_fl_button_clicked(GtkButton *button,gpointer data)
{
	if(0==strcmp(data,"Start"))
	{
		Start();
	}
	else if(0==strcmp(data,"Stop"))
	{
		Stop();
	}
	else
	{

	}
}

static void FL_Session(void)
{
	Std_ReturnType ercd;
	static uint8 data[2] = {0x10,0x02};
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		Trace(">>>START<<<\nFL: Change to Program Session");
		isFL_Busy = TRUE;
	}
}
static void     FL_SecurityRequestSeed(void)
{
	Std_ReturnType ercd;
	uint8 data[2] = {0x27,0xFF};

	data[1] = 2*FL_Level-1;
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		Trace("FL: Request Seed");
		isFL_Busy = TRUE;
	}
}
static void     FL_SecuritySendKey(void)
{
	Std_ReturnType ercd;
	uint8 data[6] = {0x27,0xFF};
	uint32 key = ((FL_Seed/7)<<3) - 111;
	data[1] = 2*FL_Level;
	data[2] = ((key>>24)&0xFF);
	data[3] = ((key>>16)&0xFF);
	data[4] = ((key>>8)&0xFF);
	data[5] = ((key)&0xFF);
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		Trace("FL: Send Key");
		isFL_Busy = TRUE;
	}
}
static void     FL_ReadFingerPrint(void)
{
	Std_ReturnType ercd;
	uint8 data[3] = {0x22,0xFD,0x01};
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		Trace("FL: Read Finger Print");
		isFL_Busy = TRUE;
	}
}
static void     FL_WriteFingerPrint(void)
{
	Std_ReturnType ercd;
	uint8 data[3+128] = {0x2E,0xFD,0x01};
	for(int i=0;i<128;i++)
	{
		data[3+i] = i*2;
	}
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		Trace("FL: Write Finger Print");
		isFL_Busy = TRUE;
	}
}

static void FL_Response(uint8* data,uint16 size)
{
	Std_ReturnType ercd;
	switch(sFL_Step)
	{
		case FL_STEP_SESSION:
			ercd = (data[0]==0x50)?E_OK:E_NOT_OK;
			break;
		case FL_STEP_SECURITY_REQUEST_SEED:
			ercd = (data[0]==0x67)?E_OK:E_NOT_OK;
			if(E_OK==ercd){FL_Seed=((uint32)data[2]<<24) + ((uint32)data[3]<<16) + ((uint32)data[4]<<8) + (uint32)(data[5]);};
			break;
		case FL_STEP_SECURITY_SEND_KEY:
			ercd = (data[0]==0x67)?E_OK:E_NOT_OK;
			break;
		case FL_STEP_READ_FINGER_PRINT:
			ercd = (data[0]==0x62)?E_OK:E_NOT_OK;
			break;
		case FL_STEP_WRITE_FINGER_PRINT:
			ercd = (data[0]==0x6E)?E_OK:E_NOT_OK;
			break;
		default:
			ercd = E_NOT_OK;
			break;
	}

	Trace(" %s! with response [",(E_OK==ercd)?"OK":"FAILED");
	for(int i=0;i<size;i++)
	{
		Trace("%0+2x,",data[i]);
	}
	Trace("]\n");

	if(E_OK==ercd)
	{
		isFL_Busy = FALSE;
		switch(sFL_Step)
		{
			case FL_STEP_SESSION:
				sFL_Step = FL_STEP_SECURITY_REQUEST_SEED;
				break;
			case FL_STEP_SECURITY_REQUEST_SEED:
				if(0u==FL_Seed){sFL_Step = FL_STEP_READ_FINGER_PRINT;}else{sFL_Step = FL_STEP_SECURITY_SEND_KEY;};
				break;
			case FL_STEP_SECURITY_SEND_KEY:
				sFL_Step = FL_STEP_READ_FINGER_PRINT;
				break;
			case FL_STEP_READ_FINGER_PRINT:
				sFL_Step = FL_STEP_WRITE_FINGER_PRINT;
				break;
			case FL_STEP_WRITE_FINGER_PRINT:
				sFL_Step = FL_STEP_STOP;
				break;
			default:
				break;
		}
	}
	else
	{
		isFL_Busy = FALSE;
		sFL_Step = FL_STEP_STOP;
	}
}
void ArFl_Schedule(void)
{
	if(!isFL_Busy)
	{
		switch(sFL_Step)
		{
			case FL_STEP_SESSION:
				FL_Session();
				break;
			case FL_STEP_SECURITY_REQUEST_SEED:
				FL_SecurityRequestSeed();
				break;
			case FL_STEP_SECURITY_SEND_KEY:
				FL_SecuritySendKey();
				break;
			case FL_STEP_READ_FINGER_PRINT:
				FL_ReadFingerPrint();
				break;
			case FL_STEP_WRITE_FINGER_PRINT:
				FL_WriteFingerPrint();
				break;
			default:
				break;
		}
	}
}

void ArFl_Init(void)
{
	Stop();
}


// CALLBACK of CanTP
void CanTp_RxIndication(uint8* data,uint16 length)
{
	FL_Response(data,length);
}

GtkWidget* ArFlashLoader(void)
{
	GtkWidget* pBox;
	GtkWidget* pButton;

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	pButton = gtk_button_new_with_label("Start");
	gtk_box_pack_start(GTK_BOX(pBox),pButton,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT (pButton), "clicked", G_CALLBACK(on_fl_button_clicked) , "Start");

	pButton = gtk_button_new_with_label("Stop");
	gtk_box_pack_start(GTK_BOX(pBox),pButton,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT (pButton), "clicked", G_CALLBACK(on_fl_button_clicked) , "Stop");

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
		pFL_TextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
		gtk_text_buffer_get_end_iter(pFL_TextBuffer,&Iter);
		gtk_text_buffer_insert(pFL_TextBuffer,&Iter,"FL Log:\n",8);
	}
	return pBox;
}
