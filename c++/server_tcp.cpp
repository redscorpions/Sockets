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

    // Associazione del socket alla porta dell'indirizzo specificato
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) { // controllo di avvenuta associazione
        printf("associazione non riuscita con errore: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket); // chiusura del socket
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // Mette il socket in ascolto per accettare le connessioni in arrivo
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) { // controllo se il socket è in ascolto
        printf("listen non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accettazione di una connessione da un client
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) { // controllo dell'avvenuta accettazione
        printf("accettazione non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ListenSocket); // chiusura del socket
        WSACleanup();
        return 1;
    }

    // Essendo una configurazione di socket a host singolo non è più necessario ascoltare richieste di connessione da altri client
    closesocket(ListenSocket);

    // Ricezione continua dei pacchetti finchè il client non si disconnette
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) { // avvenuta ricezione
            printf("Bytes ricevuti: %d\n", iResult);

            // Rispedisce il buffer di dati al mittente
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) { // controllo di avvenuto invio
                printf("invio non riuscito con errore: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Byte inviati: %d\n", iSendResult);
        }
        else if (iResult == 0) // nulla viene ricevuto, il client si è disconnesso
            printf("Chiusura della connessione...\n");
        else  { // nulla viene ricevuto a causa di un errore del socket
            printf("ricezione non avvenuta con errore: %d\n", WSAGetLastError());
            closesocket(ClientSocket); // chiusura del socket
            WSACleanup();
            return 1;
        }

    } while (iResult > 0); // continua finchè vengono ricevuti dati

    // Chiusura della connessione
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) { // controllo di avvenuta terminazione del processo send (invio)
        printf("chiusura non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ClientSocket); // chiusura del socket
        WSACleanup();
        return 1;
    }

    closesocket(ClientSocket); // chiusura del socket
    WSACleanup();

    return 0;
}
