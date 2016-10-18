/*
Sketch per il controllo di servomotori con potenziometri e sensori.
Pier Calderan - Luglio 2013
*/

#include <Servo.h> 
Servo myservo1;  // creazione dell'oggetto myservo1 per controllare un servomotore 
Servo myservo2;  // creazione dell'oggetto myservo2 per controllare un servomotore 

int potPin1 = 0;  // porta analogica 0 per il fotoresistore
int potPin2 = 1;  // porta analogica 1 per il potenziometro

int val1;             // variabile per leggere il valore della porta analogica 0
int val2;             // variabile per leggere il valore della porta analogica 1

void setup() 
{ 
myservo1.attach(2);  // "attacca" il servomotore alla porta 2
myservo2.attach(3);  // "attacca" il servomotore alla porta 3 
} 
 
void loop() 
{ 
 val1 = analogRead(potPin1);           // legge il valore del fotoresistore (da 0 a 1023)
 val1 = map(val1, 0, 1023, 0, 179);  // scala il valore per il servo (180°) 
 myservo1.write(val1);             // imposta la posizione del servo in base al valore scalato

 val2 = analogRead(potPin2);           // legge il valore del potenziometro (da 0 a 1023)
 val2 = map(val2, 0, 1023, 0, 179);  // scala il valore per il servo (180°) 
 myservo2.write(val2);             // imposta la posizione del servo in base al valore scalato

 delay(15);                                 // attesa per dare tempo ai motori di posizionarsi 
}

