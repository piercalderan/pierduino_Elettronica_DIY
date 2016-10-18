/*
Sketch per il controllo della luminosità di un LED
con visualizzazione sul display dei valori PWM.
Pier Calderan - Luglio 2013
*/

#include <LiquidCrystal.h> // importa la libreria LiquidCrystal
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // crea l'oggetto lcd

int sensorPin = A0;  // variabile assegnata all'ingresso analogico A0
int ledPin = 9;      // variabile assegnata alla porta digitale 9 (PWM)

int sensorValue = 0; // variabile assegnata al valore del sensore
int mapOutput = 0;   // variabile per la mappatura dell'ingresso analogico

void setup() 
{
lcd.begin(16, 2); // inizializza il display per 2 linee e 16 caratteri
lcd.print("Valore PWM"); // stampa la scritta Valore PWM sulla prima riga

pinMode(ledPin, OUTPUT); // modalità OUTPUT (uscita) della porta digitale
}

void loop() {
sensorValue = analogRead(sensorPin);   // lettura del valore del sensore A0

mapOutput = map(sensorValue,0,1023,0,255); // mappatura dei valori 0-1023 a 0-255
analogWrite(ledPin, mapOutput); // scrittura dei valori da 0 a 255 sulla porta PWM

lcd.setCursor(0, 1); // imposta il cursore sulla seconda riga
lcd.print(mapOutput); // scrive il dato PWM
}
