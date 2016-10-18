/*
Sketch per il controllo della luminosità di un LED 
collegato alla porta 11 (PWM) tramite un potenziometro
collegato alla porta analogica A0.
Pier Calderan - Luglio 2013
*/

int sensorPin = A0;  // variabile assegnata all'ingresso analogico A0
int ledPin = 11;     // variabile assegnata alla porta digitale 11 (PWM)

int sensorValue = 0; // variabile assegnata al valore del sensore
int mapOutput = 0;   // variabile per la mappatura dell'ingresso analogico

void setup() 
{
pinMode(ledPin, OUTPUT); // inizializza il LED 1 come output
}

void loop() {
sensorValue = analogRead(sensorPin);   // lettura del valore del sensore sulla porta Analog 0

//*************************************************************
mapOutput = map(sensorValue,0,1023,0,255); // mappatura dei valori in ingresso 0-1023 a valori di uscita 0-255
//*************************************************************

analogWrite(ledPin, mapOutput); // scrittura dei valori da 0 a 255 sulla porta 11 (PWM) 
}

