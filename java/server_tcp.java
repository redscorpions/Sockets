import java.io.IOException; // classe che rappresenta un'eccezione di input/output
import java.io.PrintWriter; // fornisce metodi per la scrittura di dati formattati su un flusso di output, come un file o un socket
import java.net.ServerSocket; // classe utilizzata per implementare il lato server di un'applicazione di rete basata su socket
import java.net.Socket; // rappresenta un socket di rete
import java.util.Scanner; // classe utilizzata per leggere input da diverse fonti, come file, stringhe o flussi di input

// Classe ServerTCP
public class ServerTCP {

    // Metodo che avvia il server TCP
    public static void startServer() throws IOException {
        // Apre una porta TCP specifica (1337) per l'ascolto delle connessioni
        ServerSocket serverSocket = new ServerSocket(1337);
        System.out.println("Server socket ready on port: " + 1337);

        // Attende una connessione da un client
        Socket socket = serverSocket.accept();
        System.out.println("Received client connection");

        // Creazione di oggetti Scanner e PrintWriter per leggere e scrivere dati attraverso il socket
        Scanner in = new Scanner(socket.getInputStream());
        PrintWriter out = new PrintWriter(socket.getOutputStream());

        // Loop principale del server che continua a ricevere dati dal client
        while (true) {
            // Lettura di una linea di testo dal client
            String line = in.nextLine();
            
            // Verifica se il client ha inviato "quit" per terminare la connessione
            if (line.equals("quit")) {
                break;
            } else {
                // Invia una risposta al client
                out.println("Received: " + line);
                out.flush(); // Assicura che i dati siano inviati subito
            }
        }

        // Chiusura dei flussi e dei socket
        System.out.println("Closing sockets");
        in.close();
        out.close();
        socket.close();
        serverSocket.close();
    }

    // Metodo main
    public static void main(String[] args) {
        try {
            // Chiamata al metodo startServer() per avviare il server
            startServer();
        } catch (IOException e) {
            // Gestione dell'eccezione in caso di errori di I/O
            e.printStackTrace();
        }
    }
}
