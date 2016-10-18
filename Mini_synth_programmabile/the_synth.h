#include <math.h>
#include <MIDI.h>

#include "avr/interrupt.h"
#include "avr/pgmspace.h"
#include "sin256.h"
#include "ramp256.h"
#include "saw256.h"
#include "square256.h"
#include "noise256.h"
#include "tria256.h"
#include "env0.h"
#include "env1.h"
#include "env2.h"
#include "env3.h"

#define FS 16000.0                                             //-Sample rate
#define SET(x,y) (x |=(1<<y))		        		//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       			// |
#define CHK(x,y) (x & (1<<y))           			// |
#define TOG(x,y) (x^=(1<<y))            			//-+
unsigned int EFTWS[128];
unsigned int PITCHS[128];
unsigned int FTWS[128];
byte midi_1st=0;
byte midi_2nd=0;
char shape = 0;

uint16_t smidiToFreq[512] PROGMEM = { 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 28, 28, 28, 29, 29, 30, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 41, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 48, 49, 50, 50, 51, 52, 53, 53, 54, 55, 56, 57, 57, 58, 59, 60, 61, 62, 63, 64, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 82, 84, 85, 86, 87, 89, 90, 91, 93, 94, 95, 97, 98, 99, 101, 102, 104, 105, 107, 108, 110, 112, 113, 115, 117, 118, 120, 122, 123, 125, 127, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151, 153, 156, 158, 160, 162, 165, 167, 170, 172, 175, 177, 180, 182, 185, 188, 190, 193, 196, 199, 202, 205, 208, 211, 214, 217, 220, 223, 226, 230, 233, 237, 240, 243, 247, 251, 254, 258, 262, 265, 269, 273, 277, 281, 285, 290, 294, 298, 302, 307, 311, 316, 320, 325, 330, 334, 339, 344, 349, 354, 360, 365, 370, 375, 381, 386, 392, 398, 404, 409, 415, 421, 428, 434, 440, 446, 453, 460, 466, 473, 480, 487, 494, 501, 508, 516, 523, 531, 539, 547, 554, 563, 571, 579, 587, 596, 605, 613, 622, 631, 641, 650, 659, 669, 679, 689, 699, 709, 719, 730, 740, 751, 762, 773, 784, 796, 807, 819, 831, 843, 855, 868, 880, 893, 906, 919, 933, 946, 960, 974, 988, 1002, 1017, 1032, 1047, 1062, 1077, 1093, 1109, 1125, 1141, 1158, 1175, 1192, 1209, 1227, 1245, 1263, 1281, 1300, 1319, 1338, 1357, 1377, 1397, 1418, 1438, 1459, 1480, 1502, 1524, 1546, 1568, 1591, 1614, 1638, 1662, 1686, 1710, 1735, 1760, 1786, 1812, 1838, 1865, 1892, 1920, 1948, 1976, 2005, 2034, 2063, 2093, 2124, 2155, 2186, 2218, 2250, 2283, 2316, 2350, 2384, 2419, 2454, 2490, 2526, 2562, 2600, 2638, 2676, 2715, 2754, 2794, 2835, 2876, 2918, 2961, 3004, 3047, 3092, 3137, 3182, 3228, 3275, 3323, 3371, 3420, 3470, 3521, 3572, 3624, 3677, 3730, 3784, 3839, 3895, 3952, 4009, 4068, 4127, 4187, 4248, 4310, 4372, 4436, 4500, 4566, 4632, 4700, 4768, 4837, 4908, 4979, 5051, 5125, 5199, 5275, 5352, 5430, 5509, 5589, 5670, 5753, 5836, 5921, 6007, 6095, 6183, 6273, 6364, 6457, 6551, 6646, 6743, 6841, 6940, 7041, 7144, 7248, 7353, 7460, 7569, 7679, 7790, 7904, 8019, 8135, 8254, 8374, 8495, 8619, 8744, 8872, 9001, 9132, 9264, 9399, 9536, 9675, 9815, 9958, 10103, 10250, 10399, 10550, 10704, 10859, 11017, 11178, 11340, 11505, 11672, 11842, 12014, 12189, 12366, 12546, 12729, 12914, 13102, 13292, 13486, 13682, 13881, 14083, 14288, 14495, 14706, 14920, 15137, 15357, 15581, 15807, 16037, 16271, 16507, 16747, 16991, 17238, 17489, 17743, 18001, 18263, 18529, 18798, 19072, 19349, 19631, 19916, 20206, 20500, 20798, 21100, 21407, 21719, 22035, 22355, 22680, 23010, 23345, 23684, 24029, 24378, 24733, 25093, 25458, 25828, 26204, 26585, 26971, 27364 };
unsigned short scaling[16] = { 1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 10, 20 };


