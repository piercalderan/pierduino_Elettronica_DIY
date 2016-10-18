/*
Sketch per la visualizzazione su LCD dell'orario da un server Internet.
Pier Calderan - Luglio 2013
*/
// controllo della lista di server NTP http://tf.nist.gov/tf-cgi/servers.cgi
#include <SPI.h>  // libreria SPI      
#include <Ethernet.h>	// libreria Ethernet
#include <EthernetUdp.h> // libreria UDP
#include <LiquidCrystal.h> // libreria display LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // l'assegnazione è diversa da quella standard!
byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x78, 0x13}; // è importante cambiare il MAC!
unsigned int localPort = 8888;     // porta locale per l'ascolto dei pacchetti UDP
IPAddress timeServer(132, 163, 4, 101); // controllare che il server sia attivo!

const int NTP_PACKET_SIZE= 48; // pacchetto NTP dei primi 48 byte del messaggio
byte packetBuffer[NTP_PACKET_SIZE]; // buffer per i pacchetti in entrata e in uscita

EthernetUDP Udp; // crea un'istanza UDP

void setup() {
lcd.begin(16, 2); // inizializza il display
lcd.setCursor(0,0); lcd.print("OROLOGIO UTC");

if (Ethernet.begin(mac) == 0) { // controllo della connessione
lcd.setCursor(0,1); lcd.print("Non connesso!   ");
  for(;;);
}
  Udp.begin(localPort); // se la connessione al server ha successo, apre la porta UDP
}

void loop()
{
sendNTPpacket(timeServer); // manda un pacchetto al server NTP
delay(5000);  // attesa di una risposta
if ( Udp.parsePacket() ) {  // se si riceve una risposta...
Udp.read(packetBuffer,NTP_PACKET_SIZE);  // viene letto il pacchetto nel buffer

// Il timestamp inizia al byte 40 byte del pacchetto ricevuto ed è di quattro byte,
unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  

// si uniscono i quattro byte in un lungo intero ottenendo lo scarto in secondi dal 1° gennaio 1900):
unsigned long secondi_dal_1900 = highWord << 16 | lowWord;  

// CONVERSIONE DEL TEMPO NELL'ORARIO ATTUALE
// Il tempo Unix inizia dal 1° gennaio 1970 equivalente a 2208988800 secondi
const unsigned long settanta_anni = 2208988800UL;    

// si sottraggono 70 anni per ottenere epoch (epoca)
unsigned long epoch = secondi_dal_1900 - settanta_anni;  

// stampa dell'ora:
// lcd.setCursor(0,1); lcd.print((epoch  % 86400L) / 3600); // un giorno = 86400 secondi
// volendo aggiornare all'ora locale è necessario aggiungere una o due ore
lcd.setCursor(0,1); lcd.print(((epoch  % 86400L) / 3600)+2);
lcd.print(":"); // stampa i due punti :

if (((epoch % 3600) / 60) < 10 ) {
lcd.setCursor(3,1); lcd.print("0"); // se i minuti sono inferiori a 10 stampa "0" davanti
}
lcd.print((epoch  % 3600) / 60); // stampa i minuti (un'ora = 3600 secondi)
lcd.print(":"); // stampa i due punti :

if ((epoch % 60) < 10 ) // se i secondi sono inferiori a 10 stampa "0" davanti
{ lcd.setCursor(6,1); lcd.print("0"); }
lcd.print(epoch  % 60); // stampa i secondi (un minuto = 60 secondi) 
}
delay(10000); // ritardo di 10 secondi per una nuova richiesta al server 
}


// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
// set all bytes in the buffer to 0
memset(packetBuffer, 0, NTP_PACKET_SIZE);
// Initialize values needed to form NTP request
// (see URL above for details on the packets)
packetBuffer[0] = 0b11100011;   // LI, Version, Mode
packetBuffer[1] = 0;     // Stratum, or type of clock
packetBuffer[2] = 6;     // Polling Interval
packetBuffer[3] = 0xEC;  // Peer Clock Precision
// 8 bytes of zero for Root Delay & Root Dispersion
packetBuffer[12]  = 49;
packetBuffer[13]  = 0x4E;
packetBuffer[14]  = 49;
packetBuffer[15]  = 52;

// all NTP fields have been given values, now
// you can send a packet requesting a timestamp:         
Udp.beginPacket(address, 123); //NTP requests are to port 123
Udp.write(packetBuffer,NTP_PACKET_SIZE);
Udp.endPacket();
}
