// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Drew Peries, Colin Tan
// Last Modified: 12/31/2023

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/ADC0.h"
#include "../inc/SPI.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Overworld.h"
#include "Animations.h"
#include "global.h"
#include "battle.h"
#include "integer.h"
#include "diskio.h"
#include "ff.h"

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint8_t playerState = 5;
uint16_t score = 0;
uint8_t menuBuffer = 0;
uint8_t menuX;
uint8_t menuY;
int8_t A, B, pause, menu;
uint8_t language = 1;

uint32_t M=17;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

int8_t direction(uint32_t x, uint32_t y) {
    if (y < 1600 || y > 2500) {
        if (y < 1600) {
            return 2;
        } else {
            return -2;
        }
    } else if (x < 1300 || x > 3000) {
        if  (x < 1600) {
            return -1;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}




uint16_t PokeBuffer[4096];
uint16_t PokeBuffer2[4096];
uint32_t joystickx;
int32_t tempjoy;
int8_t G12Flag = 0;


// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    int ymovement = ADC0xin();
    int xmovement = ADCin();
    tempjoy = direction(xmovement, ymovement);

    // 2) read input switches
    uint32_t totalInput = GPIOB->DIN31_0;

    pause = ((totalInput & (1<<19)) >> 19);
    B = (((totalInput & (1<<16)) >> 16));
    menu = (((totalInput & (1<<17)) >> 17));
    A = (((totalInput & (1<<13)) >> 13));





    // 3) move sprites

    // 4) start sounds
    // 5) set semaphore
    G12Flag = 1;
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}



uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};


// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InvertDisplay(1);
  ADCinit();     //PB18 = ADC1 channel 5, slidepot
  ADC0init();
  DAC_Init();
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  uint8_t cursor = 1;
  uint8_t menuFrame = 0;
  __enable_irq();
  disk_initialize(0);
  ST7735_InitR(INITR_REDTAB);
  ST7735_InvertDisplay(1);


  //ST7735_DrawBitmap(64, 64, PokeBuffer, 64, 64);
  while(1){
    // wait for semaphore
      if (G12Flag == 1) {
          G12Flag = 0;

          switch (playerState) {
              case (0):
                  Overworld(tempjoy, A, B, menu, pause);
              break;
          case (1):
              pauseScreen(tempjoy, A, B, menu, pause, menuFrame, cursor);
              if (menuFrame < 7) {
                  menuFrame++;
              }
              if (playerState != 1) {
                  menuFrame = 0;
              }
              break;
          case(2): //menu
              menuScreen(tempjoy, A, B, menu, pause, menuFrame, cursor);
              menuFrame++;
              if (menuFrame > 23) {
                  menuFrame--;
              }
              if (playerState != 2) {
                  menuFrame = 0;
              }
              break;
          case (3):
              //WILD TRANSITION PHASE
              wildTransition(tempjoy, A, B, menu, pause, menuFrame);
              menuFrame++;
              //if conditional to check if finished
              if(menuFrame > 50) {
                  menuFrame = 49;
              }
              if(playerState != 3) {
                  menuFrame = 0;
              }
              break;
          case (4):
                  //battle phase
                          inBattle(tempjoy, A, B, menu, pause);
                          break;
          case (5): //start screen
               cursor = languageScreen(tempjoy, A, cursor, menuFrame);
               menuFrame++;
               if (playerState != 5) {
                   menuFrame = 0;
               }
               break;

          case (6): //starter selection screen
               characterScreen(tempjoy, A, B, cursor, menuFrame);
               menuFrame++;
               if (menuFrame > 12) {
                   menuFrame--;
               }
               if (playerState != 6) {
                   menuFrame = 0;
               }
               break;
          case (7):
                endScreen(tempjoy, A, B, cursor, menuFrame);
                  menuFrame++;
                  if (menuFrame > 2) {
                      menuFrame--;
                  }
                  break;

        }
          joystickx++;
      }
  }
}
