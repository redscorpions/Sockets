import socket # libreria per la gestione dei socket
import termcolor # libreria per la gestione dei colori del terminale

def server_start():
    port = 27015 # porta predefinita per la connessione
    
    while(true):
        # Scegliere se inizializzare una connessione basata su protocollo UDP o TCP
        print('Seleziona opzione:\n1) Avvia il server in loopback\n2) Avvia il server in rete locale')
        opzione = int(input(' -> ')) # scelta
        while(opzione != 1 or opzione != 2): # loop per assicurarsi che la scelta sia valida
            opzione = int(input(' -> '))

        if opzione == 1:
            host = socket.gethostbyname("localhost") # ottenimento dell'indirizzo IP di loopback (localhost)
            break
        elif opzione == 2:
            host = socket.gethostbyname(socket.gethostname()) # ottenimento dell'indirizzo IP su rete locale
            break
    
    server_socket = socket.socket() # creazione del nuovo socket

    server_socket.bind((host, port)) # associazione dell'indirizzo IP e della porta al socket

    server_socket.listen(1) # limitazione ad un solo client che può connettersi

    print(termcolor.colored(("[+] Server avviato sull'indirizzo IP: " + host + ':' + str(port)), 'green')) # comunicazione di avvenuto avviamento del server socket

    conn, address = server_socket.accept() # accettazione delle connessioni
    print(termcolor.colored(("[+] Connessione da: " + str(address[0]) + ':' + str(address[1])), 'green')) # visualizzazione delle informazioni del client connesso

    # rimane in ascolto
    while(True):
        # riceve dati con dimensione massima del buffer impostata a 1024 byte
        data = conn.recv(1024).decode()

        # interrompe il loop se non vengono più ricevuti dati
        if not data:
            break

        print("Ricevuto: " + str(data)) # stampa del messaggio ricevuto

        data = input(' -> ') # richiesta di una risposta da inserire sul server
        
        conn.send(data.encode()) # invio della risposta al Client
    
    print(termcolor.colored(('[x] Chiusura connessione'), 'red')) # avviso di chiusura connessione

    conn.close() # chiusura della connessione

# la funzione viene chiamata solo se il file viene eseguito come main
if __name__ == '__main__':
    server_start()
