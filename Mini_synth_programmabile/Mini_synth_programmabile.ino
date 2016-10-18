/*
Sketch di un sintetizzatore dotato di sequencer a 4 tracce e pattern di 32 beat.
Modificare i valori dlle tracce a piacere per cambiare i suoni della sezione ritmica e dei suoni melodici.
Il potenziometro A5 regola la velocità del sequencer.
Pier Calderan - Luglio 2013
*/
#include <LiquidCrystal.h>
#include "the_synth.h"
#include <Stdio.h>
#include <MIDI.h>
#define PWM_PIN 3

LiquidCrystal LCD(11, 10, 6, 5, 4, 2);

unsigned int counter=0;
unsigned char bar;


void setup()
{
  MIDI.begin();
  LCD.clear();
  LCD.begin(16, 2);
  LCD.setCursor(0,0);
  LCD.print("PierCalderan Syn");
  
  pinMode(PWM_PIN,OUTPUT);
  
  SetupInt();  
  initSynth();

  bar = 0;


}


unsigned char pattern[4][32]=
{
	{1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0}, // kick
	{50,0,0,0,0,0,48,50,0,0,0,0,0,57,56,55,54,53,52,51,50,0,0,0,0,0,0,0,0,0,0,48}, // basso
	{0,0,50,0,0,0,0,0,0,0,48,0,0,0,0,0,50,0,0,0,0,72,0,0,0,0,0,36,0,0,0,0}, // mod
	{0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,0,1,0,1,0}  // click
};

void loop()
{
float ton1=(analogRead(0)/2); // regola l'intonazione della prima traccia (kick)
float ton2=(analogRead(1)/2); // regola l'inviluppo della seconda traccia (basso)
float ton3=(analogRead(2)/2); // regola l'intonazione della terza traccia (mod)
float en3=(analogRead(3)/1000);   // regola l'inviluppo della terza traccia (click)

float ton4=(analogRead(4));   // regola l'inviluppo della quarta traccia

setup_voice(0,(unsigned int)SinTable,ton1,(unsigned int)Env1,1.0,512);
setup_voice(1,(unsigned int)RampTable,ton2,(unsigned int)Env3,1.0,512);
setup_voice(2,(unsigned int)SawTable,ton3,(unsigned int)Env3,en3,512);

setup_voice(3,(unsigned int)NoiseTable,ton4,(unsigned int)Env3,.04,512);
  
int vel=(analogRead(5))/5;  // regola la velocità del pattern
  delay(vel);
  LCD.setCursor(0,1);
  LCD.print("BPM: ");

int temp = vel;
char ascii[3];
sprintf(ascii,"%03d",250-vel);
LCD.print(ascii);

    if(synthTick()) 
    {
bar = counter&0x1f;

LCD.setCursor(9,1);
LCD.print("BAR: ");

int temp = bar;
char ascii[3];
sprintf(ascii,"%02d",temp);
LCD.print(ascii);

      if(pattern[0][bar]) trigger(0);
      if(pattern[1][bar]) mtrigger(1,pattern[1][bar]);
      if(pattern[2][bar]) mtrigger(2,pattern[2][bar]);
//      if(pattern[2][bar]) trigger(2);        
      if(pattern[3][bar]) trigger(3);

      tim=0;
      counter++;
  }
}





void SetupInt() {
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);

  TCCR2A = (TCCR2A | _BV(COM2B1)) & ~_BV(COM2B0);
  TCCR2B &= ~(_BV(COM2B1) | _BV(COM2B0));
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

  OCR2A = 0;
  cli();
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  OCR1A = 1536;
  TIMSK1 |= _BV(OCIE1A);
  sei();
}











