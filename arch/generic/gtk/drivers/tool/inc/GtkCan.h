

#ifndef GTKCAN_H_
#define GTKCAN_H_

#include <glib.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>
#include <Std_Types.h>
typedef struct
{
	guchar      cmd;
	guchar      dlc;
	guint 		id;
	guchar      data[8];
	guchar      bus;  // Identifier of this CAN NODE on the bus
	guint16     reserved;
}GtkCanMsg_Type;

#define GTK_CAN_CMD_TX   ((guchar)0x00)
#define GTK_CAN_CMD_RX   ((guchar)0x01)

#define GTK_CAN_CMD_TX_ACK ((guchar)0x80)
#define GTK_CAN_CMD_RX_ACK ((guchar)0x81)

#endif /* GTKCAN_H_ */
