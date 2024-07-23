/*
 * Pokemon.C
 *
 *  Created on: Apr 20, 2024
 *      Author: Colin Tan
 *
 *      This holds all of the Data for pokemon
 *      Struct containing stats, yadda yadda, and sprite info
 */


#include "Pokemon.h"
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "SmallFont.h"
#include "images/images.h"
#include "Animations.h"
#include "global.h"


//update with all pokemon
Move allMoves[] = {
    {"Tackle", 40, TYPE_NORMAL}, //0
    {"Shock", 90, TYPE_ELECTRIC}, //1
    {"Ember", 90, TYPE_FIRE},    //2
    {"Freeze", 90, TYPE_ICE},    //3
    {"Dig", 100, TYPE_GROUND},   //4
    {"Psychic", 90, TYPE_PSYCHIC}, //5
    {"Surf", 110, TYPE_WATER},   //6
    {"Absorb", 120, TYPE_GRASS},  //7
    {"Cut", 150, TYPE_NORMAL},    //8
    {"Curse", 80, TYPE_GHOST},    //9
    {"Outrage", 80, TYPE_DRAGON}, //10
    {"Beat Up", 80, TYPE_DARK},   //11
    {"Kiss", 90, TYPE_FAIRY},     //12
    {"Revenge", 120, TYPE_FIGHTING},//13
    {"Rollout", 100, TYPE_ROCK}, //14
    {"Sting", 90, TYPE_BUG},        //15
    {"Bullet", 80, TYPE_STEEL},     //16
    {"Sludge", 80, TYPE_POISON},    //17
    {"Aerial", 75, TYPE_FLYING}     //18
};


struct Pokemon snorlax = {"Snorlax", 220, 80, 65, 30, 110, 65, 50, {allMoves[0], allMoves[4], allMoves[16], allMoves[16]}, 3, PokeBuffer, PokeBuffer2, TYPE_NORMAL, "snorlax.bin", "snorlaxback.bin"};
struct Pokemon mudkip = {"Mudkip", 210, 70, 50, 40, 50, 50, 50, {allMoves[0], allMoves[6], allMoves[4], allMoves[16]}, 2, PokeBuffer, PokeBuffer2, TYPE_WATER, "mudkip.bin", "mudkipback.bin"};
struct Pokemon treecko = {"Treecko", 190, 50, 40, 70, 50, 50, 50, {allMoves[0], allMoves[7], allMoves[4], allMoves[16]}, 1, PokeBuffer, PokeBuffer2, TYPE_GRASS, "treecko.bin" , "treeckoback.bin"};
struct Pokemon torchic = {"Torchic", 205, 200, 40, 45, 70, 70, 50, {allMoves[0], allMoves[2], allMoves[4], allMoves[16]}, 0, PokeBuffer, PokeBuffer2, TYPE_FIRE, "torchic.bin" , "torchicback.bin"};
struct Pokemon meowth = {"Meowth", 200, 65, 35, 90, 40, 40, 50, {allMoves[0], allMoves[4], allMoves[16], allMoves[16]}, 4, PokeBuffer, PokeBuffer2, TYPE_NORMAL, "meowth.bin", "meowthback.bin"};
struct Pokemon ampharos = {"Ampharos", 250, 75, 85, 55, 115, 90, 50, {allMoves[1], allMoves[5], allMoves[16], allMoves[16]}, 5, PokeBuffer, PokeBuffer2, TYPE_ELECTRIC, "ampharos.bin", "ampharosback.bin"};
struct Pokemon cubchoo = {"Cubchoo", 215, 70, 40, 40, 60, 40, 50, {allMoves[0], allMoves[3], allMoves[16], allMoves[16]}, 8, PokeBuffer, PokeBuffer2, TYPE_ICE, "cubchoo.bin", "cubchooback.bin"};
struct Pokemon wobbuffet = {"Wobbuffet", 270, 33, 58, 33, 33, 58, 50, {allMoves[0], allMoves[4], allMoves[16], allMoves[16]}, 6, PokeBuffer, PokeBuffer2, TYPE_PSYCHIC, "wobbuffet.bin", "wobbuffetback.bin"};
struct Pokemon rayquaza = {"Rayqua", 250, 100, 50, 55, 50, 50, 50, {allMoves[5], allMoves[1], allMoves[5], allMoves[16]}, 7, PokeBuffer, PokeBuffer2, TYPE_DRAGON,"rayquaza.bin" ,"rayquazaback.bin"};

struct Pokemon allPokemon[9] = {torchic, treecko, mudkip, snorlax, meowth, ampharos, wobbuffet, rayquaza, cubchoo};
struct Pokemon player[6];
uint8_t playerFainted[6] = {0, 0, 0, 0, 0, 0};
uint8_t playerAlive = 1;
uint8_t playerDead = 0;
uint8_t totalPokemon = 0;
//// Define the moves for each Pokemon
////uint8_t mudKipMoves[] = {0, 6, 4, 16};
////uint8_t treeckoMoves[] = {0, 7, 4, 16};
////uint8_t torchicMoves[] = {0, 2, 4, 16};
////uint8_t meowthMoves[] = {0, 4, 16, 16};
////uint8_t kyogreMoves[] = {6, 1, 3, 16};
////uint8_t rayquaazaMoves[] = {5, 1, 3, 16};
////uint8_t groudonMoves[] = {4, 1, 3, 16};
//uint8_t gengarMoves[] = {9, 5, 16, 16};
//
//
//// Create the Pokemon objects
////Pokemon mudkip = createPokemon("Mudkip", 50, 70, 50, 40, 50, 50, TYPE_WATER, mudKipMoves, 0, mudkip);
////Pokemon treecko = createPokemon("Treecko", 40, 50, 40, 70, 50, 50, TYPE_GRASS, treeckoMoves, 1, treecko);
////Pokemon torchic = createPokemon("Torchic", 45, 60, 40, 45, 70, 70, TYPE_FIRE, torchicMoves, 2, torchic);
////Pokemon meowth = createPokemon("Meowth", 40, 45, 35, 90, 40, 40, TYPE_NORMAL, meowthMoves, 3, meowth);
////Pokemon kyogre = createPokemon("Kyogre", 100, 100, 90, 90, 140, 140, TYPE_WATER, kyogreMoves, 4, kyogre);
////Pokemon rayquaza = createPokemon("Rayquaza", 105, 150, 90, 95, 90, 90, TYPE_DRAGON, rayquaazaMoves, 5, rayquaza);
////Pokemon groudon = createPokemon("Groudon", 100, 150, 140, 90, 90, 90, TYPE_GROUND, groudonMoves, 6, groudon);
//
