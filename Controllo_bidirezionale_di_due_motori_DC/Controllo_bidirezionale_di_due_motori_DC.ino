/*
Sketch per il controllo bidirezionale della velocità di
due motori DC collegati a un driver L293.
Pier Calderan - Luglio 2013
*/

int potPin1 = 0;  // porta analogica 0 per il potenziometro 1
int val1;             // variabile per la lettura dei dati del potenziometro 1

int potPin2 = 1; // porta analogica 1 per il potenziometro 2
int val2;            // variabile per la lettura dei dati del potenziometro 2

  const int switch1Pin = 2;    // porta digitale per l'interruttore 1
  const int switch2Pin = 3;    // porta digitale per l'interruttore 2
// motore 1 
  const int motor1Pin = 9;    // porta digitale per il pin 2 (1A)
  const int motor2Pin = 10;  // porta digitale per il pin 7 (2A)
  const int enable1Pin = 4;   // porta digitale per il pin 1 (1,2EN)

// motore 2 
  const int motor3Pin = 6;    // porta digitale per il pin 10 (3A)
  const int motor4Pin = 11;  // porta digitale per il pin 15 (4A)
  const int enable2Pin = 7;   // porta digitale per il pin 9 (3,4EN)

  void setup() {
    pinMode(switch1Pin, INPUT);
    pinMode(switch2Pin, INPUT);

// motore 1 
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(enable1Pin, OUTPUT);
// motore 2 
    pinMode(motor3Pin, OUTPUT);
    pinMode(motor4Pin, OUTPUT);
    pinMode(enable2Pin, OUTPUT);
 
    // abilitazione dei due motori:
    digitalWrite(enable1Pin, HIGH);
    digitalWrite(enable2Pin, HIGH);
  }
 
  void loop() {
//*************************************
  val1 = analogRead(potPin1);          // legge il valore del potenziometro 1 (da 0 a 1 023) 
  val1 = map(val1, 0, 1023, 0, 255);    // scala il valore per analogWrite (da 0 a 255) 
  val2 = analogRead(potPin2);          // legge il valore del potenziometro 2 (da 0 a 1 023) 
  val2 = map(val2, 0, 1023, 0, 255);   // scala il valore per analogWrite (da 0 a 255) 
  
    // se l'interruttore 1 è chiuso il motore gira verso sinistra
    if (digitalRead(switch1Pin) == HIGH) 
    {
      digitalWrite(motor1Pin, LOW); 
      analogWrite(motor2Pin, 255-val1);  // mantiene i valori con la rotazione invertita
    }
    // se l'interruttore 1 è aperto il motore gira verso destra
    else {
      digitalWrite(motor1Pin, HIGH);  
      analogWrite(motor2Pin, val1);   // valore scalato del potenziometro
    }

    // se l'interruttore 2 è chiuso il motore gira verso sinistra
    if (digitalRead(switch2Pin) == HIGH) 
    {
      digitalWrite(motor3Pin, LOW);
      analogWrite(motor4Pin, 255-val2); // mantiene i valori con la rotazione invertita
    }
    // se l'interruttore 2 è aperto il motore gira verso destra
    else {
      digitalWrite(motor3Pin, HIGH);  
      analogWrite(motor4Pin, val2);   // valore scalato del potenziometro
//*************************************
    }
  }

