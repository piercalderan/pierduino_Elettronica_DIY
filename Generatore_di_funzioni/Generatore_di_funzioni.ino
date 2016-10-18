/*
Sketch di un Function Generator, ovvero di un generatore di funzioni.
Produce 4 forme d'onda a bassa frequenza (15-1000 Hz circa): Triangle, Square, Sawtooth e Sine.
Collegare opzionalmente un display 16x2 rispettando la piedinatura come indicato.
Collegare opzionalmente un potenziometro da 4,7k - 10 k alla porta analogica 5.
Uscita audio PWM impostata sul pin 3 (è necessario cambiare il collegamento standard del display!).
Pier Calderan - Luglio 2013
*/

#include "avr/pgmspace.h"
#include <LiquidCrystal.h>
// attenzione al collegamento del display!
LiquidCrystal lcd(12, 11, 6, 5, 4, 2);

// area di memoria per contenere i dati della forma d'onda
unsigned char RAMTable[256]; 

// tabella di 256 valori seno
unsigned char sineTab[] = {
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
  242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
  221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
  76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
  33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124};

double freqOut;
const double REFCLOCK=31376.6;      // clock di riferimento
int counter=0;

// variabili  volatili per l'interrupt
volatile byte IntCNT;              
volatile byte IntCNT1;             
volatile byte counter_ms;         //counter fittizio per poter disabilitare il timer0 (quello usato dalla funzione delay())
volatile unsigned long Phase;     // accumulatore della fase 32 bit
volatile unsigned long Freqword;  // parola di 32 bit
int statusPin1 = 0;
int buttonInput1 = 13;

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Function Gener. ");
  lcd.setCursor(0,1);
  lcd.print("by Pier Calderan");
  delay (3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SINE WAVE       ");

SineWave(); // forma d'onda SINE di default

  pinMode(3, OUTPUT); // pin 3 = PWM out frequency: NOTA cambiare la corrispondente porta OCR2B e i bit COM2B0 e COM2B1 del timer2
  pinMode(buttonInput1, INPUT);

Setup_timer2(); 
  // disabilita gli interrupt per evitare variazioni nel timing
  bitClear(TIMSK0,TOIE0); // disabilita il Timer0 (quello della funzione delay())
  bitSet(TIMSK2,TOIE2);   // abilita il Timer2 Interrupt
}

void loop()
{
freqOut=analogRead(5);                // legge il valore da 0 a 1023 dal potenziometro collegato alla porta analogica A5
Freqword=pow(2,32)*freqOut/REFCLOCK;  // calcolo frequenza di uscita rispetto al clock di riferimento
  if (counter_ms > 100) {             // timer per evitare lo sfarfallio del display (ATTENZIONE delay() è disabilitato!)
  counter_ms=0;
  lcd.setCursor(0,1);
  lcd.print("Freq.  ");
  lcd.print(freqOut);      
        
statusPin1=digitalRead(buttonInput1);
if (statusPin1)  counter ++;
  
  if (counter > 3)
 { 
  counter = 0;
  TriangleWave();
// visualizza il tipo di forma d'onda la frequenza sul display
  lcd.setCursor(0,0);
  lcd.print("TRIANGLE WAVE   ");
  lcd.setCursor(0,1);
  lcd.print("Freq.  ");
  lcd.print(freqOut);      
  }
  
if (counter == 1)
{
  SquareWave();
// visualizza il tipo di forma d'onda la frequenza sul display
  lcd.setCursor(0,0);
  lcd.print("SQUARE WAVE     ");
  lcd.setCursor(0,1);
  lcd.print("Freq.  ");
  lcd.print(freqOut);      
  }
  
if (counter == 2)
{
  Sawtooth();
// visualizza il tipo di forma d'onda la frequenza sul display
  lcd.setCursor(0,0);
  lcd.print("SAWTOOTH WAVE   ");
  lcd.setCursor(0,1);
  lcd.print("Freq.  ");
  lcd.print(freqOut);      
  }
if (counter == 3)
{
  SineWave();
// visualizza il tipo di forma d'onda la frequenza sul display
  lcd.setCursor(0,0);
  lcd.print("SINE WAVE       ");
  lcd.setCursor(0,1);
  lcd.print("Freq.  ");
  lcd.print(freqOut);      
  }
}
}

//******************************************************************
// Imposta il timer2 con prescaler a 1, PWM mode per la correzione di fase PWM, 16.000.000/512 = 31.372 Hz di clock
void Setup_timer2() {
// Timer2 Clock Prescaler a 1
  bitSet(TCCR2B, CS20);
  bitClear(TCCR2B, CS21);
  bitClear(TCCR2B, CS22);
// Timer2 PWM Mode impostato su Phase Correct PWM sul pin 3
  bitClear(TCCR2A, COM2B0);
  bitSet(TCCR2A, COM2B1);
// Mode 1 Phase Correct PWM
  bitSet(TCCR2A, WGM20); 
  bitClear(TCCR2A, WGM21);
  bitClear(TCCR2B, WGM22);
}

// Timer2 Interrupt a 31372,550 KHz = 32uSec (REFCLOCK per il generatore)
// runtime di 8 microsecondi
ISR(TIMER2_OVF_vect) 
{
 static unsigned int PhaseOut;

  Phase=Phase+Freqword; // accumula fase accurata a 32 bit
  IntCNT=Phase >> 24;   // legge gli 8 bit superiori

PhaseOut=PhaseOut+freqOut;

OCR2B=RAMTable[PhaseOut>>8];   // uscita audio sul pin 3

   if(IntCNT1++ == 125) {  
    counter_ms++; // incremento di 4 millisecondi
    IntCNT1=0;
   }   
}


void TriangleWave()
{
  for (int i = 0; i < 128; ++i) {
    RAMTable[i] = i * 2;
  }
  int value = 255;
  for (int i = 128; i < 256; ++i) {
    RAMTable[i] = value; //inserisce i valori nella tabella
    value -= 2;
  }
}

void SquareWave()
{
  int t;
  for (int i = 0; i < 256; ++i) 
  {
if (i == 0) t = 0;
if (i == 128) t = 255;
    RAMTable[i] = t; // // inserisce i valori nella tabella
  }
}

void Sawtooth()
{
  for (int i = 0; i < 256; ++i) 
  {
    RAMTable[i] = i; // inserisce i valori nella tabella
  }
}


void SineWave()
{
// inserisce i valori nella tabella
  for (int i = 0; i < 256; ++i) 
  {
    RAMTable[i] = sineTab[i]; // inserisce i valori nella tabella
  }
}
