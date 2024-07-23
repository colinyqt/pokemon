/*
 * global.h
 *
 *  Created on: Apr 19, 2024
 *      Author: drewp
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdint.h>
#include "Pokemon.h"

uint32_t Random(uint32_t n);
int getPokemonSDC(char* name);
int getPlayerPokemonSDC(char* name);

extern uint8_t playerState;
extern uint16_t score;
extern uint8_t menuBuffer;
extern uint8_t menuX;
extern uint8_t menuY;
extern uint8_t language;
extern uint16_t PokeBuffer[4096];
extern uint16_t PokeBuffer2[4096];
#endif /* GLOBAL_H_ */
