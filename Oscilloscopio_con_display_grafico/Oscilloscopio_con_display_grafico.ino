/*
Sketch di un oscilloscopio con LCD grafico 128x64 pixel.
Pier Calderan - Luglio 2013
*/
#include <glcd.h>
#include "fonts/allFonts.h"
#include "bitmaps/allBitmaps.h"

#define txtLINE0   0
#define txtLINE1   16
#define txtLINE2   30
#define txtLINE3   46


const int LCD_WIDTH = 128;
const int LCD_HEIGHT = 64;
const int SAMPLES = 100;
const int menu_switch = 3;                    // Digital 3 pin for switches
const int channel = 5;                   // Analog 4 pin for channel 0
const int VREF[] = { 512, 1024, 2048, 4096};
// reference voltage 5.0V ->  50 :   1V/div range (100mV/dot)
//                        -> 100 : 0.5V/div
//                        -> 250 : 0.2V/div
//                        -> 500 : 100mV/div
//                       -> 1000 :  50mV/div
//const int MILLIVOL_per_dot[] = {  100, 50, 20, 10, 5}; // mV/dot
const int MODE_ON = 0;
const int MODE_INV = 1;
const int MODE_OFF = 2;
const char *Modes[] = {  "ON", "INV", "OFF"};
const int TRIG_AUTO = 0;
const int TRIG_NORM = 1;
const int TRIG_SCAN = 2;
const int TRIG_ONE  = 3;
const char *TRIG_Modes[] = {  "Auto", "Norm", "Scan", "One"};
const int TRIG_E_UP = 0;
const int TRIG_E_DN = 1;
#define RATE_MIN 0
#define RATE_MAX 13
const char *Rates[] = {  "F1-1", "F1-2 ", "F2  ", "5ms", "10ms", "20ms", "50ms", "0.1s", "0.2s", "0.5s", "1s", "2s", "5s", "10s"};
#define RANGE_MIN 0
#define RANGE_MAX 4
const char *Ranges[] = {  " 1V ", "0.5V", "0.2V", "0.1V", "50mV"};
byte range0 = RANGE_MIN;
byte range1 = RANGE_MAX;
byte channel_mode = 0; 

byte rate = 3;
byte trig_mode = TRIG_AUTO;
byte trig_lv = 30;
byte trig_edge = TRIG_E_UP;
byte trig_ch = 0;
byte Start = 1;
byte menu = 0;
short channel_offset = 127;

unsigned long startMillis;
byte data[4][SAMPLES];
byte sample = 0;

void setup()
{
GLCD.Init(NON_INVERTED);
GLCD.SelectFont(System5x7);
pinMode(3, INPUT);
  GLCD.ClearScreen();
  DrawGrid();
  DrawText();
}

void CheckSW() 
{
  static unsigned short oain[2];
  static unsigned long Millis = 0, oMillis = 0;
  unsigned long ms;

  unsigned short ain = 0; //digitalRead(menu_switch);
  ms = millis();
  if ((ms - Millis)<5)
    return;
  Millis = ms;

  if (!(abs(oain[0] - oain[1])>10 && abs(oain[1] - ain)<2)) 
  {
    oain[0] = oain[1];
    oain[1] = ain;
    return;
  }
  oain[0] = oain[1];
  oain[1] = ain;

  if (ain > 950 || (Millis - oMillis)<200)
    return;
  oMillis = Millis;

  int sw;
  for (sw = 0; sw < 10; sw ++) 
  {
    const int sw_lv[] = {
      889, 800, 700, 611, 514, 419, 338, 231, 132, 70    };
    if (ain > sw_lv[sw])
      break;
  }

  switch (menu) 
  {
  case 0:
  default:
    menu0_sw(sw); 
    break;
  case 1:
    menu1_sw(sw); 
    break;
  case 2:
    menu2_sw(sw); 
    break;
  }

  DrawText();
}

