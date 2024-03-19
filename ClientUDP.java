import java.net.DatagramPacket; // pacchetto di dati utilizzato nelle comunicazioni di rete tramite il protocollo UDP
import java.net.DatagramSocket; // socket per la comunicazione tramite il protocollo UDP
import java.net.InetAddress; // fornisce funzionalità per rappresentare e manipolare indirizzi IP (ottenere l'IP di "localhost")

// Classe ClientUDP
public class ClientUDP {

	// Metodo main
    public static void main(String[] args) throws Exception {
        // Creazione di un socket UDP per il client
        DatagramSocket clientSocket = new DatagramSocket();

        // Ottenimento dell'indirizzo IP per "localhost" e assegnazione alla variabile IPAddress
        InetAddress IPAddress = InetAddress.getByName("localhost");

        // Assegnazione del numero di porta alla variabile port
        int port = 9876;

        // Creazione di array di byte per i dati da inviare e ricevere
        byte[] sendData = new byte[1024];
        byte[] receiveData = new byte[1024];

        // Creazione di una stringa da inviare al server
        String sentence = "Ciao, Server UDP!";
        sendData = sentence.getBytes(); // Conversione della stringa in array di byte

        // Creazione di un pacchetto Datagram da inviare al server
        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);

        // Invio del pacchetto al server tramite il socket
        clientSocket.send(sendPacket);

        // Creazione di un pacchetto Datagram per ricevere i dati dal server
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

        // Invio del pacchetto vuoto al server (errore: dovrebbe essere una ricezione)
        clientSocket.send(receivePacket);

        // Conversione dei dati ricevuti in una stringa
        String modifiedSentence = new String(receivePacket.getData());

        // Stampa della stringa ricevuta dal server
        System.out.println("Received from server: " + modifiedSentence);

        // Chiusura del socket del client
        clientSocket.close();
    }
}
