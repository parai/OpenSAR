/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/
#include <string.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include "Std_Types.h"
#include "Dio.h"
#include "Port.h"
#if defined(USE_DET)
#include "Det.h"
#endif



#define DIO_GET_PORT_FROM_CHANNEL_ID(_channelId) (_channelId / 8)
#define DIO_GET_BIT_FROM_CHANNEL_ID(_channelId) (1 << (_channelId % 8))
#define CHANNEL_PTR		(&DioChannelConfigData)
#define CHANNEL_GRP_PTR	(&DioConfigData)
#define PORT_PTR		(&DioPortConfigData)

// ================= Types

typedef struct
{
	uint8_t Level[DIO_PORT_NUM];
	uint8_t Direction[DIO_PORT_NUM];
}DioReg_Type;

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
static int Channel_Config_Contains(Dio_ChannelType channelId)
{
	Dio_ChannelType* ch_ptr=(Dio_ChannelType*)CHANNEL_PTR;
	int rv=0;
	while ((Dio_ChannelType)DIO_END_OF_LIST!=*ch_ptr)
	{
		if (*ch_ptr==channelId)
		{
			rv=1;
			break;
		}
		ch_ptr++;
	}
	return rv;
}

static int Port_Config_Contains(Dio_PortType portId)
{
	Dio_PortType* port_ptr=(Dio_PortType*)PORT_PTR;
	int rv=0;
	while ((Dio_PortType)DIO_END_OF_LIST!=*port_ptr)
	{
		if (*port_ptr==portId)
		{
			rv=1;
			break;
		}
		port_ptr++;
	}
	return rv;
}

static int Channel_Group_Config_Contains(const Dio_ChannelGroupType* _channelGroupIdPtr)
{
	Dio_ChannelGroupType* chGrp_ptr=(Dio_ChannelGroupType*)CHANNEL_GRP_PTR;
	int rv=0;

	while ((Dio_PortType)DIO_END_OF_LIST!=chGrp_ptr->port)
	{
		if (chGrp_ptr->port==_channelGroupIdPtr->port
				&& chGrp_ptr->offset==_channelGroupIdPtr->offset
				&& chGrp_ptr->mask==_channelGroupIdPtr->mask)
		{
			rv=1;
			break;
		}
		chGrp_ptr++;
	}
	return rv;
}

#define VALIDATE_CHANNEL(_channelId, _api) \
	if(0==Channel_Config_Contains(channelId)) {	\
		Det_ReportError(MODULE_ID_DIO,0,_api,DIO_E_PARAM_INVALID_CHANNEL_ID ); \
		level = 0;	\
		goto cleanup;	\
		}
#define VALIDATE_PORT(_portId, _api)\
	if(0==Port_Config_Contains(_portId)) {\
		Det_ReportError(MODULE_ID_DIO,0,_api,DIO_E_PARAM_INVALID_PORT_ID ); \
		level = STD_LOW;\
		goto cleanup;\
	}
#define VALIDATE_CHANNELGROUP(_channelGroupIdPtr, _api)\
	if(0==Channel_Group_Config_Contains(_channelGroupIdPtr)) {\
		Det_ReportError(MODULE_ID_DIO,0,_api,DIO_E_PARAM_INVALID_GROUP_ID ); \
		level = STD_LOW;\
		goto cleanup;\
	}
#else
#define VALIDATE_CHANNEL(_channelId, _api)
#define VALIDATE_PORT(_portId, _api)
#define VALIDATE_CHANNELGROUP(_channelGroupIdPtr, _api)
#endif

