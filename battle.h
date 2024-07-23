/*
 * battle.h
 *
 *  Created on: Apr 20, 2024
 *      Author: drewp
 */

#ifndef BATTLE_H_
#define BATTLE_H_

#include "Pokemon.h"

struct battle {
    uint8_t isWild; // check if you can catch
    uint8_t ScreenFlag; // check which screen +
    uint8_t cursor;
    uint8_t cursorBuffer;
    uint8_t Abuffer;
    uint8_t animFrame;
    uint8_t faster;
    uint8_t fainted;
    uint8_t battleIter;
    struct Move playerMove;
    uint8_t enemyMove;
    uint8_t WinLose;
    uint8_t enemyFainted;
    struct Pokemon enemy[6];
    uint8_t playerIndex;
    uint8_t enemyIndex;
};

extern struct battle battleInfo;

void inBattle(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause);
int attackPokemon(struct Pokemon pokemon1, struct Pokemon pokemon2, int moveIndex) ;
uint8_t enemyPokemonFainted();
uint8_t playerPokemonFainted();

#endif /* BATTLE_H_ */
