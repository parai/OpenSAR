#include <glib.h>
#include <gio/gio.h>

int main (int argc, char *argv[])
{
	/* initialize glib */
	g_thread_init(NULL);
	g_type_init ();
	GTimer* pSysTimer = g_timer_new();
	while (TRUE)
	{
		GError * error = NULL;

		/* create a new connection */
		GSocketConnection * connection = NULL;
		GSocketClient * client = g_socket_client_new();

		/* connect to the host */
		connection = g_socket_client_connect_to_host (client,
											   (gchar*)"localhost",
												8000, /* your port goes here */
												NULL,
												&error);

		/* don't forget to check for errors */
		if (error != NULL)
		{
			g_print("SERVER: <%s>!\n",error->message);
			g_error (error->message);
			g_object_unref(client);
			continue;
		}

		/* use the connection */
		GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
		g_output_stream_write  (ostream,
							  "Hello server!", /* your message goes here */
							  13, /* length of your message */
							  NULL,
							  &error);
		GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
		guchar data[32];
		gssize size = g_input_stream_read(istream,data,32,NULL,&error);
		if(size == 14)
		{
			guint Id = (((guint)data[0])<<24) + (((guint)data[1])<<16) + (((guint)data[2])<<8) + data[3];
			g_print("CANID=0x%-3x,DLC=%x, [",Id,(guint)data[4]);
			for(int i=0;i<8;i++)
			{
				g_print("%-2x,",(guint)data[5+i]);
			}
			gdouble elapsed = g_timer_elapsed(pSysTimer,NULL); // unit in
			g_timer_start(pSysTimer);
			g_print("] %8.4fus\n",elapsed);
		}
		/* don't forget to check for errors */
		if (error != NULL)
		{
			g_print("SERVER: <%s>!\n",error->message);
			g_error (error->message);
		}
		g_object_unref(connection);
		g_object_unref(client);
	}
  return 0;
}
