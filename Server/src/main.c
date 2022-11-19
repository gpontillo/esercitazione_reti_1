/*
 ============================================================================
 Name        : server.c
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

#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // size of request queue

void errorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
}

void clearWinSock() {
	#if defined WIN32
	WSACleanup();
	#endif
}

void closeConnection(int mySocket) {
	// CHIUSURA DELLA CONNESSIONE
	closesocket(mySocket);
	clearWinSock();
}

int main(int argc, char *argv[]) {

	int port;
	if (argc > 1) {
		port = atoi(argv[1]); // if argument specified convert argument to binary
	}
	else
		port = PROTOPORT; // use default port number
	if (port < 0) {
		printf("bad port number %s \n", argv[1]);
		return 0;
	}
	#if defined WIN32 // initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		errorHandler("Error at WSAStartup()\n");
		return 0;
	}
	#endif
	
	// CREAZIONE DELLA SOCKET
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySocket < 0) {
		errorHandler("socket creation failed.\n");
		clearWinSock();
		return -1;
	}

	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port);
	/*
	 * converts values between the host and
	 * network byte order. Specifically, htons() converts 16-bit quantities
	 * from host byte order to network byte order.
	 */
	if (bind(MySocket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorHandler("bind() failed.\n");
		closesocket(MySocket);
		clearWinSock();
		return -1;
	}

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen (MySocket, QLEN) < 0) {
		errorHandler("listen() failed.\n");
		closesocket(MySocket);
		clearWinSock();
		return -1;
	}

	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int clientSocket; // socket descriptor for the client
	int clientLen; // the size of the client address
	printf("Waiting for a client to connect...");
	while (1) { /* oppure for (;;) */
		clientLen = sizeof(cad); // set the size of the client address
		if ((clientSocket = accept(MySocket, (struct sockaddr *)&cad,&clientLen)) < 0) {
			errorHandler("accept() failed.\n");
			closeConnection(MySocket);
			return 0;
		}
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

		//roba da fare
	}
	return 0;
}
