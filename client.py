import socket # libreria per la gestione dei socket
import termcolor # libreria per la gestione dei colori del terminale

def client_start():
    client_socket = socket.socket() # creazione di un nuovo socket

    server = input("Indirizzo del server -> ") # indirizzo del server
    port = 27015 # porta predefinita del server

    client_socket.connect((server, port)) # richiesta di connessione al server

    print(termcolor.colored(('[+] Connessione avvenuta con ' + str(server) + ':' + str(port)), 'green')) # comunicazione di avvenuta connessione

    message = input(' -> ') # inserimento dei dati da inviare al server

    while message.lower().strip() != 'chiudi': # controllo per la chiusura della connessione
        client_socket.send(message.encode()) # invio dei dati al socket

        data = client_socket.recv(1024).decode() # ricezione della risposta
        print('Ricevuto: ', data) # stampa della risposta

        message = input(' -> ') # inserimento dei nuovi dati da inviare

    print(termcolor.colored(('[x] Chiusura connessione'), 'red')) # avviso di chiusura della connessione

    client_socket.close() # chiusura del socket

# la funzione viene chiamata solo se il file viene eseguito come main
if __name__ == '__main__':
    client_start()
