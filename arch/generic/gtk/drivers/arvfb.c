#include "arvfb.h"

// =============================== [ ARVFB DATAs     ] ==============================

static GSocketService * pGSocketService;


// =============================== [ ARVFB FUNCTIONS ] ==============================
static gboolean arvfb_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data)
{
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	ArMsgType ArMsgIn,ArMsgOut;
	boolean Answer = FALSE;
	gssize size = g_input_stream_read  (istream,
						&ArMsgIn,
						sizeof(ArMsgType),
						NULL,
						NULL);
	if( (size >= MSG_MIN_LENGTH) &&
	    ((ArMsgIn.Length+MSG_MIN_LENGTH) == size) )
	{
		switch(ArMsgIn.Type)
		{
			case MSG_ON_CAN:
				Answer = ArvfbCanProcess((const ArCanMsgType*)&ArMsgIn,(ArCanMsgType*)&ArMsgOut);
				break;
			case MSG_ON_LIN:
				break;
			default:
				break;
		}
		if(Answer)
		{
			GError* error;
			GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
			g_output_stream_write  (ostream,
									&ArMsgOut,
									ArMsgOut.Length+MSG_MIN_LENGTH,
									NULL,
									&error);
		}
	}
	return FALSE;
}

void ArvfbInit(void)
{
	GError * error = NULL;
	gboolean isOK;
	ArPortType port = 8000;

	/* create the new socketservice */
	pGSocketService = g_socket_service_new ();

	do{
		/* connect to the port */
		isOK = g_socket_listener_add_inet_port ((GSocketListener*)pGSocketService,
									port,
									NULL,
									&error);
	}while((isOK!=TRUE) && (port<9000));

	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("VFB: Init Error <%s>\n!",error->message);
		//g_error (error->message);
	}
	else
	{
		g_print("VFB: Listener on localhost:%d\n",port);
	}

	/* listen to the 'incoming' signal */
	g_signal_connect (pGSocketService,
				"incoming",
				G_CALLBACK (arvfb_incoming_callback),
				NULL);

	/* start the socket service */
	g_socket_service_start (pGSocketService);
}
