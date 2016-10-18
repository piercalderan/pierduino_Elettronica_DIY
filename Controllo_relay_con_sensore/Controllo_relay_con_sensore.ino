/*
Sketch per il controllo di un relè collegato alla porta 13
tramite un sensore collegato alla porta A0.
Pier Calderan - Luglio 2013
*/

int photocellPin = 5;     // porta analogica 0 del fotoresistore
int photocellReading;   // variabile di lettura del fotoresistore
int pinRelay = 13;      // porta digitale 13 assegnata al relè
int lightLimit = 800;  // soglia per l'accensione del relè

void setup() { 
pinMode(pinRelay, OUTPUT); // imposta la porta 13 come uscita
Serial.begin(9600);
}

void loop() 
{
  photocellReading = analogRead(photocellPin);  
  
if (photocellReading > lightLimit)
  digitalWrite(pinRelay,HIGH);
else
  digitalWrite(pinRelay,LOW);
  Serial.println(photocellReading);
  delay(50); // ritardo per l'elaborazione del dato del sensore
}


