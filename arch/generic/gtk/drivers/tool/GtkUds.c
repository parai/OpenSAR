#include "GtkCan.h"

static gboolean gtk_uds_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data)
{
	GError * error = NULL;
	GtkCanMsg_Type gtkCanMsg;
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	gssize size = g_input_stream_read(istream,&gtkCanMsg,sizeof(GtkCanMsg_Type),NULL,&error);
	if(size == GTK_CAN_MSG_BOX_RX_SIZE)
	{
		g_print("CANID=0x%-3x,DLC=%x, [",gtkCanMsg.id,(guint)gtkCanMsg.dlc);
		for(int i=0;i<8;i++)
		{
			g_print("%-2x,",(guint)gtkCanMsg.data[i]);
		}
		g_print("]\n");
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
int main (int argc, char *argv[])
{
	g_type_init ();
	gtk_uds_server_init();


	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	return 0;

}
