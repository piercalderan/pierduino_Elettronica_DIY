/*
Sketch per il controllo di un braccio meccanico (3 servomotori) 
tramite connessione Wi-Fi e protocollo OSC.
Pier Calderan - Luglio 2013
*/
#include <Servo.h> 
 
Servo motor_1; 
Servo motor_2; 
Servo motor_3; 

#define OSC_SLASH 0
#define OSC_PAGE 1
#define OSC_SLASH2 2
#define OSC_READ_VALUE 3
#define OSC_READ_VALUE_1 4
#define OSC_READ_VALUE_2 5
#define OSC_READ_VALUE_3 6
#define OSC_READ_VALUE_4 7
#define OSC_READ_VALUE_5 8
#define OSC_READ_VALUE_6 9
#define OSC_READ_VALUE_7 10
#define OSC_READ_FOO_1 11
#define OSC_READ_FOO_2 12
#define OSC_READ_FOO_3 13
#define OSC_READ_FOO_4 14
#define OSC_READ_TAG_1 15
#define OSC_READ_TAG_2 16
#define OSC_READ_TAG_3 17
#define OSC_READ_TAG_4 18
#define OSC_READ_TAG_5 19
#define OSC_READ_TAG_6 20
#define OSC_READ_TAG_7 21
#define OSC_READ_TAG_8 22
#define OSC_READ_TAG_9 23
#define OSC_MAX_TAG 32
unsigned long temp;
unsigned char buff[4];
int a = 0;
int mapx=1;

static byte OSC_NEXT = OSC_SLASH; 
static char OSC_DATA[32];

byte OSC_READ_BYTE;
int VALUE_C = 0;
int InputByte = 0;

void setup() {
Serial.begin(57600);	
motor_1.attach(2);
motor_2.attach(3);
motor_3.attach(4);
} 

void loop() 
{
while (Serial.available() > 0) {
InputByte = Serial.read() & 0xFF;
OSC_Handle(InputByte); 
}
}

void OSC_RX(char * msg, unsigned int value) {
int i;
String tagname; 
for (int i=0; i <= 9; i++)
{
 tagname += String (OSC_DATA[i]);
}

int valuex = (byte)(value);
if (tagname.substring(3,8) == "fader") 
{
if (OSC_DATA[8] == 49)
{
 mapx = map(valuex,0,127,10,92); 
 motor_1.write(mapx); // servomotore 1
delay(15);
}
if (OSC_DATA[8] == 50)
{
 mapx = map(valuex,0,127,10,160); 
 motor_2.write(mapx); // servomotore 2
delay(15);
}

if (OSC_DATA[8] == 51)
{
 mapx = map(valuex,0,127,10,160); 
 motor_3.write(mapx); // servomotore 3
delay(15);
}
}

if (tagname.substring(3,9) == "toggle") 
{
  
if (OSC_DATA[9] == 49) motor_1.write(92); //motor 92 
if (OSC_DATA[9] == 50) motor_2.write(90); //motor 90
if (OSC_DATA[9] == 51) motor_3.write(90); //motor 90
}
return;
}


void OSC_Handle(int c) {

  switch(OSC_NEXT) 
  {
  
case OSC_SLASH: // read first slash 
VALUE_C = 0;
    OSC_READ_BYTE = 0;
    if(c==47) {
      OSC_DATA[OSC_READ_BYTE++] = c;
      OSC_NEXT = OSC_PAGE;            
      }
break;

  case OSC_PAGE:
    if(c==49) {
      OSC_DATA[OSC_READ_BYTE++] = c;  // page
      OSC_NEXT = OSC_SLASH2;            
      }
    else
    {
    OSC_NEXT = OSC_SLASH; 
    } 
  
break;
      
  case OSC_SLASH2: // page
    if(c==47) {
      OSC_DATA[OSC_READ_BYTE++] = c;
      OSC_NEXT = OSC_READ_TAG_1; 
      }
  
break;

//***************************************************************
case OSC_READ_TAG_1:
            OSC_DATA[OSC_READ_BYTE++] = c;   // f p t
            OSC_NEXT = OSC_READ_TAG_2;     
 
break;

case OSC_READ_TAG_2:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fa u o 
            OSC_NEXT = OSC_READ_TAG_3;            
 
break;

case OSC_READ_TAG_3:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fad s g 
            OSC_NEXT = OSC_READ_TAG_4;            
break;

case OSC_READ_TAG_4:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fade h g
            OSC_NEXT = OSC_READ_TAG_5;            
break;

case OSC_READ_TAG_5:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fader 1
            OSC_NEXT = OSC_READ_TAG_6;            
break;

case OSC_READ_TAG_6:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fader1
            OSC_NEXT = OSC_READ_TAG_7;
break;

case OSC_READ_TAG_7:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fader10
            OSC_NEXT = OSC_READ_TAG_8;            
break;

case OSC_READ_TAG_8:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fader100
            OSC_NEXT = OSC_READ_TAG_9;            
break;

case OSC_READ_TAG_9:
            OSC_DATA[OSC_READ_BYTE++] = c;   // fader1000
OSC_NEXT = OSC_READ_FOO_1;            

 if (OSC_READ_BYTE > OSC_MAX_TAG)
 {
       OSC_NEXT = OSC_SLASH;
 }
break;
//***************************************************************
case OSC_READ_FOO_1:
            OSC_DATA[OSC_READ_BYTE++] = c;   // ,
OSC_NEXT = OSC_READ_FOO_2;            
break;

case OSC_READ_FOO_2:
            OSC_DATA[OSC_READ_BYTE++] = c;   // ,f
OSC_NEXT = OSC_READ_FOO_3;            
break;

case OSC_READ_FOO_3:
            OSC_DATA[OSC_READ_BYTE++] = c;   // ,f0
OSC_NEXT = OSC_READ_FOO_4;            
break;

case OSC_READ_FOO_4:
            OSC_DATA[OSC_READ_BYTE++] = c;   // ,f00
OSC_NEXT = OSC_READ_VALUE_1;            
break;
//***************************************************************
case OSC_READ_VALUE_1:
      OSC_DATA[OSC_READ_BYTE++] = c; // read val 1
      OSC_NEXT = OSC_READ_VALUE_2; 
      temp=0;
buff[0]=c;
break;

case OSC_READ_VALUE_2:
      OSC_DATA[OSC_READ_BYTE++] = c; //read val 2
      OSC_NEXT = OSC_READ_VALUE_3; 
buff[1]=c;
break;

case OSC_READ_VALUE_3:

      OSC_DATA[OSC_READ_BYTE++] = c; //read val 3
      VALUE_C = c-63;
      OSC_NEXT = OSC_READ_VALUE_4;          
buff[2]=c;
break;


case OSC_READ_VALUE_4:
      OSC_DATA[OSC_READ_BYTE++] = c; //read val 4
      OSC_NEXT = OSC_SLASH;
buff[3]=c;

temp= (unsigned long) buff[3];
temp |=((unsigned long) buff[2]) << 8;
temp |=((unsigned long) buff[1]) << 16;
temp |=((unsigned long) buff[0]) << 24;

union
{
    unsigned long ul;
    float f;
} u;

u.ul = temp; 

a = (byte)u.f;
OSC_RX(OSC_DATA,a);
break;
    
  default:
    OSC_NEXT = OSC_SLASH;
  }
}

