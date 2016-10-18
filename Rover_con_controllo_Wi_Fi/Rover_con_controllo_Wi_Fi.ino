/*
Sketch per il controllo Wi-Fi e protocollo OSC di un rover a tre motori DC.
Pier Calderan - Luglio 2013
*/
#define SERIAL_SPEED 57600
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

int DigitalPin2 = 2; 
int DigitalPin3 = 3;
int DigitalPin4 = 4;
int DigitalPin5 = 5;
int DigitalPin6 = 6;
int DigitalPin7 = 7;
int DigitalPin8 = 8;
int DigitalPin9 = 9;
int DigitalPin10 = 10;
int DigitalPin11 = 11;
int DigitalPin12 = 12;
int DigitalPin13 = 13;


void setup() {

Serial.begin(SERIAL_SPEED);	
pinMode (DigitalPin2,OUTPUT); 
pinMode (DigitalPin3,OUTPUT); 
pinMode (DigitalPin4,OUTPUT); 
pinMode (DigitalPin5,OUTPUT); 
pinMode (DigitalPin6,OUTPUT); 
pinMode (DigitalPin7,OUTPUT); 
pinMode (DigitalPin8,OUTPUT); 
pinMode (DigitalPin9,OUTPUT); 
pinMode (DigitalPin10,OUTPUT); 
pinMode (DigitalPin11,OUTPUT); 
pinMode (DigitalPin12,OUTPUT); 
pinMode (DigitalPin13,OUTPUT); 
}


void loop() 
{
    while (Serial.available() > 0) {
      InputByte = Serial.read() & 0xFF;
    OSC_Handle(InputByte); 
}
}


void OSC_RX(char * msg, unsigned int value)
{
 int i;
 String tagname; 

 for (int i=0; i <= 9; i++)
 {
  tagname += String (OSC_DATA[i]);
 }

int valuex = (byte)(value);

if (tagname.substring(3,8) == "fader") 
{
if (OSC_DATA[8] == 49) analogWrite(DigitalPin3,valuex); //led1
if (OSC_DATA[8] == 50) analogWrite(DigitalPin5,valuex); //led2 
if (OSC_DATA[8] == 51) analogWrite(DigitalPin6,valuex); //led3


if (value < 128 )
{
digitalWrite(DigitalPin12,LOW);
digitalWrite(DigitalPin8,HIGH);
digitalWrite(DigitalPin13,LOW);
mapx = map(valuex,0,127,0,255);
if (OSC_DATA[8] == 52) analogWrite(DigitalPin10,mapx); //motore dx
if (OSC_DATA[8] == 53) analogWrite(DigitalPin11,mapx); //motore sx
if (OSC_DATA[8] == 54) analogWrite(DigitalPin9,mapx); //motore centrale
}
if (value > 127 )
{
digitalWrite(DigitalPin12,HIGH); //motore dx
digitalWrite(DigitalPin8,LOW); //motore sx
digitalWrite(DigitalPin13,HIGH); //motore centrale
mapx = map(valuex,128,255,0,255);
if (OSC_DATA[8] == 52) analogWrite(DigitalPin10,mapx); 
if (OSC_DATA[8] == 53) analogWrite(DigitalPin11,mapx); 
if (OSC_DATA[8] == 54) analogWrite(DigitalPin9,mapx); 
}



}

if (tagname.substring(3,9) == "toggle") 
{
  
if (OSC_DATA[9] == 49) digitalWrite(DigitalPin3,valuex); //led1 on/off 
if (OSC_DATA[9] == 50) digitalWrite(DigitalPin5,valuex); //led2 on/off
if (OSC_DATA[9] == 51) digitalWrite(DigitalPin6,valuex); //led3 on/off

if (OSC_DATA[9] == 52) digitalWrite(DigitalPin4,valuex); //motore dx
if (OSC_DATA[9] == 53) digitalWrite(DigitalPin7,valuex); //motore sx
if (OSC_DATA[9] == 54) digitalWrite(DigitalPin2,valuex); //motore centrale
}

Serial.println(mapx);
     
     
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

