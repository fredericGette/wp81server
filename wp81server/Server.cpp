#include "pch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "Server.h"

#pragma comment (lib, "Ws2_32.lib")

void debug1(WCHAR* format, ...)
{
	va_list args;
	va_start(args, format);

	WCHAR buffer[1000];
	_vsnwprintf_s(buffer, sizeof(buffer), format, args);

	OutputDebugStringW(buffer);

	va_end(args);
}

// A sample of the select() return value

int recvTimeOutTCP(SOCKET socket, long sec, long usec)
{
	// Setup timeval variable
	struct timeval timeout;
	struct fd_set fds;

	// assign the second and microsecond variables
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	// Setup fd_set structure
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	// Possible return values:
	// -1: error occurred
	// 0: timed out
	// > 0: data ready to be read
	return select(0, &fds, 0, 0, &timeout);
}

// https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedcode1c.html
int startServer()
{
	WSADATA            wsaData;
	SOCKET             ListeningSocket, NewConnection;
	SOCKADDR_IN        ServerAddr, SenderInfo;
	int                Port = 7171;
	// Receiving part
	char			   recvbuff[1024];
	int                ByteReceived, i, nlen, SelectTiming;

	// Initialize Winsock version 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// The WSAGetLastError() function is one of the only functions
		// in the Winsock 2.2 DLL that can be called in the case of a WSAStartup failure
		debug1(L"Server: WSAStartup failed with error %ld.\n", WSAGetLastError());
		// Exit with error
		return 1;
	}
	else
	{
		debug1(L"Server: The Winsock DLL found!\n");
		debug1(L"Server: The current status is %hs.\n", wsaData.szSystemStatus);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		//Tell the user that we could not find a usable WinSock DLL
		debug1(L"Server: The dll do not support the Winsock version %u.%u!\n",LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
		// Do the clean up
		WSACleanup();
		// and exit with error
		return 1;
	}
	else
	{
		debug1(L"Server: The dll supports the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
		debug1(L"Server: The highest version this dll can support is %u.%u\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
	}

	// Create a new socket to listen for client connections.
	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check for errors to ensure that the socket is a valid socket.
	if (ListeningSocket == INVALID_SOCKET)
	{
		debug1(L"Server: Error at socket(), error code : %ld.\n", WSAGetLastError());
		// Clean up
		WSACleanup();
		// and exit with error
		return 1;
	}
	else
		debug1(L"Server: socket() is OK!\n");

	// Set up a SOCKADDR_IN structure that will tell bind that we
	// want to listen for connections on all interfaces using port 7171.

	// The IPv4 family
	ServerAddr.sin_family = AF_INET;
	// host-to-network byte order
	ServerAddr.sin_port = htons(Port);
	// Listen on all interface, host-to-network byte order
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associate the address information with the socket using bind.
	// Call the bind function, passing the created socket and the sockaddr_in
	// structure as parameters. Check for general errors.
	if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		debug1(L"Server: bind() failed!Error code : %ld.\n", WSAGetLastError());
		// Close the socket
		closesocket(ListeningSocket);
		// Do the clean up
		WSACleanup();
		// and exit with error
		return 1;
	}
	else
		debug1(L"Server: bind() is OK!\n");

	// Listen for client connections with a backlog of 5
	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
		debug1(L"Server: listen() : Error listening on socket %ld.\n", WSAGetLastError());
		// Close the socket
		closesocket(ListeningSocket);
		// Do the clean up
		WSACleanup();
		// Exit with error
		return 1;
	}
	else
		debug1(L"Server: listen() is OK, I'm listening for connections...\n");

	debug1(L"Server: listen() during 10s...\n");
	// Set 10 seconds 10 useconds timeout
	SelectTiming = recvTimeOutTCP(ListeningSocket, 10, 10);

	switch (SelectTiming)
	{
	case 0:
		// Timed out, do whatever you want to handle this situation
		debug1(L"\nServer: Timeout while waiting you retard client!...\n");
		break;

	case -1:
		// Error occurred, more tweaking here and the recvTimeOutTCP()...
		debug1(L"\nServer: Some error encountered with code number : %ld\n", WSAGetLastError());
		break;

	default:
	{
		// Accept a new connection when available. 'while' always true
		while (1)
		{
			debug1(L"Server: connexion...\n");
			// Reset the NewConnection socket to SOCKET_ERROR
			// Take note that the NewConnection socket in not listening
			NewConnection = SOCKET_ERROR;
			// While the NewConnection socket equal to SOCKET_ERROR
			// which is always true in this case...
			while (NewConnection == SOCKET_ERROR)
			{
				// Accept connection on the ListeningSocket socket and assign
				// it to the NewConnection socket, let the ListeningSocket
				// do the listening for more connection
				NewConnection = accept(ListeningSocket, NULL, NULL);
				debug1(L"\nServer: accept() is OK...\n");
				debug1(L"Server: New client got connected, ready to	receive and send data...\n");

				// At this point you can do two things with these sockets
				// 1. Wait for more connections by calling accept again
				//    on ListeningSocket (loop)
				// 2. Start sending or receiving data on NewConnection.
				ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);

				// When there is data
				if (ByteReceived > 0)
				{
					debug1(L"Server: recv() looks fine....\n");
					// Some info on the receiver side...
					//getsockname(ListeningSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
					//debug1(L"Server: Receiving IP(s) used : %s\n", inet_ntoa(ServerAddr.sin_addr));
					//debug1(L"Server: Receiving port used : %d\n", htons(ServerAddr.sin_port));

					// Some info on the sender side
					// Allocate the required resources
					memset(&SenderInfo, 0, sizeof(SenderInfo));
					nlen = sizeof(SenderInfo);

					getpeername(NewConnection, (SOCKADDR *)&SenderInfo, &nlen);
					debug1(L"Server: Sending IP used : %hs\n", inet_ntoa(SenderInfo.sin_addr));
					debug1(L"Server: Sending port used : %d\n", htons(SenderInfo.sin_port));

					// Print the received bytes. Take note that this is the total
					// byte received, it is not the size of the declared buffer
					debug1(L"Server: Bytes received : %d\n", ByteReceived);
					// Print what those bytes represent
					debug1(L"Server: Those bytes are : \n");
					// Print the string only, discard other
					// remaining 'rubbish' in the 1024 buffer size
					for (i = 0; i < ByteReceived; i++)
						debug1(L"%c", recvbuff[i]);
					debug1(L"\n");
				}
				// No data
				else if (ByteReceived == 0)
					debug1(L"Server: Connection closed!\n");
				// Others
				else
					debug1(L"Server: recv() failed with error code : %d\n", WSAGetLastError());
			}

			// Clean up all the send/recv communication, get ready for new one
			if (shutdown(NewConnection, SD_SEND) != 0)
				debug1(L"\nServer: Well, there is something wrong with the shutdown().The error code : %ld\n", WSAGetLastError());
			else
				debug1(L"\nServer: shutdown() looks OK...\n");

			// Well, if there is no more connection in 15 seconds,
			// just exit this listening loop...
			debug1(L"Server: listen() during 15s...\n");
			if (recvTimeOutTCP(ListeningSocket, 15, 0) == 0)
				break;
		}
	}
	}

	debug1(L"\nServer: The listening socket is timeout...\n");
	// When all the data communication and listening finished, close the socket
	if (closesocket(ListeningSocket) != 0)
		debug1(L"Server: Cannot close ListeningSocket socket.Error code : %ld\n", WSAGetLastError());
	else
		debug1(L"Server: Closing ListeningSocket socket...\n");

	// Finally and optionally, clean up all those WSA setup
	if (WSACleanup() != 0)
		debug1(L"Server: WSACleanup() failed!Error code : %ld\n", WSAGetLastError());
	else
		debug1(L"Server: WSACleanup() is OK...\n");

	return 0;
}
