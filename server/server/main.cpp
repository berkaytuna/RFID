// placeholder

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "server.h"
#include <stdio.h>
#include <winsock.h>
#include <fcntl.h>
#include <stdbool.h>
#include <iostream> 
#include <thread> 
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define DEFAULT_BUFLEN 512
#define ANZAHL_8150_CLIENTS 5

int clientSockets[ANZAHL_8150_CLIENTS] = { };
thread clientThreads[ANZAHL_8150_CLIENTS] = { };
int maxClientNum = ANZAHL_8150_CLIENTS;

void delay(int ms) {
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}

inline void printClients()
{
	for (int i = 0; i < maxClientNum; i++)
	{
		printf("clientSocket %d: %d\n", i, clientSockets[i]);
	}
	printf("\n");
}

void communicateClient(int clientSocket)
{
	char recvBuf[DEFAULT_BUFLEN];
	int recvBufLen = DEFAULT_BUFLEN;

	while (1)
	{
		int recvResult = recv(clientSocket, recvBuf, recvBufLen, 0);
		printf("recvResult: %d\n", recvResult);
		printf("%s\n", recvBuf);
		
		//printf("%c\n",recvBuf[0]);
	}
}

void acceptConnection(int socketServer)
{
	printClients();

	while (1)
	{
		// select
		fd_set set;
		FD_ZERO(&set); /* clear the set */
		FD_SET(socketServer, &set); /* add our file descriptor to the set */
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int selectResult = select(socketServer + 1, &set, NULL, NULL, &timeout);
		if (selectResult == -1)
		{
			printf("selectResult: %d\n\n", selectResult);
		}
		else
		{
			// accept
			struct sockaddr_in client;
			unsigned long addrClient;

			memset(&client, 0, sizeof(client));
			client.sin_family = AF_INET;
			addrClient = inet_addr("192.168.2.142");
			memcpy((char*)&client.sin_addr, &addrClient, sizeof(addrClient));
			client.sin_port = htons(22);

			int socketLength = sizeof(client);
			int clientSocket = accept(socketServer, (struct sockaddr*)&client, &socketLength);

			if (clientSocket == -1)
			{
				printf("clientSocket: %d", clientSocket);
			}
			else
			{
				printf("...\n\n");

				for (int i = 0; i < maxClientNum; i++)
				{
					if (clientSockets[i] == 0)
					{
						clientSockets[i] = clientSocket;
						clientThreads[i] = thread(communicateClient, clientSocket);
						break;
					}
				}
			}
			printClients();
		}
	}
}

int main()
{
	printf("\n");

	// Start Winsock
	WSADATA wsa;
	int wsaStartupResult = WSAStartup(MAKEWORD(2, 0), &wsa);
	printf("wsaStartupResult: %d\n", wsaStartupResult);

	// socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	printf("serverSocket: %d\n", serverSocket);

	// bind
	struct sockaddr_in server;
	unsigned long addrServer;

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	addrServer = inet_addr("192.168.2.215");
	memcpy((char*)&server.sin_addr, &addrServer, sizeof(addrServer));
	server.sin_port = htons(8150);

	int bindResult = bind(serverSocket, (struct sockaddr*)&server, sizeof(server));
	printf("bindResult: %d\n", bindResult);

	// listen
	int listenResult = listen(serverSocket, ANZAHL_8150_CLIENTS);
	printf("listenResult: %d\n\n", listenResult);

	thread acceptThread(acceptConnection, serverSocket);

	while (1)
	{
		
	}

	return 0;
}