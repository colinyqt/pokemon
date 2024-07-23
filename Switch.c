/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){


         IOMUX ->SECCFG.PINCM[PB13INDEX] = 0X00040081;// SW4
         IOMUX ->SECCFG.PINCM[PB19INDEX] = 0X00040081;// SW1
         IOMUX ->SECCFG.PINCM[PB17INDEX] = 0X00040081;// SW 2
         IOMUX ->SECCFG.PINCM[PB16INDEX] = 0X00040081;// SW 3
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
//    int totalSwitch = GPIOB->DIN31_0;
//  return totalSwitch; //replace this your code
}
