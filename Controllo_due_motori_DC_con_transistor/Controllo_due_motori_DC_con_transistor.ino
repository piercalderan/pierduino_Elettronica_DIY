/*
Sketch per il Controllo di velocità di due motori DC tramite potenziometri e transistor.
1x motore DC collegato alla porta 11
1x motore DC collegato alla porta 6
1x potenziometro colelgato alla porta A0
1x potenziometro colelgato alla porta A1
Pier Calderan - Luglio 2013
*/

int sensorPin1 = A0;  // variabile assegnata all'ingresso analogico A0
int motorPin1 = 11;     // variabile assegnata alla porta digitale 11 (PWM)
int sensorValue1 = 0; // variabile assegnata al valore del sensore A0
int mapOutput1 = 0;   // variabile per la mappatura dell'ingresso analogico A0

int sensorPin2 = A1;  // variabile assegnata all'ingresso analogico A1
int motorPin2 = 6;     // variabile assegnata alla porta digitale 6 (PWM)
int sensorValue2 = 0; // variabile assegnata al valore del sensore A1
int mapOutput2 = 0;   // variabile per la mappatura dell'ingresso analogico A1

void setup() 
{
pinMode(motorPin1, OUTPUT); // inizializza il motore 1 come output
pinMode(motorPin2, OUTPUT); // inizializza il motore 2 come output
}

void loop() {
sensorValue1 = analogRead(sensorPin1);   // lettura del valore del sensore sulla porta Analog 0
sensorValue2 = analogRead(sensorPin2);   // lettura del valore del sensore sulla porta Analog 1

//*************************************************************
mapOutput1 = map(sensorValue1,0,1023,0,255); // mappatura dei valori in ingresso 0-1023 a valori di uscita 0-255
mapOutput2 = map(sensorValue2,0,1023,0,255); // mappatura dei valori in ingresso 0-1023 a valori di uscita 0-255
//*************************************************************

analogWrite(motorPin1, mapOutput1); // scrittura dei valori da 0 a 255 sulla porta 11 (PWM) 
analogWrite(motorPin2, mapOutput2); // scrittura dei valori da 0 a 255 sulla porta 6 (PWM) 
}

