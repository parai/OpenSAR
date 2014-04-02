
#include "arvfb.h"
// ========================== [ LOCAL MACROS    ] ===============================


// ========================== [ LOCAL TYPES    ] ===============================

// ========================== [ LOCAL VARIANTS  ] ===============================

// ========================== [ LOCAL FUNCTIONS ] ===============================


// poll if there is a message on port needed to be transmitted or processed.
// port: see localhost:port, socket
// pMsg: both input ArMsg and output ArMsg
boolean ArvfbPoll(ArPortType port,ArMsgType* pMsg)
{
	boolean rv= FALSE;
	GError * error = NULL;

	uint32 command = pMsg->Command;

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
						  pMsg,
						  pMsg->Length+MSG_MIN_LENGTH,
						  NULL,
						  &error);
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	gssize size = g_input_stream_read(istream,pMsg,sizeof(ArMsgType),NULL,&error);
	if( (size > MSG_MIN_LENGTH) &&
		((MSG_MIN_LENGTH+pMsg->Length)==size) &&
		((command|MSG_CMD_ACK) == pMsg->Command) )
	{
		rv = TRUE;
	}
	else
	{   // <= MSG_MIN_LENGTH, default nothing has been gotten on the port
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
void ArvfbSend(ArPortType port,ArMsgType* pMsg)
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
	/* use the connection */
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	g_output_stream_write  (ostream,
						  pMsg,
						  pMsg->Length+MSG_MIN_LENGTH,
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
