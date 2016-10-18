/*
Sketch per il re-invio di dati in modalità MIDI Thru.
Ogni volta che il dato viene ricevuto il LED lampeggia (pin 13).
Pier Calderan - Luglio 2013
*/

#include <MIDI.h>
#define LED 13   		// LED pin on Arduino board

void Blink(byte num) 
{ 	// Basic blink function
  for (byte i=0;i<num;i++) {
    digitalWrite(LED,HIGH);
    delay(100);
    digitalWrite(LED,LOW);
    delay(100);
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  MIDI.begin();            	// Launch MIDI with default options
				// (input channel is default set to 1)
}

void loop() {
if (MIDI.read()) {
  
if (MIDI.getType() == NoteOn ) 
{	 
  MIDI.sendNoteOn(MIDI.getData1(),MIDI.getData2(),1);
  Blink(1);
}
}
}