void menu0_sw(int sw) 
{  
  switch (sw) {
  case 0:
    // START/HOLD
    if (Start)
      Start = 0;
    else
      Start = 1;
    break;
  case 1:
    // CH0 RANGE -
    if (range0 < RANGE_MAX)
      range0 ++;
    break;
  case 2:

    if (range1 < RANGE_MAX)
      range1 ++;
    break;
  case 3:
    // RATE FAST
    if (rate > 0)
      rate --;
    break;
  case 4:
    // TRIG MODE
    if (trig_mode < TRIG_ONE)
      trig_mode ++;
    else
      trig_mode = 0;
    break;
  case 5:
    // SEND
//    SendData();
    break;
  case 6:
    // TRIG MODE
    if (trig_mode > 0)
      trig_mode --;
    else
      trig_mode = TRIG_ONE;
    break;
  case 7:
    // RATE SLOW
    if (rate < RATE_MAX)
      rate ++;
    break;
  case 8:

    if (range1 > 0)
      range1 --;
    break;
  case 9:
    // CH0 RANGE +
    if (range0 > 0)
      range0 --;
    break;
  case 10:
  default:
    // MENU SW
    menu ++;
    break;
  }
}

void menu1_sw(int sw) {  
  switch (sw) {
  case 0:
    // START/HOLD
    if (Start)
      Start = 0;
    else
      Start = 1;
    break;
  case 1:
    // CH0 offset +
    if (channel_offset < 1023)
      channel_offset += 1024/VREF[range0];
    break;
  case 2:
    break;
  case 3:
    // trigger level +
    if (trig_lv < 60)
      trig_lv ++;
    break;
  case 4:
  case 6:
    // TRIG EDGE
    if (trig_edge == TRIG_E_UP)
      trig_edge = TRIG_E_DN;
    else
      trig_edge = TRIG_E_UP;
    break;
  case 7:
    // trigger level -
    if (trig_lv > 0)
      trig_lv --;
    break;
  case 8:
    break;
  case 9:
    // CH0 OFF -
    if (channel_offset > -1023)
      channel_offset -= 1024/VREF[range0];
    break;
  case 10:
  default:
    // MENU SW
    menu ++;
    break;
  }
}

void menu2_sw(int sw) {  
  switch (sw) {
  case 0:
    // START/HOLD
    if (Start)
      Start = 0;
    else
      Start = 1;
    break;
  case 1:
    if (channel_mode < 2)
      channel_mode ++;
    break;
  case 2:
    break;
  case 3:
  case 7:
    // TRIG channel
    if (trig_ch == 0)
      trig_ch = 1;
    else
      trig_ch = 0;
    break;
  case 8:
    break;
  case 9:
    if (channel_mode > 0)
      channel_mode --;
    break;
  case 10:
    // MENU SW
    menu = 0;
    break;
  case 4:
  case 6:
  default:
    // none
    break;
  }
}

void DrawGrid() 
{
  for (int x=0; x<=SAMPLES; x += 2) { // Linee orizzontali
    for (int y=0; y<=64; y += 10) {
      GLCD.SetDot(x, y, BLACK);
      CheckSW();
    }
  }
  for (int x=0; x<=SAMPLES; x += 10 ) { // Linee verticali
    for (int y=0; y<=64; y += 2) {
      GLCD.SetDot(x, y, BLACK);
      CheckSW();
    }
  }
}

