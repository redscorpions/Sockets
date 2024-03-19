#define WIN32_LEAN_AND_MEAN // Definizione necessaria per compilare solo le parti essenziali delle intestazioni di Windows

#include <windows.h> // Header contenente le funzionalità del SO
#include <winsock2.h> // Header contenente funzioni e definizioni per l'utilizzo dei socket su Windows
#include <ws2tcpip.h> // Header contenente funzionalità aggiuntive per le comunicazioni TCP/IP
#include <stdlib.h> // Header contente definizioni per funzioni standard
#include <stdio.h> // Header per la gestione standard dell'I/O

// Il linker collegherà automaticamente le seguenti librerie
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512 // Dimensione del buffer predefinito
#define DEFAULT_PORT "27015" // Porta predefinita per la connessione

int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData; // Struttura per memorizzare informazioni sulla libreria di Windows Sockets
    SOCKET ConnectSocket = INVALID_SOCKET; // Socket per la connessione
    struct addrinfo* result = NULL, * ptr = NULL, hints; // Variabili per la risoluzione dell'indirizzo
    const char* sendbuf = "this is a test"; // Dati da inviare al server
    char recvbuf[DEFAULT_BUFLEN]; // Buffer per i dati ricevuti
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN; // Dimensione del buffer di ricezione

    // Verifica dei parametri passati al programma
    if (argc != 2) {
        printf("uso: %s nome-server\n", argv[0]);
        return 1;
    }

    // Inizializzazione di Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup non riuscita con errore: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); // Pulizia della memoria della struttura hints
    hints.ai_family = AF_UNSPEC; // Famiglia di indirizzi non specificata
    hints.ai_socktype = SOCK_STREAM; // Tipo di socket TCP
    hints.ai_protocol = IPPROTO_TCP; // Protocollo TCP

    // Risoluzione dell'indirizzo del server e della porta
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("chiamata a getaddrinfo fallita con errore: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Tentativo di connessione a un indirizzo finché uno non riesce
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Creazione di un socket per la connessione al server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("connessione al socket non riuscita con errore: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connessione al server
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Impossibile connettersi al server!\n");
        WSACleanup();
        return 1;
    }

    // Invia un buffer iniziale al server
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("invio non riuscito con errore: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes inviati: %ld\n", iResult);

    // Chiudi la connessione poiché non verranno inviati altri dati
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("chiusura non riuscita con errore: %d\n", WSAGetLastError());
        closesocket(ConnectSocket); // chiusura del socket
        WSACleanup(); // Pulizia di Winsock
        return 1;
    }

    // Continua a ricevere dati finchè il server non chiude
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) // Il client riceve dei dati
            printf("Bytes ricevuti: %d\n", iResult);
        else if (iResult == 0) // il client non riceve dati
            printf("Connessione chiusa\n");
        else // connessione chiusa a causa di un errore
            printf("recv non riuscita con errore: %d\n", WSAGetLastError());
    } while (iResult > 0);

    // Pulizia
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
