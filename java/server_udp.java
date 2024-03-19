import java.net.DatagramPacket; // pacchetto di dati utilizzato nelle comunicazioni di rete tramite il protocollo UDP
import java.net.DatagramSocket; // socket per la comunicazione tramite il protocollo UDP
import java.net.InetAddress; // fornisce funzionalità per rappresentare e manipolare indirizzi IP (ottenere l'IP di "localhost")

// Classe ServerUDP
public class ServerUDP {

	// Metodo main
	public static void main(String[] args) throws Exception {
	// Creazione di un socket UDP per il server sulla porta 9876 e utilizzo del try-with-resources per garantire la chiusura 
	// automatica del socket
        try (DatagramSocket serverSocket = new DatagramSocket(9876)) {
        	
            // Dichiarazione degli array di byte per i dati da ricevere e inviare
            byte[] receiveData = new byte[1024];
            byte[] sendData = new byte[1024];

            // Loop principale del server che continua a ricevere e rispondere a pacchetti UDP
            while (true) {
            	
                // Creazione di un pacchetto Datagram per ricevere dati
                DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                serverSocket.receive(receivePacket); // Ricezione del pacchetto tramite il socket del server

                // Conversione dei dati ricevuti in una stringa
                String sentence = new String(receivePacket.getData());
                System.out.println("Received: " + sentence);

                // Ottenimento dell'indirizzo IP e della porta del mittente
                InetAddress IPAddress = receivePacket.getAddress();
                int port = receivePacket.getPort();

                // Trasformazione della stringa ricevuta in maiuscolo
                String capitalizedSentence = sentence.toUpperCase();
                sendData = capitalizedSentence.getBytes(); // Conversione della stringa maiuscola in array di byte

                // Creazione di un pacchetto Datagram per inviare la risposta al mittente
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                serverSocket.send(sendPacket); // Invio del pacchetto al mittente tramite il socket del server
            }
        }
	}
}
