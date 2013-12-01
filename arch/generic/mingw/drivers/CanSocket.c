#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include "Can.h"
#include "CanSocket.h"

extern void arch_generate_irqn(IrqType IRQn);

CAN_HW_t Can_HwUnit[CAN_ARC_CTRL_CONFIG_CNT];
static void startSocketup(void)
{
	static boolean started = False;
	if(started == False)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
}
// Local Functions
static void Can_RxMainThread(uint32_t ctrl)
{
	CAN_HW_t *canHw = &Can_HwUnit[ctrl];
	// the listening socket to be created
	SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for accepting incoming requests.
	SOCKET AcceptSocket;
	// The socket address to be passed to bind
	struct sockaddr_in service;
	int ercd;
	int I;
	uint32 port = (8000 + 1000*ctrl)+1;  // try until find one.
	startSocketup();
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket function failed with error: %u\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket function failed with error: %u\n", WSAGetLastError());
		return;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	do{
		//----------------------
		// Bind the socket.
		service.sin_port = (u_short)htons(port);
		ercd = bind(ListenSocket, (SOCKADDR *) &service, sizeof (service));
		if (ercd == SOCKET_ERROR) {
			port ++;
			continue;
		}
		else
		{
			canHw->port = port;
			printf("CAN_CTRL_%d Listen to 127.0.0.1:%d\n",(int)ctrl,(int)port);
			break; // OK
		}
	}while(TRUE);
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 32) == SOCKET_ERROR) {
		printf("listen failed with error: %d\n",WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}
	for(;;)
	{
		Can_IdType id;
		uint8 msg[64];
		Sleep(0);
		AcceptSocket = accept(ListenSocket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			continue;
		}
		ercd = recv(AcceptSocket, (void*)msg, 64, 0);

		if ( ercd == 17 )
		{ // Rx ISR
			WaitForSingleObject(canHw->mutex,INFINITE);
			id = (((uint32)msg[0])<<24) + (((uint32)msg[1])<<16) + (((uint32)msg[2])<<8) + msg[3];
			if( BM_WUPI == (canHw->RIER & BM_WUPI))
			{
				canHw->IRQF |= cCanIsrWak;
				printf("Can Wakeup!\n");
			}
			else
			{
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
					canHw->rxMsg[I].dlc = msg[4];
					memcpy(canHw->rxMsg[I].data,&msg[5],msg[4]);
					canHw->rxMsg[I].state = eCanMsgBoxRxed;
				}
				else
				{
					printf("Can Message Box Full, Message Lost!\n");
				}
				canHw->IRQF |= cCanIsrRx;
			}
			ReleaseMutex(canHw->mutex);
			arch_generate_irqn(SysCan_0_IRQn+ctrl);
		}
		else
		{
			if((ercd != -1) && (ercd != 0))
			{
				printf("ERROR:Invalid CAN message length : %d.\n",ercd);
			}
			else
			{
				printf("ERROR: Something Wrong when receive: %d.\n",ercd);
			}
		}
		ercd = closesocket(AcceptSocket);
		if (ercd == SOCKET_ERROR){
			printf("RX:closesocket function failed with error: %d\n", WSAGetLastError());
		}
	}
}
static void Can_TxMainThread(uint32_t ctrl)
{
	CAN_HW_t *canHw;
	void *pvObjectList[ 2 ];
	struct sockaddr_in sockaddr;
	SOCKET ConnectSocket;
	int ercd;
	uint8_t msg[32];
	canHw = &Can_HwUnit[ctrl];
	pvObjectList[ 0 ] = canHw->mutex;
	pvObjectList[ 1 ] = canHw->txEvent;
	startSocketup();
	// Creat IP address
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = (u_short)htons(8000+1000*ctrl);
	for(;;)
	{
		int i;
		// time may be not that accurate
		Sleep(0); // slow it down
		WaitForMultipleObjects( sizeof( pvObjectList ) / sizeof( void * ), pvObjectList, True, INFINITE );
		assert((canHw->TIER & BM_TX0) == BM_TX0);
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket function failed with error: %d\n", WSAGetLastError());
		}
		// Connect to server.
		ercd = connect(ConnectSocket, (SOCKADDR *) & sockaddr, sizeof (SOCKADDR));
		if (ercd == SOCKET_ERROR) {
			printf("connect function failed with error: %d\n", WSAGetLastError());
			ercd = closesocket(ConnectSocket);
			if (ercd == SOCKET_ERROR){
				printf("closesocket function failed with error: %d\n", WSAGetLastError());
			}
		}
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
		msg[13] = (uint8)(canHw->port>>24);
		msg[14] = (uint8)(canHw->port>>16);
		msg[15] = (uint8)(canHw->port>>8);
		msg[16] = (uint8)(canHw->port);
		send(ConnectSocket,(const char*)msg,17,0);
		ercd = closesocket(ConnectSocket);
		if (ercd == SOCKET_ERROR){
			printf("closesocket function failed with error: %d\n", WSAGetLastError());
		}
		// Clear TIER
		canHw->TIER &= ~(BM_TX0);
		canHw->IRQF |= cCanIsrTx;

		ReleaseMutex(canHw->mutex);
		arch_generate_irqn(SysCan_0_IRQn+ctrl);
	}
}

