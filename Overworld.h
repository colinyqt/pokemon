/*
 * Overworld.h
 *
 *  Created on: Apr 17, 2024
 *      Author: drewp
 */

#ifndef OVERWORLD_H_
#define OVERWORLD_H_

void Overworld(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause);

int8_t canMove(int8_t direction);

uint32_t Random(uint32_t n);

void checkEncounter();

void healPokemon();

void YalePatt();

struct Overworld {
    int x;
    int y;
    int walkFrame;
    int moveCount;
    int WildPokemon;
    int face;
    uint8_t sprite;
    int firstFlag;
    int npcFlag;
    int npcKill;
};

extern struct Overworld wld;

#endif /* OVERWORLD_H_ */
