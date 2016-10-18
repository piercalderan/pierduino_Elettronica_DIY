/*
Sketch di un termometro con LM35 e visualizzaione su LCD
di gradi centigradi e Fahrenheit con precisione al centesimo di grado.
Pier Calderan - Luglio 2013
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 10, 6, 5, 4, 2);

int sensorPin = 0;

void setup() {
lcd.begin(16, 2); }

void loop() {
//funzione di lettura del sensore e conversione in gradi centigradi
int sensorValue = analogRead(sensorPin); 
float voltage = sensorValue * 5.0; 
voltage /= 1024.0;
float temperature = voltage * 100 ;

lcd.setCursor(0, 0);
lcd.print("TEMP C  TEMP F");
lcd.setCursor(0, 1);
lcd.print(temperature); // temperature in gradi centigradi
float temperatureF = (temperature * 9.0 / 5.0) + 32.0;  // conversione in gradi Fahrenheit
lcd.setCursor(8, 1);
lcd.print(temperatureF); // temperature in gradi Fahrenheit

delay(250); // ritardo di 250 millisecondi
}



