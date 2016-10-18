/*
Sketch di un mini sito di due pagine HTML per la visualizzazione dei dati
di un sensore di temperatura e del controllo di 7 LED.
Le pagine web HTML e i file delle immagini PNG
devono essere salvati nella card SD dello shield Ethernet.
Pier Calderan - Luglio 2013
*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define BUFFER_SIZE   20

byte mac[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
//byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //indirizzo MAC alternativo
IPAddress ip(192, 168, 1, 20);
EthernetServer server(80);
File HTML_File;
char DATA_READ[BUFFER_SIZE] = {0};
char BUFFER_INDEX = 0;

int Flag_pin[10];
boolean Open_Home = false;
int pin_reset = 0;

void setup()
{
  for (int i=2;i<11;i++)
  {  pinMode(i, OUTPUT); } // pin digitali di uscita
    digitalWrite(10, HIGH);
    if (!SD.begin(4)) {return;}
    Ethernet.begin(mac, ip);
    server.begin();
}

void loop()
{

    EthernetClient client = server.available();
      if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { 
                char c = client.read();
               
                if (BUFFER_INDEX < (BUFFER_SIZE - 1)) 
                {
                    DATA_READ[BUFFER_INDEX] = c;
                    BUFFER_INDEX++;
                }
                if (c == '\n' && currentLineIsBlank) {
if (Parse_String(DATA_READ, "GET / ") || Parse_String(DATA_READ, "GET /index.htm")) 
{
WriteData();
delay(25);
Open_Home = true;
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                                                client.println();
                        HTML_File = SD.open("index.htm", FILE_READ);        
                    }
                    else if (Parse_String(DATA_READ, "GET /termo.png")) {
                        HTML_File = SD.open("termo.png");
                        if (HTML_File) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }

                    else if (Parse_String(DATA_READ, "GET /led.htm")) {
                        Open_Home = false;
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        HTML_File = SD.open("led.htm", FILE_READ);        
                    }
                    

                    if (HTML_File) 
                    {
                        while(HTML_File.available()) {
                            client.write(HTML_File.read());
                        }
                        HTML_File.close();
                    }

                        client.println();

if (Open_Home == true) {

                        HTML_File = SD.open("SENSOR.HTM", FILE_READ);        
                        while(HTML_File.available()) 
                        {
                            client.write(HTML_File.read()); 
                        }
                        HTML_File.close();
                        }
int pinx = 0;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=2")) pinx = 2;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=3")) pinx = 3;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=5")) pinx = 5;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=6")) pinx = 6;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=7")) pinx = 7;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=8")) pinx = 8;
                    if (Parse_String(DATA_READ, "GET /led.htm?ID=9")) pinx = 9;


 
                  if (pinx > 0) 
                    {
                    pin_reset = 0;
                    client.write(pinx); 
                    Flag_pin[pinx]=1; 
                    Pin_High(pinx,Flag_pin[pinx]);  
                   }

                  if (pin_reset == 1) 
                    {
                    for (int i = 0;i<5;i++){
                    client.write(i); 
                    Flag_pin[i]=0; 
                    Pin_High(pinx,Flag_pin[i]);  }
                   }

                    BUFFER_INDEX = 0;
                    String_CLS(DATA_READ, BUFFER_SIZE);
                    break;
                }

                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
        delay(1);
        client.stop();
    }
}

void String_CLS(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

char Parse_String(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}

void Pin_High(int pin, int flag)
{
  for (int i=2;i<11;i++)
  {    digitalWrite(i, 0);
  } // spegne tutti i LED 

  digitalWrite(pin, flag);
  delay(25);
}


void WriteData() {
String SensorData = "";

//funzione di lettura del sensore e conversione in gradi centigradi

int sensorValue = analogRead(A0); //lettura del sensore sulla porta A0
float tensione = sensorValue * 4.8;   // prodotto della tensione di ingresso per lo step dell'ADC (0,0048 V = 4,8 mV)
float temperature = tensione /= 10;   // conversione in gradi centigradi ovvero tensione totale diviso 10 mv
float temperatureF = (temperature * 9 / 5) + 32; // conversione in gradi Fahrenheit

char buffer[10];
dtostrf(temperature, 2, 2, buffer); //conversione double to string
  
SensorData ="<font face=Tahoma><ul><h2>TEMPERATURA: gradi centigradi: ";
SensorData += String(buffer);

dtostrf(temperatureF, 2, 2, buffer); //conversione double to string

SensorData +="<br>TEMPERATURA: gradi Fahrenheit: ";
SensorData += String(buffer);

 if (SD.exists("SENSOR.HTM")) SD.remove("SENSOR.HTM");

 File dataFile = SD.open("SENSOR.HTM", FILE_WRITE);
  
  if (dataFile) {
    dataFile.println(SensorData);
    dataFile.close();
  }  

}



