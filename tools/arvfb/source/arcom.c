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
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */

#include "arvfb.h"

// ======================== [ LOCAL MACROS    ] ==============================================
#define ARCOM_PDU_NUMBER  1024  // Is 1024 enough

#define ARCOM_NAME_LENGTH  64

#define ARCOM_SIGNALS_IN_PDU 64

// ======================== [ LOCAL TYPES     ] ==============================================
typedef struct
{
	char  Name[ARCOM_NAME_LENGTH];
	uint8  StartBit;
	uint8  BitSize;
	void*  Pdu;       // pointer to my container PDU
	GtkWidget* Entry; // EntryWidget
}ArComSignalType;

typedef struct
{
	char   Name[ARCOM_NAME_LENGTH];
	uint32  Identifier;	 // refer the ArCanMsg Identifier
	uint8   BusID;		 // refer the ArCanMsg BusID
	boolean IsTxEnabled; // If True, arcom will transmit it.
	uint32  Period;		 // unit in ms
	uint32  Timer;
	uint8   Data[8];
	uint8   SignalNbr;   // assert(SignalNbr<=64 && SignalNbr>0)
	ArComSignalType Signals[ARCOM_SIGNALS_IN_PDU];
}ArComPduType;

typedef struct
{
	uint32       PduNbr;
	ArComPduType Pdu[ARCOM_PDU_NUMBER];
}ArComArchType;

// ======================== [ LOCAL VARIANTS  ] ==============================================
static ArComArchType sArch;

static GTimer* pSysTimer;
static gboolean isSysTimerStarted;


// ======================== [ LOCAL FUNCTIONS ] ==============================================
static void StartTimer(void)
{
	g_timer_start(pSysTimer);
	isSysTimerStarted = TRUE;
}

static void StopTimer(void)
{
	g_timer_stop(pSysTimer);
	isSysTimerStarted = FALSE;
}
static gboolean IsTimerElapsed(gulong microseconds)
{
	gboolean rv;
	if(isSysTimerStarted)
	{
		gdouble elapsed;
		gulong  elapsed_microseconds;

		elapsed = g_timer_elapsed(pSysTimer,NULL);
		elapsed_microseconds = (elapsed*1000);

		if(elapsed_microseconds>microseconds)
		{
			rv = TRUE;
		}
		else
		{
			rv = FALSE;
		}
	}
	else
	{
		rv = TRUE;
	}
	return rv;
}

static uint32 GetTimerElapsedMicroSeconds()
{
	uint32 rv;
	if(isSysTimerStarted)
	{
		gdouble elapsed;
		gulong  elapsed_microseconds;

		elapsed = g_timer_elapsed(pSysTimer,NULL);
		elapsed_microseconds = (elapsed*1000);

		rv = elapsed_microseconds;
	}
	else
	{
		rv = 0;
	}
	return rv;
}

static void Write(const ArComSignalType* arsig,uint32 SigValue)
{

    int BA = 0;
    int bitsize = arsig->BitSize;
    int SigSize = arsig->BitSize;
    int start   = arsig->StartBit;
    int SigStart= arsig->StartBit;
    int value   = SigValue;
    ArComPduType* Pdu = (ArComPduType*)arsig->Pdu;
    uint8* data = Pdu->Data;;
    int pos = start/8;
    int CrossB = (SigSize+7)/8;
    if(SigStart>=(pos*8) && (SigStart+SigSize)<=(pos+CrossB)*8)
    {
    }
    else
    {
        CrossB += 1;
    }
    for(int i=0;i<CrossB;i++)
    {
        start   += BA;   // bit accessed in this cycle
        bitsize -= BA;
        pos = start/8;
        int offset = start%8;
        if((8-offset) > bitsize){
            BA =  bitsize;
        }
        else
        {
            BA = (8-offset);
        }
        int BM = ((1<<BA)-1)<<offset;
        data[pos] &=  ~BM;
        data[pos] |=  BM&(value<<offset);
        value = value>>(bitsize-BA);
    }
}
static uint32 Read(const ArComSignalType* arsig)
{
	int value   = 0;
	int SigSize = arsig->BitSize;
	int SigStart= arsig->StartBit;
	ArComPduType* Pdu = (ArComPduType*)arsig->Pdu;
	uint8* data = Pdu->Data;

	int pos = SigStart/8;
	int CrossB = (SigSize+7)/8;
	if(SigStart>=(pos*8) && (SigStart+SigSize)<=(pos+CrossB)*8)
	{
	}
	else
	{
		CrossB += 1;
	}
	for(int i=0;i<CrossB;i++)
	{
		value = value+(data[pos+i]<<(8*i));
	}
	int offset = SigStart%8;
	return (value>>offset)&((1<<SigSize)-1);
}

static void Refresh(const ArComPduType *Pdu)
{
	for (int j = 0; j<Pdu->SignalNbr ; j++) {
		const ArComSignalType* Signal = &(Pdu->Signals[j]);
		GtkWidget* pEntry = Signal->Entry;
		gchar value[32];
		sprintf(value,"%d",Read(Signal));
		gtk_entry_set_text(GTK_ENTRY(pEntry),value);
	}
}

