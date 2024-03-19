#undef UNICODE // Disabilita la definizione di UNICODE

#define WIN32_LEAN_AND_MEAN // Riduce il numero di intestazioni di Windows da includere

#include <windows.h> // Include l'intestazione per le funzionalità di Windows
#include <winsock2.h> // Include l'intestazione per i socket di Windows
#include <ws2tcpip.h> // Include l'intestazione per la comunicazione TCP/IP di Windows
#include <stdlib.h> // Include l'intestazione per funzioni di utilità standard
#include <stdio.h> // Include l'intestazione per la gestione input/output standard

// Il linker collegherà automaticamente la libraria Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512 // Dimensione predefinita del buffer
#define DEFAULT_PORT "27015" // Porta predefinita per la connessione

int __cdecl main(void) 
{
    WSADATA wsaData; // Struttura per i dati di inizializzazione di Winsock
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET; // Socket per l'ascolto delle connessioni
    SOCKET ClientSocket = INVALID_SOCKET; // Socket per la comunicazione con il client

    struct addrinfo *result = NULL; // struttura *result
    struct addrinfo hints; // struttura hints

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN]; // Buffer per i dati ricevuti
    int recvbuflen = DEFAULT_BUFLEN; // Dimensione del buffer di ricezione
    
    // Inizializza Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("Inizializzazione non riuscita con errore: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); // Pulizia della memoria della struttura hints
    hints.ai_family = AF_INET; // Famiglia di indirizzi IPv4
    hints.ai_socktype = SOCK_STREAM; // Tipo di socket TCP
    hints.ai_protocol = IPPROTO_TCP; // Protocollo TCP
    hints.ai_flags = AI_PASSIVE; // Ascolto su qualsiasi indirizzo disponibile

    // Risolve l'indirizzo del server e la porta
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("risoluzione dell'indirizzo non riuscita con errore: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Crea un socket per l'ascolto delle connessioni in arrivo
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) { // controllo della corretta creazione del socket
        printf("socket non riuscita con errore: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Configura il socket di ascolto TCP
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind non riuscita con errore: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // Mette il socket in ascolto per le connessioni in arrivo
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accetta una connessione da un client
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Non è più necessario il socket del server
    closesocket(ListenSocket);

    // Ricevi finché il peer non chiude la connessione
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes ricevuti: %d\n", iResult);

            // Rispedisci il buffer al mittente
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send non riuscita con errore: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes inviati: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Chiusura della connessione...\n");
        else  {
            printf("recv non riuscita con errore: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // Chiudi la connessione poiché abbiamo finito
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // Pulizia
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