static DioReg_Type Dio_Reg = {
	.Level = {0,0,0,0,0,0,0,0,0,0,0},
	.Direction = {0,0,0,0,0,0,0,0,0,0,0}
};
static boolean     isDioClientStarted = False;
static HANDLE   thread = NULL;
static HANDLE   mutex = NULL;
static void DioClientThread(void)
{
	SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for accepting incoming requests.
	SOCKET AcceptSocket;
	// The socket address to be passed to bind
	struct sockaddr_in service;
	int ercd;
	int I;

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
	service.sin_port = (u_short)htons(60000);
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
			WaitForSingleObject(mutex,INFINITE);
			ercd = send(AcceptSocket, (void*)&Dio_Reg, sizeof(Dio_Reg), 0);
			recv(AcceptSocket, (void*)&Dio_Reg, sizeof(Dio_Reg), 0);
			ReleaseMutex(mutex);
			if(-1 == ercd)
			{
				printf("Dio: Simulator Off.\n");
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
static void DioStartClient(void)
{
	thread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DioClientThread, (LPVOID)NULL, 0, NULL );
	if( thread != NULL )
	{
		SetThreadPriority( thread, THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriorityBoost( thread, TRUE );
		SetThreadAffinityMask( thread, 0x01 );
	}
	else
	{
		assert(False);
	}
	mutex = CreateMutex( NULL, FALSE, NULL );
	isDioClientStarted = True;
}
Dio_LevelType Dio_ReadChannel(Dio_ChannelType channelId)
{
	Dio_LevelType level;
	VALIDATE_CHANNEL(channelId, DIO_READCHANNEL_ID);

	Dio_PortLevelType portVal = Dio_ReadPort(DIO_GET_PORT_FROM_CHANNEL_ID(channelId));
	Dio_PortLevelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(channelId);

	if ((portVal & bit) != STD_LOW){
		level = STD_HIGH;
	} else {
		level = STD_LOW;
	}

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
	cleanup:
#endif
	return (level);
}
void exDio_SetPinDirection(uint32_t channelId,Port_PinDirectionType Direction)
{
	uint8_t portDir;
	uint8_t bit;
	if(channelId < Port_PIN_NUM)
	{
		portDir = Dio_Reg.Direction[DIO_GET_PORT_FROM_CHANNEL_ID(channelId)];
		bit = DIO_GET_BIT_FROM_CHANNEL_ID(channelId);
		if(PORT_PIN_IN == Direction)
		{
			portDir &= ~bit;
		}
		else
		{
			portDir |= bit;
		}
		Dio_Reg.Direction[DIO_GET_PORT_FROM_CHANNEL_ID(channelId)] = portDir;
	}
}
void Dio_WriteChannel(Dio_ChannelType channelId, Dio_LevelType level)
{
	VALIDATE_CHANNEL(channelId, DIO_WRITECHANNEL_ID);

	Dio_PortLevelType portVal = Dio_ReadPort(DIO_GET_PORT_FROM_CHANNEL_ID(channelId));
	Dio_PortLevelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(channelId);

	if(level == STD_HIGH){
		portVal |= bit;
	} else {
		portVal &= ~bit;
	}

	Dio_WritePort(DIO_GET_PORT_FROM_CHANNEL_ID(channelId), portVal);

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
	cleanup:
#endif
	return;
}

Dio_PortLevelType Dio_ReadPort(Dio_PortType portId)
{
	Dio_LevelType level = 0;
	VALIDATE_PORT(portId, DIO_READPORT_ID);

	if(False == isDioClientStarted)
	{
		DioStartClient();
	}
	WaitForSingleObject(mutex,INFINITE);
	level = Dio_Reg.Level[portId];
	ReleaseMutex(mutex);

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
	cleanup:
#endif
	return level;
}

void Dio_WritePort(Dio_PortType portId, Dio_PortLevelType level)
{
    VALIDATE_PORT(portId, DIO_WRITEPORT_ID);
	if(False == isDioClientStarted)
	{
		DioStartClient();
	}
	WaitForSingleObject(mutex,INFINITE);
    // TODO: simulation run, a port only has 8 pins
    Dio_Reg.Level[portId] = (uint8_t)level;
    ReleaseMutex(mutex);

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
    cleanup:
#endif
    return;
}

Dio_PortLevelType Dio_ReadChannelGroup(
    const Dio_ChannelGroupType *channelGroupIdPtr)
{
	Dio_LevelType level;
	VALIDATE_CHANNELGROUP(channelGroupIdPtr,DIO_READCHANNELGROUP_ID);

	// Get masked values
	level = Dio_ReadPort(channelGroupIdPtr->port) & channelGroupIdPtr->mask;

	// Shift down
	level = level >> channelGroupIdPtr->offset;

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
	cleanup:
#endif
	return level;
}

void Dio_WriteChannelGroup(const Dio_ChannelGroupType *channelGroupIdPtr,
    Dio_PortLevelType level)
{
	VALIDATE_CHANNELGROUP(channelGroupIdPtr,DIO_WRITECHANNELGROUP_ID);

	// Shift up and apply mask so that no unwanted bits are affected
	level = (level << channelGroupIdPtr->offset) & channelGroupIdPtr->mask;

	// Read port and clear out masked bits
	Dio_PortLevelType portVal = Dio_ReadPort(channelGroupIdPtr->port) & (~channelGroupIdPtr->mask);

	// Or in the upshifted masked level
	portVal |= level;

	Dio_WritePort(channelGroupIdPtr->port, portVal);

#if ( DIO_DEV_ERROR_DETECT == STD_ON )
	cleanup:
#endif
	return;
}