static void Transmit(const ArComPduType *Pdu)
{
	ArMsgType armsg;
	ArCanMsgType* pMsg = (ArCanMsgType*)&armsg;
	pMsg->Type    = MSG_ON_CAN;
	pMsg->Command = MSG_CMD_SEND;
	pMsg->Length  = MSG_CAN_LENGTH;
	pMsg->Msg.BusID = Pdu->BusID;
	pMsg->Msg.Identifier = Pdu->Identifier;
	pMsg->Msg.DataLengthCode = 8;
	memcpy(pMsg->Msg.Data,Pdu->Data,8);
	Can_Transmit(&armsg);
}

static void DefinePdu( char* Name,uint32 Identifier,uint8 BusID,boolean IsTxEnabled,
						uint32 Period)
{
	if(sArch.PduNbr < ARCOM_PDU_NUMBER)
	{
		ArComPduType* Pdu = &(sArch.Pdu[sArch.PduNbr]);
		if(strlen(Name)<ARCOM_NAME_LENGTH)
		{
			strcpy(Pdu->Name,Name);
		}
		else
		{
			g_assert(0);
		}
		Pdu->Identifier  = Identifier;
		Pdu->BusID       = BusID;
		Pdu->IsTxEnabled = IsTxEnabled;
		Pdu->Period      = Period;
		sArch.PduNbr ++;
	}
	else
	{
		g_assert(0);
	}
}

static void DefineSignal( char* Name,uint8 StartBit,uint8 BitSize,uint32 DefaultValue)
{
	if( (sArch.PduNbr <= ARCOM_PDU_NUMBER) &&
		(sArch.PduNbr > 0) )
	{
		ArComPduType* Pdu = &(sArch.Pdu[sArch.PduNbr-1]);
		if(Pdu->SignalNbr < ARCOM_SIGNALS_IN_PDU)
		{
			ArComSignalType* Signal = &(Pdu->Signals[Pdu->SignalNbr]);
			if(strlen(Name)<ARCOM_NAME_LENGTH)
			{
				strcpy(Signal->Name,Name);
			}
			else
			{
				g_assert(0);
			}
			Signal->BitSize  = BitSize;
			Signal->StartBit = StartBit;
			Signal->Pdu      = (void*)Pdu;
			Write(Signal,DefaultValue);
			Pdu->SignalNbr ++;
		}
	}
	else
	{
		g_assert(0);
	}
}

static void on_entry_signal_activate(GtkEntry *entry,gpointer data)
{
	const gchar* pValue = gtk_entry_get_text(entry);

	ArComSignalType* Signal = (ArComSignalType*)data;

	uint32 max = (1u<<Signal->BitSize)-1u;

	uint32 value = atoi(pValue);

	if(value <= max)
	{
		Write(Signal,value);
	}
	else
	{
		gchar value[64];
		sprintf(value,"%d",max);
		gtk_entry_set_text(GTK_ENTRY(entry),value);
		Write(Signal,max);
	}
}

static void on_entry_period_activate(GtkEntry *entry,gpointer data)
{
	const gchar* pValue = gtk_entry_get_text(entry);

	ArComPduType* Pdu = (ArComPduType*)data;


	uint32 value = atoi(pValue);

	Pdu->Period = value;

	Arch_Trace("Change %s\'s period to %d\n",Pdu->Name,Pdu->Period);

	if(0u==Pdu->Period)
	{
		Arch_Trace("\t as Period is zero, transmission is disabled.\n");
	}

}

static void on_button_trigger_transmit_clicked(GtkEntry *entry,gpointer data)
{
	ArComPduType* Pdu = (ArComPduType*)data;

	Arch_Trace("Trigger %s\'s transmission\n",Pdu->Name);
	Transmit(Pdu);

}

