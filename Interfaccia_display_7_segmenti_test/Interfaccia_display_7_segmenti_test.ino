/*
Interfaccia display 7 segmenti.
Test per accendere tutti i segmenti (appare il numero 8).
Pier Calderan - Luglio 2013
*/

void setup() {              
 pinMode(6, OUTPUT); // imposta la porta 6 per il segmento A
 pinMode(7, OUTPUT); // imposta la porta 7 per il segmento B
 pinMode(8, OUTPUT); // imposta la porta 8 per il segmento C
 pinMode(9, OUTPUT); // imposta la porta 9 per il segmento D
 pinMode(10, OUTPUT); // imposta la porta 10 per il segmento E
 pinMode(11, OUTPUT); // imposta la porta 11 per il segmento F
 pinMode(12, OUTPUT); // imposta la porta 12 per il segmento G
 pinMode(13, OUTPUT); // imposta la porta 13 per il punto (DP)
 digitalWrite(13, 0);       // il punto decimale viene tenuto spento

// scrive il numero "8" 
digitalWrite(6, 1);   // accende il segmento A
digitalWrite(7, 1);   // accende il segmento B
digitalWrite(8, 1);   // accende il segmento C
digitalWrite(9, 1);   // accende il segmento C
digitalWrite(10, 1); // accende il segmento E
digitalWrite(11, 1); // accende il segmento F
digitalWrite(12, 1); // accende il segmento G
digitalWrite(13, 1); // accende il segmento G
}

void loop(void) 
{

}
