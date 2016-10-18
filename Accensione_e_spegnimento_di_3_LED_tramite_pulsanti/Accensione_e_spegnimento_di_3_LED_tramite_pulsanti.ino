/*
Sketch per l'accensione e lo spegnimento di tre LED.
Pier Calderan - Luglio 2013
*/
int buttonState1 = 0;       // imposta la variabile di stato del pulsante 1
int buttonState2 = 0;       // imposta la variabile di stato del pulsante 2
int buttonState3 = 0;       // imposta la variabile di stato del pulsante 3
const int button1 = 3;     // imposta la porta 3 per il pulsante 1
const int button2 = 5;     // imposta la porta 5 per il pulsante 2
const int button3 = 6;     // imposta la porta 6 per il pulsante 3
int flag1=2, flag2=4, flag3=6; // variabili flag che tengono spenti i LED RX

void setup() {
Serial.begin(57600); // imposta la velocità della porta seriale a 57600 baud
pinMode(button1, INPUT);   // inizializza il pulsante 1 come input
pinMode(button2, INPUT);   // inizializza il pulsante 2 come input
pinMode(button3, INPUT);   // inizializza il pulsante 3 come input
}

void loop()  {
buttonState1 = digitalRead(button1); // legge lo stato del pulsante 1
buttonState2 = digitalRead(button2); // legge lo stato del pulsante 2
buttonState3 = digitalRead(button3); // legge lo stato del pulsante 3
if (buttonState1 == HIGH) flag1=1; // se il pulsante 1 è premuto...
else flag1=2;  // altrimenti rimane spento
if (buttonState2 == HIGH) flag2=3; // se il pulsante 2 è premuto...
else flag2=4; // altrimenti rimane spento
if (buttonState3 == HIGH) flag3=5; // se il pulsante 3 è premuto...
else flag3=6; // altrimenti rimane spento

Serial.print(flag1); // manda il valore flag 1 di acceso o spento
Serial.print(flag2); // manda il valore flag 2 di acceso o spento
Serial.print(flag3); // manda il valore flag 3 di acceso o spento
delay(50); }

