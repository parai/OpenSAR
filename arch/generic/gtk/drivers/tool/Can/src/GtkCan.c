#include "GtkCan.h"
#include <stdio.h>
#include <stdlib.h>

#define CAN_LOG_FILE "GtkCan.log"

static GTimer*    pSysTimer;
static gboolean   is1stReceived = FALSE;

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

int main (int argc, char *argv[])
{
	guint               PortList[512];
	guint               PortLen;
	GtkCanMsg_Type      PortRxMsg;

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
	while (TRUE)
	{
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
	}
  return 0;
}
