/*
Sketch per il collegamento Internet con visualizzazione dei dati
di un sensore. Le pagine web HTML e i file delle immagini PNG
devono essere salvati nella card SD dello shield Ethernet.
Pier Calderan - Luglio 2013
*/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define BUFFER_SIZE   20

byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x78, 0x13};
IPAddress ip(192, 168, 1, 20);
EthernetServer server(80);
File HTML_File;
char DATA_READ[BUFFER_SIZE] = {0};
char BUFFER_INDEX = 0;

long sensorPin = 0, temperature = 0.0, temperatureF = 0.0; // variabili del sensore

void setup()
{
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    if (!SD.begin(4)) {return;}
    Ethernet.begin(mac, ip);
    server.begin();
}

void loop()
{
WriteData();
delay(250);

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
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println("Refresh: 20");
                        client.println();
                        HTML_File = SD.open("index.htm");        
                    }
                    else if (Parse_String(DATA_READ, "GET /termo.png")) {
                        HTML_File = SD.open("termo.png");
                        if (HTML_File) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }
                    
                    if (HTML_File) 
                    {
                        while(HTML_File.available()) {
                            client.write(HTML_File.read()); 
                        }
                        HTML_File.close();
                    }

                        client.println();
                        HTML_File = SD.open("sensor.htm");        
                        while(HTML_File.available()) 
                        {
                            client.write(HTML_File.read()); 
                        }
                        HTML_File.close();

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



void WriteData() {
String SensorData = "";

//funzione di lettura del sensore e conversione in gradi centigradi
int sensorValue = analogRead(sensorPin); 
float voltage = sensorValue * 5.0;  voltage /= 1024.0;
temperature = voltage * 100 ;
temperatureF = (temperature * 9.0 / 5.0) + 32.0;

SensorData ="<font face=Tahoma><ul><h2>Temperatura in gradi centigradi: ";
SensorData += String(temperature);
SensorData +="<br>Temperatura in gradi Fahrenheit: ";
SensorData += String(temperatureF);
SensorData += "</h2></font>";
 if (SD.exists("SENSOR.HTM")) SD.remove("SENSOR.HTM");

 File dataFile = SD.open("SENSOR.HTM", FILE_WRITE);
  
  if (dataFile) {
    dataFile.println(SensorData);
    dataFile.close();
  }  

}
