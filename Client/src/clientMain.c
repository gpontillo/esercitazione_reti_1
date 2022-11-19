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
#define BUFFER_SIZE 512
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
	int Csocket;
	Csocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Csocket < 0) {
		ErrorHandler("socket creation failed.\n");
		closesocket(Csocket);
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
	if (connect(Csocket, (struct sockaddr *)&sad, sizeof(sad)) < 0)
	{
		ErrorHandler( "Failed to connect.\n" );
		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}

	// GESTIONE DELLA CONNESSIONE COL SERVER
	char buf[BUFFER_SIZE]; // buffer for data from the server
	do {
		char* aString = ""; // Stringa A da inviare
		char* bString = ""; // Stringa B da inviare

		printf("Insert first string:");
		scanf("%s", aString);
		printf("Insert second string:");
		scanf("%s", bString);

		int aStringLen = strlen(aString); // Determina la lunghezza della stringa A
		int bStringLen = strlen(bString); // Determina la lunghezza della stringa B

		// INVIO STRINGHE AL SERVER
		// Invio stringa A
		if (send(Csocket, aString, aStringLen, 0) != aStringLen) {
			ErrorHandler("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}

		// Invio stringa B
		if (send(Csocket, bString, bStringLen, 0) != bStringLen) {
			ErrorHandler("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}

		// RICEVO STRINGA DAL SERVER
		int bytesRcvd;
		int totalBytesRcvd = 0;
		printf("Received: "); // Setup to print the echoed string

		if ((bytesRcvd = recv(Csocket, buf, BUFFER_SIZE - 1, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
		buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
		printf("%s", buf); // Print the echo buffer
	}while(!strcmp(buf,"bye"));

	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	ClearWinSock();
	printf("\n"); // Print a final linefeed
	system("pause");
	return(0);
}
