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
#include <string.h>
#define SIZE 10
#define BUFFER_SIZE 1024
#define DEFAULT_PORT 5555 // Numero di porta di default

void ErrorHandler(char *errorMessage) {
	printf("%s",errorMessage);
}

void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	#endif
}

int main(void) {

	#if defined WIN32

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
		return -1;
	}
	#endif
	
	// CREAZIONE DELLA SOCKET
	int clientSocket;
	clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket < 0) {
		ErrorHandler("socket creation failed.\n");
		closesocket(clientSocket);
		ClearWinSock();
		return -1;
	}

	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	int port = DEFAULT_PORT;
	printf("Insert the port number of the server: ");
	scanf("%d", &port);

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(port); // Server port

	// CONNESSIONE AL SERVER
	if (connect(clientSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0)
	{
		ErrorHandler( "Failed to connect.\n" );
		closesocket(clientSocket);
		ClearWinSock();
		return -1;
	}

	// GESTIONE DELLA CONNESSIONE COL SERVER

	// Ricevo stringa "connnessione avvenuta"
	char buffer[BUFFER_SIZE]; // buffer for data from the server
	int bytesRicev = 0;
	if ((bytesRicev = recv(clientSocket, buffer, BUFFER_SIZE, 0)) <= 0) {
		ErrorHandler("recv() failed or connection closed prematurely");
		closesocket(clientSocket);
		ClearWinSock();
		return -1;
	}
	printf("%s\n", buffer); // Print the echo buffer

	//Ciclo do-while fin quando non riceviamo la stringa bye
	do {
		char aString[BUFFER_SIZE] = ""; // Stringa A da inviare
		char bString[BUFFER_SIZE] = ""; // Stringa B da inviare

		printf("Insert first string:");
		scanf("%s", &aString);
		printf("Insert second string:");
		scanf("%s", &bString);

		// INVIO STRINGHE AL SERVER
		// Invio stringa A
		strcpy(buffer, aString);
		if (send(clientSocket, buffer, BUFFER_SIZE, 0) <= 0) {
			ErrorHandler("send() sent a different number of bytes than expected");
			closesocket(clientSocket);
			ClearWinSock();
			return -1;
		}

		// Invio stringa B
		strcpy(buffer, bString);
		if (send(clientSocket, buffer, BUFFER_SIZE, 0) <= 0) {
			ErrorHandler("send() sent a different number of bytes than expected");
			closesocket(clientSocket);
			ClearWinSock();
			return -1;
		}

		// RICEVO STRINGA DAL SERVER

		int bytesRcvd;
		printf("Received: "); // Setup to print the echoed string

		if ((bytesRcvd = recv(clientSocket, buffer, BUFFER_SIZE, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely");
			closesocket(clientSocket);
			ClearWinSock();
			return -1;
		}
		printf("%s\n", buffer); // Print the echo buffer
		printf("---------------\n");
	}while(strcmp(buffer,"bye") != 0);

	// CHIUSURA DELLA CONNESSIONE
	closesocket(clientSocket);
	ClearWinSock();
	printf("\n"); // Print a final linefeed
	system("pause");
	return(0);
}
