#include <SoftwareSerial.h>
 
#define TXPIN 10
#define RXPIN 11

char val; 

SoftwareSerial BlueTooth(RXPIN, TXPIN);
 
void setup()
{
    pinMode(RXPIN, INPUT);
    pinMode(TXPIN, OUTPUT);
    pinMode(3, OUTPUT); // imposta il pin 3 come uscita
    pinMode(5, OUTPUT); // imposta il pin 3 come uscita
    pinMode(6, OUTPUT); // imposta il pin 3 come uscita
    Serial.begin(57600);
    BlueTooth.begin(115200);
}
 

void loop()
{
if(BlueTooth.available() > 0)
{
val = BlueTooth.read();      
if( val == '1' ) digitalWrite(3, HIGH); // se il valore è 1 il LED sul pin 3 si accende
if( val == '2' ) digitalWrite(3, LOW); // se il valore è 2 il LED sul pin 3 si spegne
if( val == '3' ) digitalWrite(5, HIGH); // se il valore è 3 il LED sul pin 5 si accende
if( val == '4' ) digitalWrite(5, LOW); // se il valore è 4 il LED sul pin 4 si spegne
if( val == '5' ) digitalWrite(6, HIGH); // se il valore è 5 il LED sul pin 6 si accende
if( val == '6' ) digitalWrite(6, LOW); // se il valore è 6 il LED sul pin 6 si spegne

}

if(Serial.available() > 0)
{
val = Serial.read();      
BlueTooth.write(val);
if( val == '1' ) digitalWrite(3, HIGH); // se il valore è 1 il LED sul pin 3 si accende
if( val == '2' ) digitalWrite(3, LOW); // se il valore è 2 il LED sul pin 3 si spegne
if( val == '3' ) digitalWrite(5, HIGH); // se il valore è 3 il LED sul pin 5 si accende
if( val == '4' ) digitalWrite(5, LOW); // se il valore è 4 il LED sul pin 4 si spegne
if( val == '5' ) digitalWrite(6, HIGH); // se il valore è 5 il LED sul pin 6 si accende
if( val == '6' ) digitalWrite(6, LOW); // se il valore è 6 il LED sul pin 6 si spegne
}
}
