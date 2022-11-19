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
#define SIZE 10
#define BUFFERSIZE 512
#define PROTOPORT 27015 // Numero di porta di default

void ErrorHandler(char *errorMessage) {
	printf("%s",errorMessage);
}

void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	#endif
}

int main(void) {
	
	char ipAddress[SIZE];

	printf("Inserisci l'IP del Server");

	gets(ipAddress);

	fflush(stdin);




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
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(5193); // Server port

	// CONNESSIONE AL SERVER
	if (connect(Csocket, (struct sockaddr *)&sad, sizeof(sad)) < 0)
	{
		ErrorHandler( "Failed to connect.\n" );
		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}
	char* inputString = "prova"; // Stringa da inviare
	int stringLen = strlen(inputString); // Determina la lunghezza

	// INVIARE DATI AL SERVER
	if (send(Csocket, inputString, stringLen, 0) != stringLen) {
		ErrorHandler("send() sent a different number of bytes than expected");
		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}

	// RICEVERE DATI DAL SERVER
	int bytesRcvd;
	int totalBytesRcvd = 0;
	char buf[BUFFERSIZE]; // buffer for data from the server
	printf("Received: "); // Setup to print the echoed string

	while (totalBytesRcvd < stringLen) {
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
		buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
		printf("%s", buf); // Print the echo buffer
	}

	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	ClearWinSock();
	printf("\n"); // Print a final linefeed
	system("pause");
	return(0);
}
