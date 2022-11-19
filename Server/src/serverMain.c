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
#include <stdbool.h>

#define PORT 5555 // default port
#define CLIENT_NUMBERS 6 // numbers of acceptable clients
#define BUFFERSIZE 1024

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

int main() {



	#if defined WIN32 // initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		errorHandler("Error at WSAStartup()\n");
		return 0;
	}
	#endif
	
	// CREAZIONE DELLA SOCKET
	int serverSocket;

	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket < 0) {
		errorHandler("socket creation failed.\n");
		clearWinSock();
		return -1;
	}


	// ASSEGNAZIONE INDIRIZZO PER LA SOCKET
	struct sockaddr_in socketAddress;

	memset(&socketAddress, 0, sizeof(socketAddress)); // ensures that extra bytes contain 0

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	socketAddress.sin_port = htons(PORT);
	/*
	 * converts values between the host and
	 * network byte order. Specifically, htons() converts 16-bit quantities
	 * from host byte order to network byte order.
	 */
	if (bind(serverSocket, (struct sockaddr*) &socketAddress, sizeof(socketAddress)) < 0) {
		errorHandler("bind() failed.\n");
		closesocket(serverSocket);
		clearWinSock();
		return -1;
	}

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO

	if (listen (serverSocket, CLIENT_NUMBERS) < 0) {
		errorHandler("listen() failed.\n");
		closesocket(serverSocket);
		clearWinSock();
		return -1;
	}

	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in clientAddress; // structure for the client address
	int clientSocket; // socket descriptor for the client
	int clientLen; // the size of the client address

	printf("Waiting for a client to connect...");

	while (true) {

		clientLen = sizeof(clientAddress); // set the size of the client address

		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,&clientLen);

		if (clientSocket < 0) {
			errorHandler("accept() failed.\n");
			closeConnection(serverSocket);
			return 0;
		}


	printf("\n\nConnection established\n\n");
	//VISUALIZZAZIONE IP DEL CLIENT
	printf("Handling client with IP: %s\n\n", inet_ntoa(clientAddress.sin_addr));

	//RICEZIONE DELLA STRINGA

	char stringaInvio[20] = "Connessione avvenuta";


	//int invio;

	send(serverSocket, stringaInvio, BUFFERSIZE, 0);



	/*char stringa[BUFFERSIZE] = " ";

	int ricevi;

	ricevi = recv(serverSocket, stringa, BUFFERSIZE, 0);

	printf("%s", stringa);*/



	}
	return 0;
}
