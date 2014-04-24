/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
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
void ArvfbSend(ArPortType port,const ArMsgType* pMsg)
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
