/*
Sketch di un termometro a LED con sensore di temperature LM35.
Pier Calderan - Luglio 2013
*/

int sensorPin = 0, temperature = 0;
int pinLed1 = 2, pinLed2 = 3, pinLed3 = 4, pinLed4 = 5;

void setup() 
{ // inizializzazione delle porte LED come uscite
pinMode(pinLed1, OUTPUT);
pinMode(pinLed2, OUTPUT);
pinMode(pinLed3, OUTPUT);
pinMode(pinLed4, OUTPUT);
}

void loop() 
{
int sensorValue = analogRead(sensorPin); // lettura della tensione di ingresso del sensore
float voltage = sensorValue * 5.0; // conversione della voltage di ingresso del sensore a sensorValue float * 5 volt
voltage /= 1024.0;            // divisione della tensione in 1024 livelli di tensione (10 bit)
float temperature = voltage * 100 ; //conversione da 10 mv per grado in gradi centigradi

// scrittura della temperatura ai LED in base alla soglia
if (temperature<=20)
{
   digitalWrite(pinLed1, HIGH);
   digitalWrite(pinLed2, LOW);
   digitalWrite(pinLed3, LOW);
   digitalWrite(pinLed4, LOW);
}

if(temperature>20 && temperature<=25)
{
  digitalWrite(pinLed1, HIGH);
  digitalWrite(pinLed2, HIGH);
  digitalWrite(pinLed3, LOW);
  digitalWrite(pinLed4, LOW);
}

if(temperature>25 && temperature<=30)
{
 digitalWrite(pinLed1, HIGH);
 digitalWrite(pinLed2, HIGH);
 digitalWrite(pinLed3, HIGH);
 digitalWrite(pinLed4, LOW);
}

if(temperature>30)
{
  digitalWrite(pinLed1, HIGH);
  digitalWrite(pinLed2, HIGH);
  digitalWrite(pinLed3, HIGH);
  digitalWrite(pinLed4, HIGH);
}
delay(25); // ritardo per l'elaborazione della temperatura
}

