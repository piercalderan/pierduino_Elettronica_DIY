/*
Sketch di un VU meter stereo con display LCD per il canale sinistro e destro e indicazione di sovraccarico.
Sono visibili "10 LED" virtuali che si accendono in sequenza in base all'ampiezza del segnale di ingresso. 
Il segnale audio stereo è applicato alle porte analogiche A0 e A1.
Pier Calderan - Luglio 2013
*/

#include <LiquidCrystal.h>
// caratteri personalizzati per il display
byte charVU[8]= {B00000,B00000,B11111,B11111,B11111,B11111,B00000,B00000}; // carattere 1 VU
byte charL[8] = {B10000,B10000,B10000,B10000,B10000,B11111,B00000,B00000}; // carattere 2 L
byte charR[8] = {B00000,B11110,B10001,B10010,B11100,B10010,B10001,B00000}; // carattere 3 R
byte charC[8] = {B00000,B00000,B01110,B11111,B11111,B01110,B00000,B00000}; // carattere 4 C
byte empty[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000}; // carattere 5 E

const int reading = 3;       // numero di letture

int count_Left = 0;         // conteggio incrementale
int Input_Left = 0;         // variabile per l'ingresso sinistro
int max_Left = 0;           // vairabile per il conteggio massimo

int count_Right = 0;        // idem
int Input_Right = 0;        // idem
int max_Right = 0;          // idem

int input_Audio_L = A0;     // ingresso audio sinistro su input analogico A0
int input_Audio_R = A1;     // ingresso aidio destro su input analogico A1

int volume_L = 0;  // variabile per il volume sinistro
int volume_R = 0;  // variabile per il volume destro

int VUmeter_L;  // variabile per il VU meter sinistro
int VUmeter_R;  // variabile per il VU meter destro

int redux = 10; // ATTENZIONE! variabile per la riduzione dei dati letti, non della tensione in ingresso!
int left_M, right_M;  // variabili di scambio per i VU meter sinistro e destro

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(11, 10, 6, 5, 4, 2); // Attenzione alla piedinatura!

void setup() {
  lcd.begin(16, 2); 
  // creazione dei caratteri personalizzati in memoria del display
  lcd.createChar(1, charVU); // carattere VU meter
  lcd.createChar(2, charL);  // carattere lettera L
  lcd.createChar(3, charR);  // carattere lettera R
  lcd.createChar(4, charC);  // carattere sovraccarico (clip)
  lcd.createChar(5, empty);  // carattere vuoto
  lcd.clear();
  delay(1000);
}

void loop() {
  lcd.setCursor(0, 0);     // imposta il cursore della prima riga
  lcd.write(2);            // scrive il carattere personalizzato 6 (L)
  lcd.setCursor(0, 1);     // imposta il cursore della prima riga
  lcd.write(3);            // scrive il carattere personalizzato 7 (R)

//****** funzioni per il canale sinistro
  Input_Left = analogRead(input_Audio_L) / redux; // lettura dati audio ridotti di 10 volte
                                                  // i valori vanno da 0 a 102 circa e sono tarati per un ingresso ottimale di 1 v
  if (Input_Left > 30) { // limite del picco oltre 300 mv
    lcd.setCursor(15, 0);   
    lcd.write(4); // scrive il carattere di picco
  }
  if (Input_Left < 31) {
    lcd.setCursor(15, 0);   
    lcd.write(5); // scrive il carattere vuoto al posto del picco
  }
  if(Input_Left > max_Left) { 
    max_Left = Input_Left;
  }
  count_Left ++; // incrementa il conteggio

  if (count_Left >= reading) // dopo 3 letture... 
  {             
    count_Left = 0; // il conteggio torna a 0
    left_M = max_Left;
    max_Left = 0;    
  }
//****** Funzioni per il canale destro identiche al sinistro
  Input_Right = analogRead(input_Audio_R) / redux;
  if (Input_Right > 30) { 
    lcd.setCursor(15, 1);   
    lcd.write(4);
  }
  if (Input_Right < 31) {
    lcd.setCursor(15, 1);   
    lcd.write(5); 
  }
  if(Input_Right > max_Right) {
    max_Right = Input_Right;
  }
  count_Right++;
  if (count_Right >= reading)
  {             
    count_Right = 0;                         
    right_M = max_Right;
    max_Right = 0;
  }
//****** routine VU meter canale destro
  volume_R = right_M / 3;
  if(volume_R > 14) { 
    volume_R = 14;
  }
  for(VUmeter_R = 0 ; VUmeter_R < volume_R ; VUmeter_R ++)
  {
    lcd.setCursor(VUmeter_R + 1, 1);
    lcd.write(1); // scrive il carattere VU meter
  }
  for(VUmeter_R = volume_R + 1 ; VUmeter_R < 15 ; VUmeter_R ++)
  {
    lcd.setCursor(VUmeter_R, 1);
    lcd.write(5); // scrive il carattere vuoto
  }
//****** VU canale sinistro
  volume_L = left_M / 3;   
  if(volume_L > 14){ 
    volume_L = 14;
  }
  for(VUmeter_L = 0 ; VUmeter_L < volume_L ; VUmeter_L ++)
  {
    lcd.setCursor(VUmeter_L + 1, 0);
    lcd.write(1);
  }
  for(VUmeter_L = volume_L + 1 ; VUmeter_L < 15 ; VUmeter_L ++)
  {
    lcd.setCursor(VUmeter_L, 0);
    lcd.write(5);
  }
}



