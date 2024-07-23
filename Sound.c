// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

uint32_t Count;
uint8_t Index;
void SysTick_IntArm(uint32_t period, uint32_t priority){
  // write this
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
    SysTick->CTRL = 0; // reset everything
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000007; // enable
    SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000))| 1 << 30; // set priority to 1




}
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    DAC_Out(effects[Index][Count]);
    Count++;
    if(Count > lengths[Index]){
        SysTick->LOAD = 0;
        Count = 0;
    }
	
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement

void Sound_Button(void){
// write this
    SysTick->LOAD = 7271; // interrupt at 15 Hz
    Index = 1;
    __enable_irq();
}
void Sound_Damage(void){
// write this
  SysTick->LOAD = 7271;
  Index = 0;
}
