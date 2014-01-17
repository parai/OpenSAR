// ========================== Example Server CODE ======================
#if 0
#include <glib.h>
#include <gio/gio.h>

/* this function will get called everytime a client attempts to connect */
gboolean
incoming_callback  (GSocketService *service,
                    GSocketConnection *connection,
                    GObject *source_object,
                    gpointer user_data)
{
  g_print("Received Connection from client!\n");
  GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
  gchar message[1024];
  g_input_stream_read  (istream,
                        message,
                        1024,
                        NULL,
                        NULL);
  g_print("Message was: \"%s\"\n", message);
  return FALSE;
}

int
main (int argc, char **argv)
{
  /* initialize glib */
  g_type_init();

  GError * error = NULL;

  /* create the new socketservice */
  GSocketService * service = g_socket_service_new ();

  /* connect to the port */
  g_socket_listener_add_inet_port ((GSocketListener*)service,
                                    1500, /* your port goes here */
                                    NULL,
                                    &error);

  /* don't forget to check for errors */
  if (error != NULL)
  {
      g_error (error->message);
  }

  /* listen to the 'incoming' signal */
  g_signal_connect (service,
                    "incoming",
                    G_CALLBACK (incoming_callback),
                    NULL);

  /* start the socket service */
  g_socket_service_start (service);

  /* enter mainloop */
  g_print ("Waiting for client!\n");
  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
#endif
// ========================== Example Server CODE ======================

// ========================== Example Client CODE ======================
#if 0
#include <glib.h>
#include <gio/gio.h>

int
main (int argc, char *argv[])
{
  /* initialize glib */
  g_type_init ();

  GError * error = NULL;

  /* create a new connection */
  GSocketConnection * connection = NULL;
  GSocketClient * client = g_socket_client_new();

  /* connect to the host */
  connection = g_socket_client_connect_to_host (client,
                                               (gchar*)"localhost",
                                                1500, /* your port goes here */
                                                NULL,
                                                &error);

  /* don't forget to check for errors */
  if (error != NULL)
  {
      g_error (error->message);
  }
  else
  {
      g_print ("Connection successful!\n");
  }

  /* use the connection */
  GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
  GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
  g_output_stream_write  (ostream,
                          "Hello server!", /* your message goes here */
                          13, /* length of your message */
                          NULL,
                          &error);
  /* don't forget to check for errors */
  if (error != NULL)
  {
      g_error (error->message);
  }
  return 0;
}
#endif
// ========================== Example Client CODE ======================
#include <assert.h>
#include <stdio.h>
#include "Can.h"
#include "CanSocket.h"


CAN_HW_t Can_HwUnit[CAN_ARC_CTRL_CONFIG_CNT];
extern void arch_generate_irqn(IrqType IRQn);
static void CanSocket_Tx(uint8 ctrl,GSocketConnection *connection);
static void CanSocket_Rx(uint8 ctrl,uint8* data,uint8 size);

static gboolean CanSocket_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data)
{
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	gchar message[32];
	gssize size = g_input_stream_read  (istream,
						message,
						32,
						NULL,
						NULL);
	//g_print("Message was: \"%s\"\n", message);
	CanSocket_Tx((uint8)(user_data),connection);

	return FALSE;
}
// Exported Functions
void Can_0_IsrEntry(void)
{
	CAN_HW_t *canHw = &Can_HwUnit[CAN_CTRL_0];
	Can_SocketEnterCritical(CAN_CTRL_0);
	if(cCanIsrTx == (canHw->IRQF&cCanIsrTx))
	{
		Can_0_TxIsr();
	}
	if(cCanIsrRx == (canHw->IRQF&cCanIsrRx))
	{
		Can_0_RxIsr();
	}
	if(cCanIsrWak == (canHw->IRQF&cCanIsrWak))
	{
		Can_0_WakeIsr();
	}
	if(cCanIsrErr == (canHw->IRQF&cCanIsrErr))
	{
		Can_0_ErrIsr();
	}
	canHw->IRQF = 0; // clear all
	Can_SocketExitCritical(CAN_CTRL_0);
}