unsigned char decay = 7;

int8_t sineTable[256]=
{
  0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 59, 62, 65, 67, 70, 73, 75, 78, 80, 82, 85, 87, 89, 91, 94, 96, 98, 100, 102, 103, 105, 107, 108, 110, 112, 113, 114, 116, 117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126, 127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 114, 113, 112, 110, 108, 107, 105, 103, 102, 100, 98, 96, 94, 91, 89, 87, 85, 82, 80, 78, 75, 73, 70, 67, 65, 62, 59, 57, 54, 51, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45, -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87, -89, -91, -94, -96, -98, -100, -102, -103, -105, -107, -108, -110, -112, -113, -114, -116, -117, -118, -119, -120, -121, -122, -123, -123, -124, -125, -125, -126, -126, -126, -126, -126, -127, -126, -126, -126, -126, -126, -125, -125, -124, -123, -123, -122, -121, -120, -119, -118, -117, -116, -114, -113, -112, -110, -108, -107, -105, -103, -102, -100, -98, -96, -94, -91, -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51, -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9, -6, -3,   
};

uint8_t expTable[256] PROGMEM =
{
  255, 250, 244, 239, 234, 229, 224, 219, 214, 210, 205, 201, 196, 192, 188, 184, 180, 176, 172, 169, 165, 162, 158, 155, 151, 148, 145, 142, 139, 136, 133, 130, 
  127, 124, 122, 119, 117, 114, 112, 109, 107, 105, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 81, 79, 77, 76, 74, 72, 71, 69, 68, 66, 65, 63, 62, 61, 59, 58, 
  57, 56, 55, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 39, 38, 37, 36, 35, 35, 34, 33, 32, 32, 31, 30, 30, 29, 28, 28, 27, 27, 26, 25, 25, 24, 
  24, 23, 23, 22, 22, 21, 21, 21, 20, 20, 19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 
  10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
};



volatile unsigned int PCW[4]={0,0,0,0};				//-Wave phase accumulators
volatile unsigned int FTW[4]={1000,200,300,400};              	//-Wave frequency tuning words
volatile unsigned char AMP[4]={255,255,255,255};                //-Wave amplitudes [0-255]
volatile unsigned int PITCH[4]={500,500,500,500};               //-Voice pitch
volatile int MOD[4]={20,0,64,127};                             	//-Voice envelope modulation [0-1023 512=no mod. <512 pitch down >512 pitch up]
volatile unsigned int wavs[4];                                  //-Wave table selector [address of wave in memory]
volatile unsigned int envs[4];                                  //-Envelopte selector [address of envelope in memory]
volatile unsigned int EPCW[4]={0x8000,0x8000,0x8000,0x8000};    //-Envelope phase accumulator
volatile unsigned int EFTW[4]={10,10,10,10};                    //-Envelope speed tuning word
volatile unsigned int tim=0;                                    //-Sample counter eg. for sequencer
volatile unsigned char divider=4;                               //-Sample rate decimator for envelope
volatile unsigned char tick=0;
volatile unsigned char envg=0;




void genSine ()
{
  shape = 0;
  for (int j = 0; j < 256; ++j)
  {
   sineTable[j] = sin(j/256.*6.2831)*127;
  }
}




void genSquare ()
{
  for (int j = 0; j < 256; ++j)
  {
    sineTable[j] = j<128?-128:127;
  }
}




void genSaw ()
{
  for (int j = 0; j < 256; ++j)
  {
    sineTable[j] = (255-j)-128;
  }
}


class soundWav
{
  public:
  
    void init (uint16_t msDecay)
    {
        m_envPos = 0;
        m_envDelta = 2000 / msDecay;
        m_delta = 0;
        m_phase = 0;
        m_volume = 0;
        m_off = true;
    }
    
    void setDecay (uint16_t msDecay)
    {
      m_envDelta = 2000 / msDecay;
    }
  
    void trigger (uint8_t note, uint8_t velo)
    {


//m_volume = velo;
      unsigned char* noteAddress = (unsigned char*)smidiToFreq;
      volatile uint16_t t;
      noteAddress += ((uint16_t)note) << 3;
      ((unsigned char*)&t)[0] = pgm_read_byte( noteAddress+0 );
      ((unsigned char*)&t)[1] = pgm_read_byte( noteAddress+1 );
      m_delta = t;
      //m_delta = pgm_read_word(&smidiToFreq[(uint16_t)note<<4]);
      m_envPos = ((127-velo)<<8);
   //   m_envDelta = scaling[note>>3]*(9-decay);
      m_envVal = pgm_read_byte(&expTable[127-velo]);
      m_off = false;
    }
  
