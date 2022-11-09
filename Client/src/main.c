/*
 ============================================================================
 Name        : client.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	
	#if defined WIN32
	// Initialize Winsock
	WSADATA wsaData;
	int iResult =
	WSAStartup(MAKEWORD(2,2),
	&wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
	#endif
	int Mysocket;
	
	closesocket(Mysocket);
	
	#if defined WIN32
	WSACleanup();
	#endif
	
	return 0;
}
