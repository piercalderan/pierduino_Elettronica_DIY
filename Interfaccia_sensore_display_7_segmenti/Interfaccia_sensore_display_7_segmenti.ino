/*
Interfaccia display 7 segmenti con
sensore collegato alla porta A0.
Pier Calderan - Luglio 2013
*/

int photocellPin = 0;     // porta analogica 0 del fotoresistore
int photocellReading;   // variabile di lettura del fotoresistore

byte SegDigit[10][7] = { 
{ 0,1,1,0,0,0,0 },  // segmenti accesi per il numero 1
{ 1,1,0,1,1,0,1 },  // segmenti accesi per il numero 2
{ 1,1,1,1,0,0,1 },  // segmenti accesi per il numero 3
{ 0,1,1,0,0,1,1 },  // segmenti accesi per il numero 4
{ 1,0,1,1,0,1,1 },  // segmenti accesi per il numero 5
{ 1,0,1,1,1,1,1 },  // segmenti accesi per il numero 6
{ 1,1,1,0,0,0,0 },  // segmenti accesi per il numero 7
{ 1,1,1,1,1,1,1 },  // segmenti accesi per il numero 8
{ 1,1,1,1,0,1,1 },  // segmenti accesi per il numero 9
{ 1,1,1,1,1,1,0 }   // segmenti accesi per il numero 0
};


void setup() { 
pinMode(6, OUTPUT); // imposta la porta 6 per il segmento A
pinMode(7, OUTPUT); // imposta la porta 7 per il segmento B
pinMode(8, OUTPUT); // imposta la porta 8 per il segmento C
pinMode(9, OUTPUT); // imposta la porta 9 per il segmento D
pinMode(10, OUTPUT); // imposta la porta 10 per il segmento E
pinMode(11, OUTPUT); // imposta la porta 11 per il segmento F
pinMode(12, OUTPUT); // imposta la porta 12 per il segmento G
pinMode(13, OUTPUT); // imposta la porta 13 per il punto (DP)
}

void loop() {
    photocellReading = analogRead(photocellPin);  
  int StepReading = map(photocellReading, 0, 1023, 0, 9);

  SegWrite(StepReading);
  
}

// funzione per scrivere i numeri da 0 a 9
void SegWrite(byte digit) {
  byte pin = 6; 			// porta digitale di partenza
  for (byte SegCount = 0; SegCount < 7; ++SegCount) 
{
   digitalWrite(pin, SegDigit[digit][SegCount]);
   ++pin;
  }
}
