/*
Sketch per la rilevazione della distanza con visualizzazione su LCD
tramite sensore URM37 V3.2 Ultrasonic Sensor DFROBOT.
Pier Calderan - Luglio 2013
*/
#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 10, 6, 5, 4, 2); // attenzione alla piedinatura del display!

int USValue = 0; // variabile per il valore di lettura
uint8_t DMcmd[4] = {0x22, 0x00, 0x00, 0x22}; // comando per la misurazione della distanza

void setup()  {
lcd.begin(16, 2); lcd.setCursor(0,0);
lcd.print("DISTANCE MONITOR");
Serial.begin(9600); // imposta la velocità della seriale a 9600 baud
lcd.setCursor(0,1); lcd.print("Init sensor");
}

void loop() {
for(int i=0;i<4;i++) {
Serial.write(DMcmd[i]); //manda il comando per la misurazione della distanza
}
delay(40); 
unsigned long timer = millis();
while(millis() - timer < 30) {
if(Serial.available()> 0) {
int header=Serial.read(); // lettura del primo byte di intestazione (0x22)
int highbyte=Serial.read(); // lettura del byte "highbyte"
int lowbyte=Serial.read();// lettura del byte "lowbyte"
int sum=Serial.read(); // lettura del byte somma
if(header == 0x22) // se l'intestazione è corretta...
{
if(highbyte==255) {
USValue=65525;  // se highbyte = 255 lettura non valida
}
else
{
USValue = highbyte*255+lowbyte; // lettura valida
}
lcd.setCursor(0,1);
lcd.print("Distanza:       "); // quindi stampa sul display...
lcd.setCursor(10,1);
lcd.print(USValue);  // il valore della distanza
}
else {
while(Serial.available())  byte bufferClear = Serial.read(); // lettura fittizia
break;
} } } delay(20); }

