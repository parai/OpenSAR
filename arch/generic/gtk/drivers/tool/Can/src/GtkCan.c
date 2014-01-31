#include "GtkCan.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define CAN_LOG_FILE "GtkCan.log"

#define cfgWithGui  0

static GTimer*    pSysTimer;
static guint               PortList[512];
static guint               PortLen;
static gboolean   is1stReceived = FALSE;
#if cfgWithGui == 1
GtkTextBuffer *pTextBuffer = NULL;
#endif

static const gchar gtkCmdTx = GTK_CAN_CMD_TX;
static void gtk_can_log_init(void)
{
	FILE* fp = NULL;
	fp = fopen(CAN_LOG_FILE,"w");
	fclose(fp);
}
static void gtk_can_log(guint port,GtkCanMsg_Type* pRxMsg)
{
	static gchar log_buffer[512];
	int len = sprintf(log_buffer,"CANID=0x%-3x,DLC=%x, [",pRxMsg->id,(guint)pRxMsg->dlc);
	for(int i=0;i<8;i++)
	{
		len += sprintf(&log_buffer[len],"%0+2x,",(guint)pRxMsg->data[i]);
	}
	gdouble elapsed = g_timer_elapsed(pSysTimer,NULL);
	if(FALSE == is1stReceived)
	{
		is1stReceived = TRUE;
		g_timer_start(pSysTimer);
		elapsed = 0;
	}
	len += sprintf(&log_buffer[len],"] %8.4fs from %-5d\n",elapsed,port);
	g_print(log_buffer);

#if cfgWithGui == 1
	GtkTextIter Iter;
	gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
	gtk_text_buffer_insert(pTextBuffer,&Iter,log_buffer,len);
#endif

	FILE* fp = NULL;
	fp = fopen(CAN_LOG_FILE,"a");
	fwrite(log_buffer,len,1,fp);
	fclose(fp);

}
// poll if there is a can message on port needed to be transimited.
static gboolean gtk_can_poll_tx(guint port,GtkCanMsg_Type* pRxMsg)
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
						  &gtkCmdTx, /* your message goes here */
						  1, /* length of your message */
						  NULL,
						  &error);
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	GtkCanMsg_Type gtkCanMsg;
	gssize size = g_input_stream_read(istream,&gtkCanMsg,sizeof(GtkCanMsg_Type),NULL,&error);
	if(size == sizeof(GtkCanMsg_Type))
	{
		gtk_can_log(port,&gtkCanMsg);
		if(NULL != pRxMsg)
		{
			memcpy(pRxMsg,&gtkCanMsg,sizeof(GtkCanMsg_Type));
		}
		rv = TRUE;
	}
	else
	{
//		if(0 != size)
//		{
//			g_print("size=%d\n",size);
//		}
		rv = FALSE;// No
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
static void gtk_can_poll_rx(guint port,GtkCanMsg_Type* pTxMsg)
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

	pTxMsg->cmd = GTK_CAN_CMD_RX;
	/* use the connection */
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	g_output_stream_write  (ostream,
						  pTxMsg, /* your message goes here */
						  sizeof(GtkCanMsg_Type), /* length of your message */
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
	GtkCanMsg_Type      PortRxMsg;
	for(int i=0;i<PortLen;i++)
	{
		if(gtk_can_poll_tx(PortList[i],&PortRxMsg))
		{
			for(int j=0;j<PortLen;j++)
			{
				if(i!=j)
				{
					gtk_can_poll_rx(PortList[j],&PortRxMsg);
				}
			}
		}
	}
	return TRUE;
}
static void Init(int argc, char *argv[])
{
	PortLen = argc -1;
	if(PortLen != 0)
	{
		g_print("GtkCan: Listen on [");
		for(int i=0;i<PortLen;i++)
		{
			PortList[i] = atoi(argv[i+1]);
			g_print("%-4d,",PortList[i]);
		}
		g_print("]\n");
	}
	else
	{
		PortList[0] = 8000;
		PortLen = 1;
	}
	gtk_can_log_init();

	/* initialize glib */
	g_type_init ();
	pSysTimer = g_timer_new();

	//g_idle_add(Idle,NULL);
	g_timeout_add(5,Idle,NULL);
}

#if cfgWithGui == 1
static GtkWidget* CreateLog(void)
{
	GtkWidget* pBox;

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

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
		gtk_text_buffer_insert(pTextBuffer,&Iter,"CAN LOG:\n",8);
	}
	return pBox;
}
#endif
int main (int argc, char *argv[])
{
	GtkWidget *pWindow;
	GtkWidget* pBox;

	Init(argc,argv);

#if cfgWithGui == 1
	gtk_init (&argc, &argv);

	pWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);


	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous(GTK_BOX(pBox),FALSE);

	gtk_container_add(GTK_CONTAINER (pWindow), pBox);

	// Initialize
	gtk_window_set_title(pWindow,(const gchar*)"GtkCan (CAN TOOL SERVER)\n");
	gtk_container_set_border_width (GTK_CONTAINER (pWindow), 0);

	gtk_box_pack_start(GTK_BOX(pBox),CreateLog(),FALSE,FALSE,0);

	gtk_widget_show_all (pWindow);

	gtk_main ();
#else
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
#endif

	return 0;
}