    int8_t render ()
    {
      if (m_off) return 0;
      
      uint8_t phase = m_phase >> 8;
  #if USE_INTERPOLATION
      uint8_t fract = m_phase & 0xFF;
        
      int16_t a = sineTable[phase];      
      int16_t b = sineTable[(phase+1)&0xFF];
        
      int8_t sound = a + (((b-a)*fract)>>8);
   #else
     int8_t sound = sineTable[phase];
  #endif

      sound = (sound*m_envVal) >> 8;      
      m_phase += m_delta;
     
      return sound;
    }
    
    void updateEnvelope ()
    {
      if (!m_off)
      {
        uint8_t envpos = m_envPos >> 8;
        m_envVal = pgm_read_byte(expTable+envpos);
        m_off = (envpos > 250);
        m_envPos += m_envDelta;
      }
    }
    
  private:
  
    uint16_t m_phase;
    uint16_t m_delta;
    uint16_t m_envPos;
    uint16_t m_envDelta;
    uint8_t m_volume;
    uint8_t m_envVal;
    bool m_off;    
};

soundWav wav[5];
uint8_t voiceToPlay = 0;

uint8_t sp = 0;

int16_t rendersoundWav()
{
  int u = sp&7;
  if (u < 5)
  wav[u].updateEnvelope();
  ++sp;
  return wav[0].render() + wav[1].render() + wav[2].render() + wav[3].render() + wav[4].render() + 128;
}



void mtrigger(unsigned char voice,unsigned char note)
{
//  PITCH[voice]=(440. * exp(.057762265 * (note - 69.)))/(FS/65535.0); //[MIDI note]
  PITCH[voice]=PITCHS[note];
  EPCW[voice]=0;
  FTW[voice]=PITCH[voice]+(PITCH[voice]*(EPCW[voice]/(32767.5*128.0  ))*((int)MOD[voice]-512));
}



unsigned char synthTick(void)
{
  tick!=tick;
  return tick;
}

ISR(TIMER1_COMPA_vect)
{
   if (MIDI.read())
    {
    if (MIDI.getType() == NoteOn )
    {
      midi_1st = MIDI.getData1();
      midi_2nd = MIDI.getData2();
//    mtrigger(0,midi_1st);
     if (midi_2nd >0) 
     wav[voiceToPlay].trigger(midi_1st,midi_2nd);
  //voiceToPlay = (voiceToPlay+1)&3;  
     if (++voiceToPlay == 5) voiceToPlay = 0;
    }
     }
   int mix = rendersoundWav();

  if (mix > 255) mix = 255;
  else
  if (mix < 0) mix = 0;
  
	if(divider)
	{
	divider--;
    }
    else
    {

  	// Volume envelope generator
	divider=4;
    if(!(EPCW[0]&0x8000))
    {
    	AMP[0]=pgm_read_byte(envs[0]+ ((EPCW[0]+=EFTW[0])>>7) );
     	if(EPCW[0]&0x8000)
          AMP[0]=0;
    }
    else
    	AMP[0]=0;

      if(!(EPCW[1]&0x8000))
      {
        AMP[1]=pgm_read_byte(envs[1]+ ((EPCW[1]+=EFTW[1])>>7) );
        if(EPCW[1]&0x8000)
          AMP[1]=0;
      }
      else
        AMP[1]=0;

      if(!(EPCW[2]&0x8000))
      {
        AMP[2]=pgm_read_byte(envs[2]+ ((EPCW[2]+=EFTW[2])>>7) );
        if(EPCW[2]&0x8000)
          AMP[2]=0;
      }
      else
        AMP[2]=0;

      if(!(EPCW[3]&0x8000))
      {
        AMP[3]=pgm_read_byte(envs[3]+ ((EPCW[3]+=EFTW[3])>>7) );
        if(EPCW[3]&0x8000)
          AMP[3]=0;
      }
      else
        AMP[3]=0;
    }


  //  Synthesizer/audio mixer
  OCR2B=127+
  	((
  	(((signed char)pgm_read_byte(wavs[0]+((PCW[0]+=FTW[0])>>8))*AMP[0])>>8)+
  	(((signed char)pgm_read_byte(wavs[1]+((PCW[1]+=FTW[1])>>8))*AMP[1])>>8)+
  	(((signed char)pgm_read_byte(wavs[2]+((PCW[2]+=FTW[2])>>8))*AMP[2])>>8)+
  	(((signed char)pgm_read_byte(wavs[3]+((PCW[3]+=FTW[3])>>8))*AMP[3])>>8)+mix)>>2);  
    
    tim++;

   //  Modulation engine
   if(tim>FS/20)
    {
      switch(envg)
      {
        case 0:
        {
          FTW[0]=PITCH[0]+(PITCH[0]*(EPCW[0]/(32767.5*128.0  ))*((int)MOD[0]-512));
          envg++;
        };break;
        case 1:
        {
          FTW[1]=PITCH[1]+(PITCH[1]*(EPCW[1]/(32767.5*128.0  ))*((int)MOD[1]-512));
          envg++;
        };break;
        case 2:
        {
          FTW[2]=PITCH[2]+(PITCH[2]*(EPCW[2]/(32767.5*128.0  ))*((int)MOD[2]-512));
          envg++;
        };break;
        case 3:
        {
          FTW[3]=PITCH[3]+(PITCH[3]*(EPCW[3]/(32767.5*128.0  ))*((int)MOD[3]-512));
          envg++;
        };break;
        case 4:
        {
          tim=0;
          envg=0;
          tick=1;
        };break;
      }
    }
}







