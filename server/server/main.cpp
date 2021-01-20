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

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define ANZAHL_8150_CLIENTS 5

void delay(int ms) {
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}

void acceptConnection(int socketServer)
{
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
			printf("select error\n");
		}
		else if (selectResult == 0)
		{
			printf("timeout occurred (20 second) \n"); /* a timeout occurred */
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
			int socketClient = accept(socketServer, (struct sockaddr*)&client, &socketLength);
			printf("socketClient: %d\n", socketClient);
		}
	}

	printf("aaa\n");
}


int main()
{
	// Start Winsock
	WSADATA wsa;
	int wsaStartupResult = WSAStartup(MAKEWORD(2, 0), &wsa);
	printf("wsaStartupResult: %d\n", wsaStartupResult);

	// socket
	int socketServer = socket(AF_INET, SOCK_STREAM, 0);
	printf("socketServer: %d\n", socketServer);

	// bind
	struct sockaddr_in server;
	unsigned long addrServer;

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	addrServer = inet_addr("192.168.2.215");
	memcpy((char*)&server.sin_addr, &addrServer, sizeof(addrServer));
	server.sin_port = htons(8150);

	int bindResult = bind(socketServer, (struct sockaddr*)&server, sizeof(server));
	printf("bindResult: %d\n", bindResult);

	// listen
	int listenResult = listen(socketServer, ANZAHL_8150_CLIENTS);
	printf("listenResult: %d\n", listenResult);

	thread thread1(acceptConnection, socketServer);

	while (1)
	{
		delay(1000);
		printf("bbb\n");
	}

	return 0;
}