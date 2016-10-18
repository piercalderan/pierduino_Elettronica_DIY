/*
Sketch per la visualizzazione su client Internet (browser) dei dati di un sensore di temperatura.
Pier Calderan - Luglio 2013
*/
#include <Ethernet.h> // libreria Ethernet
#include <SPI.h> // libreria SPI

// indirizzo MAC (quello dello shield Ethernet)
byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x78, 0x13 };

// assegnazione di un indirizzo IP:
IPAddress ip(192,168,1,20); // indirizzo locale della scheda
IPAddress gateway(192,168,1,1);	// indirizzo locale del gateway
IPAddress subnet(255, 255, 255, 0); // Subnet Mask

EthernetServer server(80); // porta 80 di default per la connessione HTTP
int sensorPin = 0; float temperature = 0.0; float temperatureF = 0.0; // variabili del sensore

void setup() {
Ethernet.begin(mac, ip); // inizializzazione della LAN
server.begin(); // inizializzazione del server
pinMode(10, OUTPUT); // Chip Select della card SD come OUTPUT
delay(1000); }

void loop() { 
//funzione di lettura del sensore e conversione in gradi centigradi
int sensorValue = analogRead(sensorPin); 
float voltage = sensorValue * 5.0;  voltage /= 1024.0;
temperature = voltage * 100 ;
temperatureF = (temperature * 9.0 / 5.0) + 32.0;  // conversione in gradi Fahrenheit
delay(250); // ritardo di 250 millisecondi

listenForEthernetClients(); // funzione di ascolto del client (browser)
}

void listenForEthernetClients() {
EthernetClient client = server.available();
if (client) {
boolean currentLineIsBlank = true;
while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Refresh: 5");  // refresh ogni 5 sec
          client.println();
          client.print("Temperatura in gradi centigradi: ");
          client.print(temperature);
          client.print("<br>Temperatura in gradi Fahrenheit: ");
          client.print(temperatureF);
          break;         }
        if (c == '\n') {
          currentLineIsBlank = true;         } 
        else if (c != '\r') {
          currentLineIsBlank = false;        }
      }
    }
    delay(1); // breve ritardo di 1 ms
    client.stop(); // chiusura della connessione
  }
}

