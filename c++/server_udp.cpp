#include <iostream> // Inclusione della libreria standard per la gestione dell'I/O 
#include <WinSock2.h> // Inclusione della libreria per le funzioni di rete di Windows (gestione socket)

#pragma comment(lib, "ws2_32.lib") // Aggiunta del collegamento alla libreria ws2_32.lib eseguito dal linker

using namespace std; // Utilizzo dello spazio dei nomi standard

int main() {
    WSADATA wsaData; // Struttura per i dati di inizializzazione di Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Inizializzazione di Winsock
        cerr << "Errore durante l'inizializzazione di Winsock" << endl; // Messaggio d'errore se l'inizializzazione di Winsock fallisce
        return 1; // Uscita dal programma con codice di errore
    }

    SOCKET serverSocket; // Inizializzazione variabile socket del server
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Creazione del socket UDP
    if (serverSocket == INVALID_SOCKET) { // Controllo per vedere se la creazione del socket ha avuto successo
        cerr << "Errore durante la creazione del socket" << endl; // Stampa di un messaggio di errore
        WSACleanup(); // Pulizia di Winsock
        return 1; // Uscita dal programma con codice di errore
    }

    sockaddr_in serverAddr; // Inizializzazione variabile della struttura per l'indirizzo del server
    serverAddr.sin_family = AF_INET; // Famiglia di indirizzi: IPv4
    serverAddr.sin_port = htons(12345); // Porta del server
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accetta connessioni da qualsiasi interfaccia

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) { // Associazione dell'indirizzo al socket
        cerr << "Errore durante l'associazione dell'indirizzo al socket" << endl; // Messaggio d'errore se l'associazione fallisce
        closesocket(serverSocket); // Chiusura del socket
        WSACleanup(); // Pulizia di Winsock
        return 1; // Uscita dal programma con codice di errore
    }

    cout << "Server UDP in ascolto sulla porta 12345..." << endl; // Stampa un messaggio di conferma di avvenuta associazione

    char buffer[1024]; // Inizializzazione Buffer per i dati ricevuti
    sockaddr_in clientAddr; // Inizializzazione Struttura per l'indirizzo del client
    int clientAddrSize = sizeof(clientAddr); // Dimensione dell'indirizzo del client
    int bytesReceived; // Inizializzazione variabile per il numero di byte ricevuti

    while (true) { // Ciclo di ricezione dati dai client
        bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrSize); // Ricezione dei dati dai client
        if (bytesReceived == SOCKET_ERROR) { // Controllo per vedere se la ricezione ha avuto successo
            cerr << "Errore durante la ricezione dei dati dal client" << endl; // Messaggio d'errore se la ricezione fallisce
            closesocket(serverSocket); // Chiusura del socket
            WSACleanup(); // Pulizia di Winsock
            return 1; // Uscita dal programma con codice di errore
        }

        buffer[bytesReceived] = '\0'; // Aggiunta del terminatore di stringa ai dati ricevuti
        cout << "Dati ricevuti dal client: " << buffer << endl; // Stampa sul server dei dati ricevuti

        const char* response = "Messaggio ricevuto correttamente!"; // Messaggio di risposta al client
        sendto(serverSocket, response, strlen(response), 0, (sockaddr*)&clientAddr, sizeof(clientAddr)); // Invio della risposta al client
    }

    closesocket(serverSocket); // Chiusura del socket del server
    WSACleanup(); // Pulizia di Winsock

    return 0; // Uscita dal programma con codice di successo
}
