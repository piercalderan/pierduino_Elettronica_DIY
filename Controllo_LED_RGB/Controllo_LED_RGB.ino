/*
Sketch per il controllo di LED RGB.
Pier Calderan - Luglio 2013
*/
int GreenPin=9, BluePin=10, RedPin=11, sensorValue=0, val;

void setup()
{
pinMode(RedPin, OUTPUT); pinMode(GreenPin, OUTPUT); pinMode(BluePin, OUTPUT);  
}

void loop()
{
val=map(analogRead(sensorValue),0,1024,0,900);  // imposta la mappa dei valori
  
if (val<150) { // RGB spento
analogWrite(RedPin,0); analogWrite(BluePin,0); analogWrite(GreenPin,0);    
} 

else if (val<300) {
analogWrite(RedPin,map(val,150,300,0,255));
analogWrite(BluePin,map(val,150,300,255,0)); 
analogWrite(GreenPin,255);

} else if (val<450) {
analogWrite(RedPin,255);
analogWrite(BluePin,0); 
analogWrite(GreenPin,255);  
} 

else if (val<600) {
analogWrite(RedPin,255);
analogWrite(BluePin,map(val,450,600,0,255)); 
analogWrite(GreenPin,map(val,450,600,255,0));
}

else if (val<750) {
analogWrite(RedPin,255);
analogWrite(BluePin,255); 
analogWrite(GreenPin,0);
}

else if (val<900) {
analogWrite(RedPin,map(val,750,900,255,0));
analogWrite(BluePin,255); 
analogWrite(GreenPin,map(val,750,900,0,255));
}
delay(10);
}

