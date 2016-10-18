/*
Sketch per il controllo di un relè collegato alla porta 13
tramite un sensore di tilt collegato alla porta digitale 8.
Pier Calderan - Luglio 2013
*/

const int buttonTilt = 8; // imposta la porta 8 per il sensore di tilt
int buttonState = 0;      // imposta la variabile di stato del sensore

int pinRelay = 13;       // porta digitale 13 assegnata al relè

void setup() { 
pinMode(buttonTilt, INPUT);   // inizializza il sensore come input
pinMode(pinRelay, OUTPUT);    // imposta la porta 13 come uscita
}

void loop() 
{
  buttonState = digitalRead(buttonTilt); // legge lo stato del sensore
  if (buttonState == HIGH)               // se il sensore è inclinato...
  digitalWrite(pinRelay,HIGH);           // il relè si accende
else
  digitalWrite(pinRelay,LOW);            // altrimenti il relè resta spento
  delay(50); // ritardo per l'elaborazione del dato del sensore
}


