#include <SD.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // l'assegnazione è diversa da quella standard!
File myFile; // creazione dell'oggetto myFile
const int chipSelect = 10; // selezione del CS per Arduino UNO

void setup() {
lcd.begin(16, 2);
lcd.setCursor(0,0); lcd.print("SD CARD TEST");
delay(2000);
lcd.setCursor(0,1); lcd.print("Inizializzazione");
pinMode(10, OUTPUT); // importantissimo! Impostare il CS come OUTPUT!

if (!SD.begin(chipSelect)) {
lcd.setCursor(0,1); lcd.print(">SD CARD ERROR!<"); // la card non viene riconosciuta
return; }
lcd.setCursor(0,1); lcd.print("SD CARD INIT OK!"); // la card viene riconosciuta con successo!
delay(2000);

lcd.print("Create test.txt!"); 
myFile = SD.open("test.txt", FILE_WRITE); // crea un file test.txt
myFile.close();
delay(2000);

if (SD.exists("test.txt")) {
lcd.setCursor(0,1); lcd.print("test.txt esiste!"); }
else {
lcd.setCursor(0,1); lcd.print("File rimosso!   "); }
delay(2000);
lcd.setCursor(0,1); lcd.print("Remove test.txt!"); 
SD.remove("test.txt"); // rimozione del file test.txt
delay(2000);

if (SD.exists("test.txt")){ 
lcd.setCursor(0,1); lcd.print("test.txt esiste!"); }
else {
delay(2000);  
lcd.setCursor(0,1); lcd.print("File rimosso!   "); // il file test.txt manca
delay(2000);  
lcd.setCursor(0,1); lcd.print("END OF TEST. OK!"); // fine del test. }
}
}

void loop(){}