static HANDLE canCreSecondaryThread(void (*entry)(uint32_t),uint8_t ctrl)
{
	HANDLE handle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)entry, (LPVOID)(uint32_t)ctrl, CREATE_SUSPENDED, NULL );
	if( handle != NULL )
	{
		SetThreadPriority( handle, THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriorityBoost( handle, TRUE );
		SetThreadAffinityMask( handle, 0x01 );
	}
	else
	{
		assert(False);
	}
	return handle;
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
	if(Can_HwUnit[ctrl].rxThread != NULL)
	{
		TerminateThread(Can_HwUnit[ctrl].rxThread,0);
	}
	if(Can_HwUnit[ctrl].txThread != NULL)
	{
		TerminateThread(Can_HwUnit[ctrl].txThread,0);
	}
	if(Can_HwUnit[ctrl].mutex != NULL)
	{
		CloseHandle(Can_HwUnit[ctrl].mutex);
	}
	if(Can_HwUnit[ctrl].txEvent != NULL)
	{
		CloseHandle(Can_HwUnit[ctrl].txEvent);
	}
}
void Can_SocketInit(uint8_t ctrl)
{
	Can_SocketDeInit(ctrl);
	Can_HwUnit[ctrl].txEvent  = CreateEvent( NULL, FALSE, FALSE, NULL );
	Can_HwUnit[ctrl].mutex    = CreateMutex( NULL, FALSE, NULL );
	Can_HwUnit[ctrl].rxThread = canCreSecondaryThread(Can_RxMainThread,ctrl);
	Can_HwUnit[ctrl].txThread = canCreSecondaryThread(Can_TxMainThread,ctrl);
}

void Can_SocketEnterCritical(uint8_t ctrl)
{
	WaitForSingleObject(Can_HwUnit[ctrl].mutex,INFINITE);
}
void Can_SocketExitCritical(uint8_t ctrl)
{
	ReleaseMutex(Can_HwUnit[ctrl].mutex);
}

void Can_SocketTriggerTx(uint8_t ctrl)
{
	SetEvent(Can_HwUnit[ctrl].txEvent);
}

void Can_SocketSetMode(uint8_t controller,Can_StateTransitionType transition)
{
	CAN_HW_t *canHw;
	canHw = &Can_HwUnit[controller];
	switch(transition )
	{
		case CAN_T_START:
			ResumeThread(canHw->rxThread);
			ResumeThread(canHw->txThread);
		break;
		case CAN_T_WAKEUP:
			ResumeThread(canHw->rxThread);
			ResumeThread(canHw->txThread);
		break;
		case CAN_T_SLEEP:  //CAN258, CAN290
			ResumeThread(canHw->rxThread);
			SuspendThread(canHw->txThread);
		break;
		case CAN_T_STOP:
			SuspendThread(canHw->rxThread);
			SuspendThread(canHw->txThread);
		break;
		default:
		break;
	}
}

