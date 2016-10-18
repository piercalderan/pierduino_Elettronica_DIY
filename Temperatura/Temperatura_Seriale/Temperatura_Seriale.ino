/*
Sketch per la trasmissione seriale del valore del sensore di temperatura.
Pier Calderan - Luglio 2013
*/

int sensorPin = 5;

void setup()
{
  Serial.begin(57600);
}

void loop() 
{
int valore = analogRead(sensorPin); // lettura della tensione di ingresso del sensore
Serial.write(valore);
delay(250); // ritardo di 250 millisecondi
}
