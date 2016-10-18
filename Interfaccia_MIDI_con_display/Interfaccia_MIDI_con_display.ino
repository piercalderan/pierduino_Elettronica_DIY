/*
Sketch per la visualizzazione su dispaly LCD
dei dati in ingresso all'interfaccia MIDI.
Pier Calderan - Luglio 2013
*/
#include <MIDI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 10, 6, 5, 4, 2);

void setup() {
  lcd.begin(16, 2);
  MIDI.begin();
lcd.setCursor(0,0);
lcd.print("DATA 1");
lcd.setCursor(8,0);
lcd.print("DATA 2");
}

void loop() {
if (MIDI.read()) {
  
if (MIDI.getType() == NoteOn ) 
{	 
lcd.clear();
lcd.setCursor(0,0);
lcd.print("NOTE on");
lcd.setCursor(6,0);
lcd.print("VELOCITY");

  lcd.setCursor(1,1);    
  lcd.print(MIDI.getData1(),DEC);
  lcd.setCursor(9,1);    
  lcd.print(MIDI.getData2(),DEC);
}
if (MIDI.getType() == NoteOff ) 
{	 
lcd.clear();
lcd.setCursor(0,0);
lcd.print("NOTE x");
lcd.setCursor(6,0);
lcd.print("VELOCITY");

  lcd.setCursor(1,1);    
  lcd.print(MIDI.getData1(),DEC);
  lcd.setCursor(9,1);    
  lcd.print(MIDI.getData2(),DEC);
}

if (MIDI.getType() == ProgramChange ) 
{	 
lcd.clear();
lcd.setCursor(0,0);
lcd.print("PRG ");
lcd.setCursor(6,0);
lcd.print("NUMBER");

  lcd.setCursor(1,1);    
  lcd.print("---->");
  lcd.setCursor(9,1);    
  lcd.print(MIDI.getData1(),DEC);
}

}
}




