#include "GtkFL.h"
#include "GtkTp.h"

// ======================== DATA     =============================
static guint    sFL_Step = 0;
static gboolean isFL_Busy = FALSE;
static uint32   FL_Seed = 0;
static uint8    FL_Level = 10;


// ======================== FUNCTION =============================
static void     FL_Response(uint8* data,uint16 size);
static void     FL_Session(void);
static void     FL_SecurityRequestSeed(void);
static void     FL_SecuritySendKey(void);
static void     FL_ReadFingerPrint(void);
static void     FL_WriteFingerPrint(void);
static gboolean FlashLoader(gpointer data);



static void FL_Session(void)
{
	Std_ReturnType ercd;
	static uint8 data[2] = {0x10,0x02};
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		g_print("FL: Change to Program Session");
		isFL_Busy = TRUE;
	}
}
static void     FL_SecurityRequestSeed(void)
{
	Std_ReturnType ercd;
	uint8 data[2] = {0x27,0xFF};

	data[1] = 2*FL_Level-1;
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		g_print("FL: Request Seed");
		isFL_Busy = TRUE;
	}
}
static void     FL_SecuritySendKey(void)
{
	Std_ReturnType ercd;
	uint8 data[6] = {0x27,0xFF};
	uint32 key = ((FL_Seed/7)<<3) - 111;
	data[1] = 2*FL_Level;
	data[2] = ((key>>24)&0xFF);
	data[3] = ((key>>16)&0xFF);
	data[4] = ((key>>8)&0xFF);
	data[5] = ((key)&0xFF);
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		g_print("FL: Send Key");
		isFL_Busy = TRUE;
	}
}
static void     FL_ReadFingerPrint(void)
{
	Std_ReturnType ercd;
	uint8 data[3] = {0x22,0x01,0x0A};
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		g_print("FL: Read Finger Print");
		isFL_Busy = TRUE;
	}
}
static void     FL_WriteFingerPrint(void)
{
	Std_ReturnType ercd;
	uint8 data[3+128] = {0x2E,0x01,0x0A};
	for(int i=0;i<128;i++)
	{
		data[3+i] = i*2;
	}
	ercd = CanTp_Transmit(data,sizeof(data));
	if(E_OK==ercd)
	{
		g_print("FL: Write Finger Print");
		isFL_Busy = TRUE;
	}
}

static void FL_Response(uint8* data,uint16 size)
{
	Std_ReturnType ercd;
	switch(sFL_Step)
	{
		case 0:
			ercd = (data[0]==0x50)?E_OK:E_NOT_OK;
			break;
		case 1:
			ercd = (data[0]==0x67)?E_OK:E_NOT_OK;
			if(E_OK==ercd){FL_Seed=((uint32)data[2]<<24) + ((uint32)data[3]<<16) + ((uint32)data[4]<<8) + (uint32)(data[5]);};
			break;
		case 2:
			ercd = (data[0]==0x67)?E_OK:E_NOT_OK;
			break;
		case 3:
			ercd = (data[0]==0x62)?E_OK:E_NOT_OK;
			break;
		case 4:
			ercd = (data[0]==0x6E)?E_OK:E_NOT_OK;
			break;
		default:
			ercd = E_NOT_OK;
			break;
	}

	g_print(" %s! with response [",(E_OK==ercd)?"OK":"FAILED");
	for(int i=0;i<size;i++)
	{
		g_print("%-2x,",data[i]);
	}
	g_print("]\n");

	if(E_OK==ercd)
	{
		isFL_Busy = FALSE;
		sFL_Step ++;
	}
	else
	{
		isFL_Busy = FALSE;
		sFL_Step = 0xFFFF;
	}
}
gboolean FlashLoader(gpointer data)
{
	if(!isFL_Busy)
	{
		switch(sFL_Step)
		{	case 0:
				FL_Session();
				break;
			case 1:
				FL_SecurityRequestSeed();
				break;
			case 2:
				FL_SecuritySendKey();
				break;
			case 3:
				FL_ReadFingerPrint();
				break;
			case 4:
				FL_WriteFingerPrint();
				break;
			default:
				break;
		}
	}
	return TRUE;
}

void FL_Init(void)
{
	g_idle_add(FlashLoader,NULL);
}


// CALLBACK of CanTP
void CanTp_RxIndication(uint8* data,uint16 length)
{
	FL_Response(data,length);
}
