
#include <string.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include "Stmo.h"

static HANDLE   thread = NULL;
static HANDLE   mutex = NULL;
static uint16_t  StmoDegree[2] = {0,0};
static void ClientThread(void)
{
	SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for accepting incoming requests.
	SOCKET AcceptSocket;
	// The socket address to be passed to bind
	struct sockaddr_in service;
	int ercd;

	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket function failed with error: 1 %u\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket function failed with error: 2 %u\n", WSAGetLastError());
		return;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	// Bind the socket.
	service.sin_port = (u_short)htons(60002);
	ercd = bind(ListenSocket, (SOCKADDR *) &service, sizeof (service));
	if (ercd == SOCKET_ERROR) {
		printf("Dio:AUTOSAR Socket Port 60000 is occupied by Others.\n");
	}
	else
	{
		printf("Dio:AUTOSAR Socket Listen On Port 60000.\n");
	}
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 32) == SOCKET_ERROR) {
		printf("listen failed with error: %d\n",WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}
	for(;;)
	{
		AcceptSocket = accept(ListenSocket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			continue;
		}

		for(;;)
		{
			uint8 ldegree[eStmoCount*2];
			for(int i=0;i<eStmoCount;i++)
			{
				ldegree[i*2+0] = 0xFFu&(StmoDegree[i]>>8);
				ldegree[i*2+1] = 0xFFu&(StmoDegree[i]>>0);
			}
			WaitForSingleObject(mutex,INFINITE);
			ercd = send(AcceptSocket, (void*)&ldegree, sizeof(ldegree), 0);
			ReleaseMutex(mutex);
			if(-1 == ercd)
			{
				printf("Gauge: Simulator Off.\n");
				break;
			}
			Sleep(10);
		}

		ercd = closesocket(AcceptSocket);
		if (ercd == SOCKET_ERROR){
			printf("closesocket function failed with error: %d\n", WSAGetLastError());
		}
	}
}
static void StartClient(void)
{
	thread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)NULL, 0, NULL );
	if( thread != NULL )
	{
		SetThreadPriority( thread, THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriorityBoost( thread, TRUE );
		SetThreadAffinityMask( thread, 0x01 );
	}
	else
	{
		assert(0);
	}
	mutex = CreateMutex( NULL, FALSE, NULL );
}

void Stmo_Init(void)
{
	StartClient();
}
Std_ReturnType Stmo_SetPosDegree(Stmo_IdType Id,Stmo_DegreeType Degree)
{
	Std_ReturnType ercd = E_NOT_OK;
	if(Id < eStmoCount)
	{
		if(StmoDegree[Id] != Degree)
		{
			printf("StmoDegree[%d] = %d\n",Id,Degree);
		}
		WaitForSingleObject(mutex,INFINITE);
		StmoDegree[Id] = Degree;
		ReleaseMutex(mutex);
		ercd = E_OK;
	}
	return ercd;
}