static void CanSocket_Tx(uint8 ctrl,GSocketConnection *connection)
{
	uint8_t msg[32];
	CAN_HW_t *canHw;
	canHw = &Can_HwUnit[ctrl];
	if(BM_TX0==(canHw->TIER & BM_TX0))
	{
		int i;
		GError* error;
		GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
		msg[0] = (uint8)(canHw->txMsg[0].id>>24);
		msg[1] = (uint8)(canHw->txMsg[0].id>>16);
		msg[2] = (uint8)(canHw->txMsg[0].id>>8);
		msg[3] = (uint8)(canHw->txMsg[0].id);
		msg[4] = canHw->txMsg[0].dlc;
		if(msg[4] > 8)
		{
			msg[4] = 8;
		}
		for(i=0;i<msg[4];i++)
		{
			msg[5+i] = canHw->txMsg[0].data[i];
		}
		for(;i<8;i++)
		{
			msg[5+i] = 0x55;  // PADDING
		}
		msg[13] = ctrl;
		g_output_stream_write  (ostream,
								msg, /* your message goes here */
								14, /* length of your message */
								NULL,
								&error);
		// Clear TIER
		canHw->TIER &= ~(BM_TX0);
		canHw->IRQF |= cCanIsrTx;
		// Request System Isr

		arch_generate_irqn(SysCan_0_IRQn+ctrl);
	}
}

static void CanSocket_Rx(uint8 ctrl,uint8* data,uint8 size)
{
	CAN_HW_t *canHw;
	canHw = &Can_HwUnit[ctrl];
	if ( size == 14 )
	{ // Rx ISR
		uint32 id = (((uint32)data[0])<<24) + (((uint32)data[1])<<16) + (((uint32)data[2])<<8) + data[3];
		if( BM_WUPI == (canHw->RIER & BM_WUPI))
		{
			canHw->IRQF |= cCanIsrWak;
			printf("Can Wakeup!\n");
		}
		else
		{
			int I;
			for(I=0;I<cCanMsgBoxSz;I++)
			{
				if(eCanMsgBoxIdle == canHw->rxMsg[I].state)
				{
					break;
				}
			}
			if(I < cCanMsgBoxSz)
			{
				canHw->rxMsg[I].id = id;
				canHw->rxMsg[I].dlc = data[4];
				memcpy(canHw->rxMsg[I].data,&data[5],data[4]);
				canHw->rxMsg[I].state = eCanMsgBoxRxed;
			}
			else
			{
				printf("Can Message Box Full, Message Lost!\n");
			}
			canHw->IRQF |= cCanIsrRx;
		}
		arch_generate_irqn(SysCan_0_IRQn+ctrl);
	}
}

void Can_1_IsrEntry(void)
{

}

void Can_2_IsrEntry(void)
{

}

void Can_3_IsrEntry(void)
{

}

void Can_4_IsrEntry(void)
{

}

void Can_SocketDeInit(uint8_t ctrl)
{
	CAN_HW_t *canHw = &Can_HwUnit[ctrl];
	g_socket_service_stop (canHw->pGSocketService);
	g_object_unref(canHw->pGSocketService);
}

void Can_SocketInit(uint8_t ctrl)
{
	CAN_HW_t *canHw = &Can_HwUnit[ctrl];
	GError * error = NULL;

	if(CAN_CTRL_0 != ctrl)
	{	// Just simulate CAN0 for test something
		return;
	}
	/* create the new socketservice */
	canHw->pGSocketService = g_socket_service_new ();

	/* connect to the port */
	g_socket_listener_add_inet_port ((GSocketListener*)canHw->pGSocketService,
								8000+ctrl, /* your port goes here */
								NULL,
								&error);

	/* don't forget to check for errors */
	if (error != NULL)
	{
		g_print("CAN: Init Error <%s>\n!",error->message);
		//g_error (error->message);
	}
	else
	{
		g_print("CAN: Listener on localhost:%d\n",8000+ctrl);
	}

	/* listen to the 'incoming' signal */
	g_signal_connect (canHw->pGSocketService,
				"incoming",
				G_CALLBACK (CanSocket_incoming_callback),
				(gpointer)ctrl);

	/* start the socket service */
	g_socket_service_start (canHw->pGSocketService);
}

void Can_SocketEnterCritical(uint8_t ctrl)
{

}
void Can_SocketExitCritical(uint8_t ctrl)
{

}

void Can_SocketTriggerTx(uint8_t ctrl)
{

}

void Can_SocketSetMode(uint8_t controller,Can_StateTransitionType transition)
{

}