void DrawText() 
{
  GLCD.FillRect(101,txtLINE0,28,64, WHITE);  // Area testo 

  switch (menu) 
  {
  case 0:
    GLCD.GotoXY(SAMPLES + 3,txtLINE0);         // posizione del cursore
    GLCD.Puts(Ranges[range0]); 
    GLCD.GotoXY(SAMPLES + 3,txtLINE1);         // posizione del cursore
    GLCD.Puts(Ranges[range1]); 
    GLCD.GotoXY(SAMPLES + 3,txtLINE2);         // posizione del cursore
    GLCD.Puts(Rates[rate]); 
    GLCD.GotoXY(SAMPLES + 3,txtLINE3);         // posizione del cursore
    GLCD.Puts(TRIG_Modes[trig_mode]); 
    break;
  case 1:
    GLCD.GotoXY(SAMPLES + 3,txtLINE0);         // posizione del cursore
    GLCD.Puts("OF1"); 
    GLCD.GotoXY(SAMPLES + 3,txtLINE1);         // posizione del cursore
    GLCD.Puts("OF2");
    GLCD.GotoXY(SAMPLES + 3,txtLINE2);         // posizione del cursore
    GLCD.Puts("Tlv"); 
    GLCD.GotoXY(SAMPLES + 3,txtLINE3);         // posizione del cursore
    GLCD.Puts(trig_edge == TRIG_E_UP ? "UP" : "DN"); 
    break;
  case 2:
    GLCD.GotoXY(SAMPLES + 3,txtLINE0);         // posizione del cursore
    GLCD.Puts(Modes[channel_mode]); 
    GLCD.GotoXY(SAMPLES + 3,txtLINE1);         // posizione del cursore

    GLCD.GotoXY(SAMPLES + 3,txtLINE2);         // posizione del cursore
    GLCD.Puts(trig_ch == 0 ? "T:1" : "T:2"); 
    break;
  }
}

//********************************************************
void DrawGraph() 
{
  int clear = 0;

  if (sample == 0)    clear = 1;



#if 0
  for (int x=0; x<SAMPLES; x++) 
  {
    GLCD.DrawLine(x, 40-data[clear][x], x+1, 40-data[clear][x+1], WHITE);
    GLCD.DrawLine(x, 40-data[sample][x], x+1, 40-data[sample][x+1], BLACK);
  }
#else
  for (int x=0; x<(SAMPLES-1); x++) 
  {
    GLCD.DrawLine(x, 40-data[clear][x], x+1, 40-data[clear][x+1], WHITE);
    GLCD.DrawLine(x, 40-data[sample][x], x+1, 40-data[sample][x+1], BLACK);
    CheckSW();
  }  
#endif


}
//********************************************************

inline unsigned long channelRead(byte ch, byte mode, int off)
{
int a = analogRead(ch);

a = map(a,0,VREF[1],0,255);

return a;
}

void  loop() 
{
  if (trig_mode != TRIG_SCAN) 
  {
    unsigned long st = millis();
    byte oad;
    if (trig_ch == 0)
      oad = channelRead(channel, channel_mode, channel_offset);

    for (;;) 
    {
      byte adc;
      if (trig_ch == 0)
        adc = channelRead(channel, channel_mode, channel_offset);

      if (trig_edge == TRIG_E_UP) 
      {
        if (adc >= trig_lv && adc > oad)
          break; 
      } 
      else 
      {
        if (adc <= trig_lv && adc < oad)
          break; 
      }
      oad = adc;

      CheckSW();
      if (trig_mode == TRIG_SCAN)
        break;
      if (trig_mode == TRIG_AUTO && (millis() - st) > 100)
        break; 
    }
  }


  if (rate <= 5 && Start) 
  {
    if (sample == 0)
      sample = 1;
    else
      sample = 0;

    if (rate >= 3 && rate <= 5) 
    { 
      const unsigned long r_[] ={500, 1000, 2000};
      unsigned long st = micros();
      unsigned long r = r_[rate - 3];
 
      for (int i=0; i<SAMPLES; i ++) 
      {
        while((st - micros())<r);
        st += r;
        data[sample][i] = channelRead(channel, channel_mode, channel_offset);
     }

    }
    DrawGraph();
    CheckSW();
    DrawGrid();
    DrawText();
  } 
  
  else 
  {
    CheckSW();
  }
  if (trig_mode == TRIG_ONE)
    Start = 0;
}

