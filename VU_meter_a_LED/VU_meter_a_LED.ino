/*
Sketch di un VU meter a 10 LED collegati alle porte 2 - 11.
Il segnale audio è applicato alla porta analogica A0.
Pier Calderan - Luglio 2013
*/

int VU_led[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};  // matrice di LED
int audioInput = 0;  // variabile per l'ingresso audio
int audioData, i;    // variabile per i dati audio e variabile per il conteggio incrementale
int redux = 10;      // variabile di riduzione dei dati audio in ingresso

void setup() {
for (i = 0; i < 10; i++)  
  pinMode(VU_led[i], OUTPUT); // imposta la matrice di LED come uscite
}

void loop() {
audioData = analogRead(audioInput) / redux;  // lettura del segnale audio in ingresso, ridotto di 10 volte per sicurezza
if (audioData == 0)  // se non ci sono dati audio, i LED rimangono spenti
{   for(i = 0; i < 10; i++)      {
     digitalWrite(VU_led[i], 0); }
}  else  { // altrimenti...
   for (i = 0; i < audioData; i++) // ... si accendono tanti LED quanto più il segnale è forte
{ digitalWrite(VU_led[i], 1);   }
   for(i = i; i < 10; i++)  
{
  digitalWrite(VU_led[i], 0);   } // spegnimento dei LED sotto il livello di segnale
}
}