static GtkWidget* ComPage(ArComPduType* Pdu)
{
	GtkWidget* pGrid;

	pGrid = gtk_grid_new();

	{
		GtkWidget* pEntry = gtk_entry_new();
		gtk_grid_attach(GTK_GRID(pGrid),gtk_label_new("Period"),0,0,1,1);
		gtk_grid_attach(GTK_GRID(pGrid),pEntry,1,0,1,1);
		gtk_entry_set_width_chars(GTK_ENTRY(pEntry),32);
		gchar value[32];
		sprintf(value,"%d",Pdu->Period);
		gtk_entry_set_text(GTK_ENTRY(pEntry),value);
		gtk_widget_set_sensitive(GTK_WIDGET(pEntry),Pdu->IsTxEnabled);
		if(Pdu->IsTxEnabled)
		{
			g_signal_connect(G_OBJECT (pEntry), "activate",
				G_CALLBACK(on_entry_period_activate) , (gpointer)(Pdu));

			GtkWidget* pButton = gtk_button_new_with_label("Trigger Transmit");
			g_signal_connect(G_OBJECT (pButton), "clicked",
					G_CALLBACK(on_button_trigger_transmit_clicked) , (gpointer)(Pdu));

			gtk_grid_attach(GTK_GRID(pGrid),pButton,2,0,1,1);
		}


	}
	for (int j = 0; j<Pdu->SignalNbr ; j++) {
		ArComSignalType * Signal = &(Pdu->Signals[j]);
		ArComPduType* Pdu = (ArComPduType*)Signal->Pdu;
		GtkWidget* pEntry = gtk_entry_new();
		Signal->Entry = pEntry;
		int J = j+2;
		gtk_grid_attach(GTK_GRID(pGrid),gtk_label_new(Signal->Name),(J%2)*2,J/2,1,1);
		gtk_grid_attach(GTK_GRID(pGrid),pEntry,(J%2)*2+1,J/2,1,1);
		gtk_entry_set_width_chars(GTK_ENTRY(pEntry),32);
		gchar value[32];
		if(TRUE)
		{   // TODO: support array types
			sprintf(value,"%d",Read(Signal));
		}
		else
		{
			sprintf(value,"un-support type");
		}
		gtk_entry_set_text(GTK_ENTRY(pEntry),value);
		gtk_widget_set_sensitive(GTK_WIDGET(pEntry),Pdu->IsTxEnabled);

		if(Pdu->IsTxEnabled)
		{
			g_signal_connect(G_OBJECT (pEntry), "activate",
				G_CALLBACK(on_entry_signal_activate) , (gpointer)(Signal));
		}
	}

	return pGrid;
}

// ======================== [ FUNCTIONS ] ==============================================
void ArCom_Init(void)
{
	memset(&sArch,0,sizeof(sArch));

	isSysTimerStarted = FALSE;
	pSysTimer = g_timer_new();

	StopTimer();

	// ABS Define
//	DefinePdu("ABS_0x102",0x102,2,TRUE,100);
//	DefineSignal("SpeedKmph",0 ,16,18000); // 180 Kmph
//	DefineSignal("TachoRpm" ,16,16,5000);  // 5000rpm
//	DefineSignal("Led1Sts"  ,32,2 ,0);
//	DefineSignal("Led2Sts"  ,34,2 ,1);
//	DefineSignal("Led3Sts"  ,36,2 ,2);
//
//	// Time Define
//	DefinePdu("Time_0x101",0x101,2,FALSE,100);
//	DefineSignal("Year"   , 0   , 16 ,2014 );
//	DefineSignal("Month"  , 16  , 8  ,4    );
//	DefineSignal("Day"    , 24  , 8  ,4    );
//	DefineSignal("Hour"   , 32  , 8  ,23   );
//	DefineSignal("Minute" , 32  , 8  ,37   );
//	DefineSignal("Second" , 32  , 8  ,00   );

}

void ArCom_Schedule(void)
{
	static uint32 I = 0;
	// TODO
	if(IsTimerElapsed(1))
	{
		uint32 elapsed = GetTimerElapsedMicroSeconds();
		for (uint32 i=0; i<sArch.PduNbr; i++)
		{
			ArComPduType* Pdu = &(sArch.Pdu[i]);
			if( (Pdu->IsTxEnabled) &&
				(Pdu->Period != 0) &&
				(Pdu->Timer < Pdu->Period))
			{
				Pdu->Timer += elapsed;
			}
		}
		StartTimer();
	}

	for (; I<sArch.PduNbr; I++)
	{
		ArComPduType* Pdu = &(sArch.Pdu[I]);
		if( (Pdu->IsTxEnabled) &&
			(Pdu->Period != 0) &&
			(Pdu->Timer >= Pdu->Period)) // Elapsed Timer
		{
			Transmit(Pdu);
			Pdu->Timer = 0; // Reset it

			I++;
			break;          // Each time, only transmit 1 item
		}
	}

	if(I >= sArch.PduNbr)
	{
		I = 0;
	}
}

void ArCom_RxIndication(const ArCanMsgType* armsg)
{
	for (uint32 i=0; i<sArch.PduNbr; i++)
	{
		ArComPduType* Pdu = &(sArch.Pdu[i]);
		if ( (Pdu->BusID==armsg->Msg.BusID) && (Pdu->Identifier==armsg->Msg.Identifier) )
		{
			memcpy(Pdu->Data,armsg->Msg.Data,8);
			Refresh(Pdu);
		}
	}

}
GtkWidget* ArCom(void)
{
	GtkWidget* pNotebook;

	pNotebook = gtk_notebook_new ();

	for (uint32 i = 0; i<sArch.PduNbr; i++)
	{
		ArComPduType* Pdu = &(sArch.Pdu[i]);
		gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),ComPage(Pdu),gtk_label_new(Pdu->Name));
	}

	return pNotebook;
}


void ArCom_DefinePdu( char* Name,uint32 Identifier,uint8 BusID,boolean IsTxEnabled,
						uint32 Period)
{
	DefinePdu(Name,Identifier,BusID,IsTxEnabled,Period);
}

void ArCom_DefineSignal( char* Name,uint8 StartBit,uint8 BitSize,uint32 DefaultValue)
{
	DefineSignal(Name,StartBit,BitSize,DefaultValue);
}

