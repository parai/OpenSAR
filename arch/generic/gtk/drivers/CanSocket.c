
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "Can.h"
#include "CanSocket.h"

#define GTK_CAN_CMD_TX   ((guchar)0x00)
#define GTK_CAN_CMD_RX   ((guchar)0x01)

#define GTK_CAN_CMD_TX_ACK ((guchar)0x80)
#define GTK_CAN_CMD_RX_ACK ((guchar)0x81)

CAN_HW_t Can_HwUnit[CAN_ARC_CTRL_CONFIG_CNT];
extern void arch_generate_irqn(IrqType IRQn);
static void CanSocket_Tx(uint8 ctrl,GSocketConnection *connection);
static void CanSocket_Rx(uint8 ctrl,GtkCanMsgBox_Type* gtkCanMsg,uint8 size);

static gboolean CanSocket_incoming_callback  (GSocketService *service,
											GSocketConnection *connection,
											GObject *source_object,
											gpointer user_data)
{
	GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	GtkCanMsgBox_Type gtkCanMsg;
	gssize size = g_input_stream_read  (istream,
						&gtkCanMsg,
						sizeof(GtkCanMsgBox_Type),
						NULL,
						NULL);

	switch(gtkCanMsg.cmd)
	{
		case GTK_CAN_CMD_TX:
			CanSocket_Tx((uint8)(user_data),connection);
			break;
		case GTK_CAN_CMD_RX:
			CanSocket_Rx((uint8)(user_data),&gtkCanMsg,size);
			break;
		default:
			g_print("CAN:Error Command!\n");
			break;
	}

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
	GError* error;
	GtkCanMsgBox_Type gtkCanMsg;
	CAN_HW_t *canHw;
	canHw = &Can_HwUnit[ctrl];
	GtkCanMsgBox_Type* pMsgBox = GtkCanGetBusyMsgBox(&(canHw->txQ));
	static const gchar dumy_ack = GTK_CAN_CMD_TX_ACK;
	GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	if(NULL != pMsgBox)
	{
		int i;
		gtkCanMsg.cmd = GTK_CAN_CMD_TX_ACK;
		gtkCanMsg.id = pMsgBox->id;
		gtkCanMsg.dlc = pMsgBox->dlc;
		if(gtkCanMsg.dlc > 8)
		{
			gtkCanMsg.dlc = 8;
		}
		for(i=0;i<gtkCanMsg.dlc;i++)
		{
			gtkCanMsg.data[i] = pMsgBox->data[i];
		}
		for(;i<8;i++)
		{
			gtkCanMsg.data[i]  = 0x55;  // PADDING
		}
//		{
//			g_print("CANID=0x%-3x,DLC=%x, [",gtkCanMsg.id,(guint)gtkCanMsg.dlc);
//			for(int i=0;i<8;i++)
//			{
//				g_print("%-2x,",(guint)gtkCanMsg.data[i]);
//			}
//			g_print("]\n");
//		}
		gtkCanMsg.bus = ctrl;
		g_output_stream_write  (ostream,
								&gtkCanMsg, /* your message goes here */
								sizeof(GtkCanMsgBox_Type), /* length of your message */
								NULL,
								&error);
		// Clear TIER
		canHw->TIER &= ~(BM_TX0);
		canHw->IRQF |= cCanIsrTx;

		Can_SetPduHandle(ctrl,pMsgBox->swPduHandle);
		// Request System Isr
		arch_generate_irqn(SysCan_0_IRQn+ctrl);
	}
	else
	{
		g_output_stream_write  (ostream,
								&dumy_ack, /* your message goes here */
								1, /* length of your message */
								NULL,
								&error);
	}
}

static void CanSocket_Rx(uint8 ctrl,GtkCanMsgBox_Type* gtkCanMsg,uint8 size)
{
	CAN_HW_t *canHw;
	canHw = &Can_HwUnit[ctrl];
	if ( size == sizeof(GtkCanMsgBox_Type) )
	{ // Rx ISR
		if( BM_WUPI == (canHw->RIER & BM_WUPI))
		{
			canHw->IRQF |= cCanIsrWak;
			printf("Can Wakeup!\n");
		}
		else
		{
		    GtkCanMsgBox_Type* pMsgBox = GtkCanGetEmptyMsgBox(&(canHw->rxQ));
		    Can_SocketEnterCritical(ctrl);
		    // check for any free box
		    if(NULL != pMsgBox) {
				memcpy(pMsgBox,gtkCanMsg,sizeof(GtkCanMsgBox_Type));
				if(pMsgBox->dlc>8)
				{
					pMsgBox->dlc = 8;
				}
			}
			else
			{
				printf("Can Message Box Full, Message Lost!\n");
			}
			canHw->IRQF |= cCanIsrRx;
			Can_SocketExitCritical(ctrl);
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

