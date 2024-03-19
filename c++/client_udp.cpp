#include <iostream> // Inclusione della libreria standard per la gestione dell'I/O 
#include <WinSock2.h> // Inclusione della libreria per le funzioni di rete di Windows (socket)
#include <ws2tcpip.h> // Inclusione della libreria per le funzioni di trasmissione dei pacchetti di Windows

#pragma comment(lib, "ws2_32.lib") // Aggiunta del collegamento alla libreria ws2_32.lib eseguito linker

using namespace std; // Utilizzo dello spazio dei nomi standard

int main() {
    WSADATA wsaData; // Struttura per i dati di inizializzazione di Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Inizializzazione di Winsock
        cerr << "Errore durante l'inizializzazione di Winsock" << endl; // Messaggio d'errore se l'inizializzazione di Winsock fallisce
        return 1; // Uscita dal programma con codice di errore
    }

    SOCKET clientSocket; // Inizializzazione variabile socket del server
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Creazione del socket UDP
    if (clientSocket == INVALID_SOCKET) { // Controllo per vedere se la creazione del socket ha avuto successo
        cerr << "Errore durante la creazione del socket" << endl; // Messaggio d'errore se la creazione del socket fallisce
        WSACleanup(); // Pulizia di Winsock
        return 1; // Uscita dal programma con codice di errore
    }

    sockaddr_in serverAddr; // Inizializzazione variabile della struttura per l'indirizzo del server
    serverAddr.sin_family = AF_INET; // Famiglia di indirizzi: IPv4
    serverAddr.sin_port = htons(12345); // Porta del server
    inet_pton(AF_INET, "127.0.0.5", &serverAddr.sin_addr); // Conversione dell'indirizzo IP del server

    const char* message = "Hello, server!"; // Messaggio da inviare al server

    // Invio dei dati al server
    int bytesSent = sendto(clientSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bytesSent == SOCKET_ERROR) { // Controllo del corretto invio dei dati
        cerr << "Errore durante l'invio dei dati al server" << endl; // Messaggio d'errore se l'invio dei dati al server fallisce
        closesocket(clientSocket); // Chiusura del socket
        WSACleanup(); // Pulizia di Winsock
        return 1; // Uscita dal programma con codice di errore
    }

    cout << "Dati inviati al server: " << message << endl; // Stampa dei dati inviati al server

    char buffer[1024]; // Inizializzazione Buffer per la ricezione dei dati
    sockaddr_in serverResponseAddr; // Inizializzazione Struttura per l'indirizzo di risposta del server
    int serverResponseAddrSize = sizeof(serverResponseAddr); // Dimensione dell'indirizzo di risposta del server

    // Ricezione della risposta dal server
    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverResponseAddr, &serverResponseAddrSize);
    if (bytesReceived == SOCKET_ERROR) { // Controllo della corretta ricezione
        cerr << "Errore durante la ricezione della risposta dal server" << endl; // Stampa di un messaggio di errore
        closesocket(clientSocket); // Chiusura del socket
        WSACleanup(); // Pulizia di Winsock
        return 1; // Uscita dal programma con codice di errore
    }

    buffer[bytesReceived] = '\0'; // Aggiunta del terminatore di stringa al buffer dei dati ricevuti
    cout << "Risposta dal server: " << buffer << endl; // Stampa della risposta (dati) ricevuta dal server

    closesocket(clientSocket); // Chiusura del socket del client
    WSACleanup(); // Pulizia di Winsock

    return 0; // Uscita dal programma con codice di successo
}
