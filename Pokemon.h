/*
 * Pokemon.h
 *
 *  Created on: Apr 20, 2024
 *      Author: drewp
 */

#ifndef POKEMON_H_
#define POKEMON_H_

#include <stdint.h>

typedef enum {
    TYPE_NORMAL,    //index 0
    TYPE_FIGHTING,  //index 1
    TYPE_FLYING,    //index 2
    TYPE_POISON,    //index 3
    TYPE_GROUND,    //index 4
    TYPE_ROCK,      //index 5
    TYPE_BUG,       //index 6
    TYPE_GHOST,     //index 7
    TYPE_STEEL,     //index 8
    TYPE_FIRE,      //index 9
    TYPE_WATER,     //index 10
    TYPE_GRASS,     //index 11
    TYPE_ELECTRIC,  //index 12
    TYPE_PSYCHIC,   //index 13
    TYPE_ICE,       //index 14
    TYPE_DRAGON,    //index 15
    TYPE_DARK,      //index 16
    TYPE_FAIRY,      //index 17
    NUM_TYPES
} Type;

struct Move {
    const char* name;
    uint16_t damage;
    Type type;
};


struct Pokemon {
    const char* name;
    int16_t hp;
    int16_t attack;
    int16_t defense;
    int16_t speed;
    int16_t specialAttack;
    int16_t specialDefense;
    uint16_t level;
    struct Move Moves[4];
    uint8_t index;
    uint16_t* sprite;
    uint16_t* backSprite;
    Type types;
    char* file;
    char* backfile;
};

extern struct Pokemon player[6];
extern uint8_t playerPokemonIndex;
extern struct Pokemon snorlax;
extern struct Pokemon mudkip;
extern struct Pokemon treecko;
extern struct Pokemon torchic;
extern struct Pokemon meowth;
extern struct Pokemon ampharos;
extern struct Pokemon cubchoo;
extern struct Pokemon wobbuffet;
extern struct Pokemon rayquaza;
extern uint8_t playerFainted[6];
extern uint8_t playerAlive;
extern uint8_t playerDead;
extern uint8_t totalPokemon;
extern struct Pokemon allPokemon[9];





//

//
//
//
//
//Pokemon createPokemon(char* name, int hp, int attack, int defense, int speed, int specialAttack, int specialDefense, int type, Move moves[4], int index, const uint16_t* sprite);
////void drawPokemonSprite(Pokemon* pokemon, int16_t x, int16_t y, uint16_t transparentColor);
////int16_t attackPokemon(Pokemon* attacker, Pokemon* defender, int moveIndex);
////bool isPokemonFainted(Pokemon* pokemon);
////void printPokemonStats(Pokemon* pokemon);
////const char* getMoveName(int moveIndex);

#endif /* POKEMON_H_ */
