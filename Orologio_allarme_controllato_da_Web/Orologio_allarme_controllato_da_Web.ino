/*
Sketch per la visualizzazione su LCD dell'orario da un server Internet e funzioni di allarme.
Pier Calderan - Luglio 2013
*/
// controllo della lista di server NTP http://tf.nist.gov/tf-cgi/servers.cgi
#include <Time.h>
#include <TimeAlarms.h>

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

unsigned long DEFAULT_TIME = 0; // 
 
int ledState = LOW;

EthernetUDP Udp; // crea un'istanza UDP

void setup() 
{
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
Alarm.alarmRepeat(8,30,0, Allarme_1);  // imposta l'allarme 1 ogni giorno alle 8:30
Alarm.alarmRepeat(17,0,0, Allarme_2); // imposta l'allarme 2 ogni giorno alle 17
Alarm.timerRepeat(30, Timer_ON_OFF);  // imposta il timer ogni 30 secondi   
Alarm.timerOnce(15, Una_sola_volta); // imposta il timer una sola volta dopo 15 secondi

lcd.begin(16, 2); // inizializza il display
lcd.setCursor(0,0); lcd.print("ALARM");

if (Ethernet.begin(mac) == 0) { // controllo della connessione
lcd.setCursor(0,1); lcd.print("Non connesso!   ");
  for(;;);
}
  Udp.begin(localPort); // se la connessione al server ha successo, apre la porta UDP

sendNTP();
}

void loop()
{
  Digital_Clock();
  Alarm.delay(1000); // wait one second between clock display
}

void sendNTP() {
sendNTPpacket(timeServer); // manda un pacchetto al server NTP
delay(5000);  // attesa di una risposta

int ore, minuti, secondi, giorno, mese, anno;

if (Udp.parsePacket()) {

Udp.read(packetBuffer,NTP_PACKET_SIZE);
unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
unsigned long secondi_dal_1900 = highWord << 16 | lowWord;  
const unsigned long settanta_anni = 2208988800UL;    
unsigned long epoch = secondi_dal_1900 - settanta_anni;  

setTime(epoch); // imposta il tempo UNIX
delay(5000);
}
}

unsigned long sendNTPpacket(IPAddress& address) {
memset(packetBuffer, 0, NTP_PACKET_SIZE);
packetBuffer[0] = 0b11100011;   // LI, Version, Mode
packetBuffer[1] = 0;     // Stratum, or type of clock
packetBuffer[2] = 6;     // Polling Interval
packetBuffer[3] = 0xEC;  // Peer Clock Precision
packetBuffer[12]  = 49;
packetBuffer[13]  = 0x4E;
packetBuffer[14]  = 49;
packetBuffer[15]  = 52;
Udp.beginPacket(address, 123); //NTP requests are to port 123
Udp.write(packetBuffer,NTP_PACKET_SIZE);
Udp.endPacket();
}

// Funzioni
void Allarme_1()
{
// attiva una porta digitale
}

void Allarme_2(){
// attiva una porta digitale
}

void Allarme_3(){
// attiva una porta digitale
}
void Timer_ON_OFF(){
    if (ledState == LOW) 
    {
ledState = HIGH;
lcd.setCursor(8,1);
lcd.print("LED ON ");}
    else 
     {
ledState = LOW;
digitalWrite(9, ledState);
lcd.setCursor(8,1);
lcd.print("LED OFF");}
}

void Una_sola_volta(){
digitalWrite(8, HIGH);
}

void Digital_Clock()
{
lcd.setCursor(0,1);
lcd.print(hour()+2); // ora locale italiana + ora legale
Digits(minute()); // chiamata alla funzione per lo zero davanti ai minuti
Digits(second()); // chiamata alla funzione per lo zero davanti ai secondi
lcd.setCursor(6,0);
lcd.print(day());
lcd.print("/");
lcd.print(month());
lcd.print("/");
lcd.print(year());
}

void Digits(int digits)
{
lcd.print(":");
if(digits < 10) lcd.print('0');
lcd.print(digits);
}