//  Setup all voice parameters
void setup_voice(unsigned char voice,unsigned int waveform, float pitch, unsigned int envelope, float length, unsigned int mod)
{
  wavs[voice]=waveform;//[address in program memory]
  envs[voice]=envelope;//[address in program memory]
  EFTW[voice]=(1.0/length)/(FS/(32767.5*10.0));//[s];
  PITCH[voice]=pitch/(FS/65535.0); //[Hz]
  MOD[voice]=mod;//0-1023 512=no mod
}



//  Setup wave
void setup_wave(unsigned char voice,unsigned char wave)
{
	switch(wave/16)
	{
		case 1:	wavs[voice]=(unsigned int)SinTable;break;
		case 2:	wavs[voice]=(unsigned int)SinTable;break;
		case 3:	wavs[voice]=(unsigned int)SinTable;break;
		case 4:	wavs[voice]=(unsigned int)SinTable;break;
		case 5:	wavs[voice]=(unsigned int)SinTable;break;
		case 6:	wavs[voice]=(unsigned int)NoiseTable;break;
		default:	wavs[voice]=(unsigned int)sineTable;break;
	}
}



//  Setup Envelope
void setup_env(unsigned char voice,unsigned char env)
{
	switch(env/16)
	{
		case 1:	envs[voice]=(unsigned int)Env0;break;
		case 2:	envs[voice]=(unsigned int)Env1;break;
		case 3:	envs[voice]=(unsigned int)Env2;break;
		case 4:	envs[voice]=(unsigned int)Env3;break;
		default:	envs[voice]=(unsigned int)Env0;break;
	}
}
//  Setup Length
void setup_length(unsigned char voice,unsigned char length)
{
	EFTW[voice]=EFTWS[length];
//  EFTW[voice]=(1.0/exp(.057762265 * (length - 69.)))/(FS/(32767.5*10.0));//[s];

}



//  Setup mod
void setup_mod(unsigned char voice,unsigned char mod)
{
  MOD[voice]=mod*8;//0-1023 512=no mod
}

void trigger(unsigned char voice)
{
  EPCW[voice]=0;
}






//  Init synth
void initSynth()
{
 genSquare();
// genSine();
// genSaw();

  wav[0].init(30);
  wav[1].init(30);  
  wav[2].init(30);  
  wav[3].init(30);  
  wav[4].init(30);   

  for(unsigned char i=0;i<128;i++)
  {
    EFTWS[i]=(1.0/exp(.057762265 * (i - 69.)))/(FS/(32767.5*10.0));//[s];
    PITCHS[i]=(440. * exp(.057762265 * (i - 69.)))/(FS/65535.0);
 // FTWS[voice]=PITCH[voice]+(PITCH[voice]*(EPCW[voice]/(32767.5*128.0  ))*((int)MOD[voice]-512));
  }

  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);

  TCCR2A = (TCCR2A | _BV(COM2B1)) & ~_BV(COM2B0);
  TCCR2B &= ~(_BV(COM2B1) | _BV(COM2B0));
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

  OCR2A = 0;
  cli();
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  OCR1A = 1536;
  TIMSK1 |= _BV(OCIE1A);
  sei();

}






