import java.io.IOException; // classe che rappresenta un'eccezione di input/output
import java.io.PrintWriter; // fornisce metodi per la scrittura di dati formattati su un flusso di output, come un file o un socket
import java.net.Socket; // rappresenta un socket di rete
import java.util.NoSuchElementException; // classe che rappresenta un'eccezione quando si tenta di accedere a un elemento inesistente
import java.util.Scanner; // classe utilizzata per leggere input da diverse fonti, come file, stringhe o flussi di input

// Classe ClientTCP
public class ClientTCP {

    // Metodo che avvia il client TCP
    public static void startClient() throws IOException {
        // Creazione di un socket e connessione al server all'indirizzo IP "127.0.0.1" e porta 1337
        Socket socket = new Socket("127.0.0.1", 1337);
        System.out.println("Connection established");

        // Creazione di oggetti Scanner per leggere dall'input stream del socket e per leggere da stdin
        Scanner socketIn = new Scanner(socket.getInputStream());
        PrintWriter socketOut = new PrintWriter(socket.getOutputStream());
        Scanner stdin = new Scanner(System.in);

        try {
            // Loop principale del client in attesa di input
            while (true) {
                // Lettura di una linea di input da stdin
                String inputLine = stdin.nextLine();
                
                // Invio della linea di input al server attraverso il socket
                socketOut.println(inputLine);
                socketOut.flush(); // Assicura che i dati siano inviati subito
                
                // Lettura della risposta dal server attraverso il socket
                String socketLine = socketIn.nextLine();
                System.out.println(socketLine);
            }
        } catch (NoSuchElementException e) {
            // Gestione dell'eccezione quando la connessione viene chiusa
            System.out.println("Connection closed");
        } finally {
            // Chiusura delle risorse (Scanner, PrintWriter e Socket)
            stdin.close();
            socketIn.close();
            socketOut.close();
            socket.close();
        }
    }

    // Metodo main
    public static void main(String[] args) {
        try {
            // Chiamata al metodo startClient() per avviare il client
            startClient();
        } catch (IOException e) {
            // Gestione dell'eccezione in caso di errori di I/O
            e.printStackTrace();
        }
    }
}
