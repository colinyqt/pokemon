/*
 * battle.c
 *
 *  Created on: Apr 19, 2024
 *      Author: drewp
 */
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "images/images.h"
#include "Animations.h"
#include "Overworld.h"
#include "global.h"
#include "battle.h"
#include "Pokemon.h"
#include "Sound.h"


//initialize global data structures because we suck at coding lol
struct battle battleInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


// animation functions
void drawStatsBox(uint32_t x, uint32_t y, struct Pokemon pokemon, uint16_t color) {
    //draw black rectangle and then white one over it, stats , display stats of the pokemon fed into the function
    ST7735_FillRect(x, y, 50, 35, color);
    ST7735_FillRect(x+3, y+3, 44, 29, 65535);
    //drawing the text - need HP and Name
    if (x<64) {
        ST7735_DrawString(1, 1, pokemon.name , 0);
        ST7735_DrawString(1, 2, "HP:", 0);
        ST7735_SetCursor(4, 2);
        if(battleInfo.enemy[battleInfo.enemyIndex].hp > 0) {
            ST7735_OutUDec(pokemon.hp, 0, 65535);
        } else {
            ST7735_OutUDec(0, 0, 65535);
        }

    } else if (x>64) {
        ST7735_DrawString(14, 7, pokemon.name , 0);
        ST7735_DrawString(14, 8, "HP:", 0);
        ST7735_SetCursor(17, 8);
        if(player[battleInfo.playerIndex].hp > 0) {
            ST7735_OutUDec(pokemon.hp, 0, 65535);
        } else {
            ST7735_OutUDec(0, 0, 65535);
        }
    }
}
void drawChoiceMenu() {
    ST7735_FillRect(68, 100, 60, 70, 0);
    ST7735_FillRect(71, 103, 54, 64, 65535);
    if (language == 1) {
    ST7735_DrawString(13, 11, "Attack", 0);
    ST7735_DrawString(13, 12, "Run", 0);
    ST7735_DrawString(13, 13, "Menu", 0);
    ST7735_DrawString(13, 14, "Catch", 0);
    } else {
        ST7735_DrawString(13, 11, "Atacar", 0);
        ST7735_DrawString(13, 12, "Correr", 0);
        ST7735_DrawString(13, 13, "La Carta", 0);
        ST7735_DrawString(13, 14, "Agarrar", 0);
}
}
void drawCursor(uint8_t cursor) {
    if (cursor == 1) {
        ST7735_DrawSmallCircle(72, 110, 0);
        ST7735_DrawSmallCircle(72, 120, 65535);
    } else if (cursor == 2) {
        ST7735_DrawSmallCircle(72, 120, 0);
        ST7735_DrawSmallCircle(72, 110, 65535);
        ST7735_DrawSmallCircle(72, 130, 65535);
    } else if (cursor == 3) {
        ST7735_DrawSmallCircle(72, 130, 0);
        ST7735_DrawSmallCircle(72, 120, 65535);
        ST7735_DrawSmallCircle(72, 140, 65535);
    } else if (cursor == 4) {
        ST7735_DrawSmallCircle(72, 130, 65535);
        ST7735_DrawSmallCircle(72, 140, 0);
    }
}
void drawBattleMenu(struct Pokemon pokemon) {
    ST7735_FillRect(68, 100, 60, 70, 0);
    ST7735_FillRect(71, 103, 54, 64, 65535);
    ST7735_DrawString(13, 11, pokemon.Moves[0].name, 0);
    ST7735_DrawString(13, 12, pokemon.Moves[1].name, 0);
    ST7735_DrawString(13, 13, pokemon.Moves[2].name, 0);
    ST7735_DrawString(13, 14, pokemon.Moves[3].name, 0);
}

void catchPokemon () {
    switch(battleInfo.animFrame) {
    case(0):
        ST7735_FillRect(68, 100, 60, 70, 65535);
        break;
    case(2):
        drawDialogue(0);
        if (language == 1) {
        ST7735_DrawString(1,13, "You threw a" ,0);
        ST7735_DrawString(1,14, "PokeBall!" ,0);
        } else {
            ST7735_DrawString(1,13, "\xADLanzaste una" ,0);
        ST7735_DrawString(1,14, "Pokebola!" ,0);
        }
        break;
    case(10):
        //draw pokeball middle of gengar
        drawSprite(70, 30, pokbal, 14, 14, 19844);
        break;
    case(18):
            //pokbal open
        drawSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535);
        drawSprite(70, 30, pokbal2, 14, 15, 19844);
        break;
    case(27):
        ST7735_FillRect(64, 5, 64, 60, 65535);
        ST7735_DrawCircle(70, 30, 0);
        break;
    case(35):
        ST7735_FillRect(64, 5, 64, 60, 65535);
        drawSprite(70, 30, pokbal2, 14, 15, 19844);
        break;
    case(40):
        ST7735_FillRect(64, 5, 64, 60, 65535);
        drawSprite(70, 30, pokbal, 14, 14, 19844);
        break;
    case(50):
            if (Random(100) < 25) {
                drawDialogue(0);
                if (language == 1) {
                ST7735_DrawString(1,13, "They Broke Free!" ,0);
                } else {
                    ST7735_DrawString(1,13, "\xADSe liberaron!" ,0);
                }
                ST7735_DrawString(1,14, ":(" ,0);
                battleInfo.animFrame += 30;
            } else {
                drawDialogue(0);
                if (language == 1) {
                ST7735_DrawString(1,13, "You caught him!" ,0);
                ST7735_DrawString(1,14, "Score + 20" ,0);
                } else {
                    ST7735_DrawString(1,13, "\xADLos atrapaste!" ,0);
                    ST7735_DrawString(1,14, "Puntuación + 20" ,0);
                }
                score += 20;
            }
            break;

    case(70):
            battleInfo.ScreenFlag = 7;
            battleInfo.animFrame = 0;
            battleInfo.WinLose = 2; // win!
            playerAlive++;
            totalPokemon++;
            player[totalPokemon] = battleInfo.enemy[battleInfo.enemyIndex];
            player[totalPokemon].index = totalPokemon;
            break;


    case(100):
            battleInfo.ScreenFlag = 0;
            battleInfo.animFrame = 40;
            ST7735_FillRect(0, 120, 128, 40, 65535);
            drawSprite(0, 160, PokeBuffer2, 64, 64, 28255);
            ST7735_FillRect(64, 5, 64, 60, 65535);
            drawSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535);
            break;
}
}

uint8_t playerPokemonStatus() {
 if (playerAlive == playerDead) {
     return 1;
 } else {
     return 0;
 }
}

// attack functions
void execute(struct Pokemon playerMon, struct Pokemon enemyMon, int moveIndex) {
        //executes based on speed parameters
    if (playerMon.speed >= enemyMon.speed) { // player is faster, subtract from enemy pok first
        battleInfo.faster = 1;
        float dmg = attackPokemon(playerMon, enemyMon, moveIndex);
        battleInfo.enemy[battleInfo.enemyIndex].hp -= dmg;
        //checks if enemy is alive, if they are then they damage the player
        if(battleInfo.enemy[battleInfo.enemyIndex].hp > 0) {
            battleInfo.enemyMove = Random(4);
            dmg = attackPokemon(enemyMon, playerMon, battleInfo.enemyMove);
            player[battleInfo.playerIndex].hp -= dmg;
            if (player[battleInfo.playerIndex].hp <= 0) {
                battleInfo.fainted = 1; // player fainted
                playerDead++;
            }
        } else {
            battleInfo.fainted = 2; // enemy fainted
        }
    } else { // enemy is faster, they attack first - opposite of above basically
        battleInfo.faster = 2;
        battleInfo.enemyMove = Random(4);
        float dmg1 = attackPokemon(enemyMon, playerMon, battleInfo.enemyMove);
        player[battleInfo.playerIndex].hp -= dmg1;
        //checks if player is alive, if they are then they damage the player
        if(player[battleInfo.playerIndex].hp > 0) {
            dmg1 = attackPokemon(playerMon, enemyMon, moveIndex);
            battleInfo.enemy[battleInfo.enemyIndex].hp -= dmg1;
            if (battleInfo.enemy[battleInfo.enemyIndex].hp <= 0) {
                battleInfo.fainted = 2;
            }
        } else {
            battleInfo.fainted = 1;
            playerDead++;
        }
    }
}


int attackPokemon(struct Pokemon attacker, struct Pokemon defender, int moveIndex) {
        // Determine which Pokemon is the attacker and which is the defender based on speed
        struct Move move = attacker.Moves[moveIndex];
        //stab implementation
        float typeEffectiveness = 1;
        if(move.type == attacker.types) { typeEffectiveness = 1.5; };

        //initialize damage
        int16_t damage = 0;

        //calculate damage functions
        if (move.type == TYPE_NORMAL) {
            damage = (((2 * attacker.level / 5 + 2) * move.damage * (attacker.attack / defender.defense)) / 50 ) + 2;
        }
        else {
            damage = (((2 * attacker.level / 5 + 2) * move.damage * (attacker.specialAttack / defender.specialDefense)) / 50 ) + 2;
        }
        //handle damage calculations and resultant HP (should update actual since we are passing pointers)
        return damage = (int)(damage * typeEffectiveness);
}

void BattleAnimation(){

    switch(battleInfo.animFrame) {
        case(0):
                execute(player[battleInfo.playerIndex], battleInfo.enemy[battleInfo.enemyIndex], battleInfo.cursor - 1);
                break;
        case(5):
                battleInfo.battleIter++;
                ST7735_FillRect(68, 100, 60, 70, 65535);
                break;
        case(10):
                drawDialogue(0);
                break;
        case(15):
               //implement if-else to see who's move is first
                if(battleInfo.faster == 1) {
                            if(language == 1) {
                                ST7735_DrawString(1, 13, player[battleInfo.playerIndex].name, 0);
                                ST7735_DrawString(1, 14, "USED", 0);
                                ST7735_DrawString(6, 14, battleInfo.playerMove.name, 0);
                            } else {
                                ST7735_DrawString(1, 13, player[battleInfo.playerIndex].name, 0);
                                ST7735_DrawString(1, 14, "USO EL ", 0);
                                ST7735_DrawString(6, 14, battleInfo.playerMove.name, 0);
                            }
                        } else {
                            if(language == 1) {
                                ST7735_DrawString(1, 13, battleInfo.enemy[battleInfo.enemyIndex].name, 0);
                                ST7735_DrawString(1, 14, "USED", 0);
                                ST7735_DrawString(6, 14, battleInfo.enemy[battleInfo.enemyIndex].Moves[battleInfo.enemyMove].name, 0);
                            }

                            else {
                                ST7735_DrawString(1, 13, battleInfo.enemy[battleInfo.enemyIndex].name, 0);
                                ST7735_DrawString(1, 14, "USO EL", 0);
                                ST7735_DrawString(6, 14, battleInfo.enemy[battleInfo.enemyIndex].Moves[battleInfo.enemyMove].name, 0);
                            }

                        }
                        break;

        case(30): //flicker 1
                ST7735_FillRect(0, 120, 128, 40, 65535);
                drawSprite(0, 160, PokeBuffer2, 64, 64, 28255);

                if (battleInfo.faster == 1) {
                    Sound_Damage();
                    drawFlashSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535, 65535);
                    break;
                }
                else {
                    Sound_Damage();
                    drawFlashSprite(0, 160, PokeBuffer2, 64, 64, 28255, 65535);
                    break;
                }
        case(32): //unflicker
                if (battleInfo.faster == 1) {
                    drawSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535);
                    break;
                }
                else {
                    drawSprite(0, 160, PokeBuffer2, 64, 64, 65535);
                    break;
                }
        case(34): //flicker 2
                if (battleInfo.faster == 1) {
                    drawFlashSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535, 0);
                    break;
                }
                else {
                    drawFlashSprite(0, 160, PokeBuffer2, 64, 64, 65535, 0);
                    break;
                }
        case(36): // unflicker
                if (battleInfo.faster == 1) {
                    drawSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535);
                    break;
                }

                else {
                    drawSprite(0, 160, PokeBuffer2, 64, 64, 65535);
                    break;
                }
        case(40):
                if (battleInfo.faster == 1) {
                    drawStatsBox(0, 0, battleInfo.enemy[battleInfo.enemyIndex], 0);
                } else {
                    drawStatsBox(78, 65, player[battleInfo.playerIndex], 0);
                }
                break;

        case(77):
                if(battleInfo.faster == 1) { //means player pokemon went first
                    // check whether or not the enemy survived
                    if(battleInfo.fainted == 2) {
                        battleInfo.ScreenFlag = 6;
                        battleInfo.animFrame=0;
                        battleInfo.battleIter = 0;
                        break;
                    } else if (battleInfo.battleIter >= 2) {
                        battleInfo.ScreenFlag = 0;
                        battleInfo.battleIter = 0;
                        battleInfo.animFrame = 40;
                    } else {
                        battleInfo.faster = 2;
                        battleInfo.animFrame = 4;
                        break;
                    }
                }  else { // if enemy went first
                    if (battleInfo.fainted == 1) {
                        battleInfo.ScreenFlag = 6;
                        battleInfo.animFrame = 0;
                        battleInfo.battleIter = 0;
                    } else if (battleInfo.battleIter >= 2){
                        battleInfo.ScreenFlag = 0;
                        battleInfo.battleIter = 0;
                        battleInfo.animFrame = 40;
                    } else {
                    battleInfo.animFrame = 4;
                    battleInfo.faster = 1;
                    break;
                }
        }

    }
}


void deathAnimation() {
    switch(battleInfo.animFrame) {
    case(1):
            if (battleInfo.fainted == 1) { //player faint
                drawFlashSprite(0, 160, PokeBuffer2, 64, 64, 65535, 63488);
            } else { // enemy faint
                drawFlashSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535, 63488);
            }
    break;
    case(5):
        //clear gengar
           if (battleInfo.fainted == 1) {
               ST7735_FillRect(0, 96, 64, 64, 65535);
           } else {
               ST7735_FillRect(64, 0, 64, 60, 65535);
           }
        break;

    case(6):
            if (battleInfo.isWild != 1) {
               if (battleInfo.fainted == 1) {
                   drawSprite(25, 140, pokbal, 14, 14, 19844);
               } else {
                drawSprite(80, 60, pokbal, 14, 14, 19844);
               }
            }
        break;
    case(14):
            if (battleInfo.fainted == 1) {
                ST7735_FillRect(0, 96, 64, 64, 65535);
            } else {
                ST7735_FillRect(64, 0, 64, 60, 65535);
            }
    break;
    case(20):
            if (battleInfo.fainted == 1) {
                //check if player has no more pokemon left
                uint8_t lost = playerPokemonStatus();
                if (lost == 1) {
                    battleInfo.WinLose = 1; // one means lost
                    battleInfo.animFrame = 0;
                    battleInfo.ScreenFlag = 7;
                } else {
                battleInfo.ScreenFlag = 3;
                battleInfo.fainted = 0;
                battleInfo.animFrame = 0;
                }
            } else { // check if battle is won
                if (battleInfo.isWild == 1 || battleInfo.isWild == 0) {
                    battleInfo.ScreenFlag = 7;
                    battleInfo.animFrame = 0;
                    battleInfo.WinLose = 2; // win!
                } else {
                    battleInfo.enemyFainted++;
                }
            }
            break;

    }
}


void pokemonSend(uint32_t frame) {
    switch (frame) {
        case(10):
                drawSprite(25, 140, pokbal, 14, 14, 19844);
            break;
        case(25):
                ST7735_FillRect(25, 126, 14, 14, 65535);
                drawSprite(25, 140, pokbal2, 14, 15, 19844);
                int throw2 = getPlayerPokemonSDC(player[battleInfo.playerIndex].file);
                (GPIOA->DOUTSET31_0 = (1<<12)); //SDC_CS_HIGH();
                (GPIOB->DOUTCLR31_0 = (1<<6)); //TFT_CS_LOW();
                break;
        case(35):
                drawSprite(0, 160, PokeBuffer2, 64, 64, 28255);
                break;
    }
}



void inBattle(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause) {
    //need a switch case because inputs do diff things depending on where we at
    switch(battleInfo.ScreenFlag) {

    case(0): //select screen
            // TODO: add flag that checks if the enemy pokemon deceased and then send out their next one
            if (battleInfo.cursor == 0) {
                if (battleInfo.animFrame == 0) {
                    ST7735_FillRect(0, 120, 128, 40, 65535); // clears dialogue box from previous frame
                    drawStatsBox(0, 0, battleInfo.enemy[battleInfo.enemyIndex], 0);
                    drawStatsBox(78, 65, player[battleInfo.playerIndex], 0);
                    battleInfo.animFrame = 1;
                    break;
                } else if (battleInfo.animFrame > 0 && battleInfo.animFrame < 41) {

                pokemonSend(battleInfo.animFrame); //draws pokemon and then the back of the sprite

                } else if (battleInfo.animFrame == 41) { //occurs when anim is done
                    drawChoiceMenu();
                    battleInfo.cursor = 1;
                    drawCursor(battleInfo.cursor);
                }
                battleInfo.animFrame++;
                break;
            }

            //if A has been pressed -> if else statement using the cursor location to determine next screen+set cursor to 0
            //get joystick input: use input to switch menu option after certain amount of frames has passed
            if (A == 1 && battleInfo.Abuffer < 3) {
                battleInfo.Abuffer++;
            } else if (A != 1) {
                battleInfo.Abuffer = 0;
            } else if (A != 0 && battleInfo.Abuffer == 3) {
                battleInfo.Abuffer = 0;
                battleInfo.ScreenFlag = battleInfo.cursor;
                battleInfo.cursor = 0;
                battleInfo.animFrame = 0;
                break;
            }
            //servicing joystick inputs - moves cursor
            if(battleInfo.cursorBuffer < 6 && move != 0) {
                battleInfo.cursorBuffer++;
            } else if (move == 0) {
                battleInfo.cursorBuffer = 0;
            }
            if(battleInfo.cursorBuffer == 6) {
                battleInfo.cursorBuffer = 0;
                if (move == -2) {
                    if (battleInfo.cursor != 1) {
                        battleInfo.cursor--;
                        drawCursor(battleInfo.cursor);
                        break;
                    }
                } else if (move == 2) {
                    if (battleInfo.cursor != 4) {
                        battleInfo.cursor++;
                        drawCursor(battleInfo.cursor);
                        break;
                    }
                }
            }
            break;

    case(1): //attack screen
            drawCursor(battleInfo.cursor);  //colin added - want to see why this isn't working lol
            if (battleInfo.cursor == 0) {
                drawBattleMenu(player[battleInfo.playerIndex]);
                battleInfo.cursor = 1;
                break;
            }

            //same joystick movement as above - cursor == index of move array
            if(battleInfo.cursorBuffer < 6 && move != 0) {
                battleInfo.cursorBuffer++;
            } else if (move == 0) {
                battleInfo.cursorBuffer = 0;
            }
            if(battleInfo.cursorBuffer == 6) {
                battleInfo.cursorBuffer = 0;
                if (move == -2) {
                    if (battleInfo.cursor != 1) {
                        battleInfo.cursor--;
                        drawCursor(battleInfo.cursor);
                        break;
                    }
                } else if (move == 2) {
                    if (battleInfo.cursor != 4) {
                        battleInfo.cursor++;
                        drawCursor(battleInfo.cursor);
                        break;
                    }
                }
            }

            //if A has been pressed -> set screen flag to battle phase
            if (A == 1 && battleInfo.Abuffer < 4) {
                battleInfo.Abuffer++;
            } else if (A != 1) {
                battleInfo.Abuffer = 0;
            } else if (A != 0 && battleInfo.Abuffer == 4) {
                battleInfo.playerMove = player[battleInfo.playerIndex].Moves[battleInfo.cursor - 1];
                battleInfo.animFrame = 0;
               battleInfo.ScreenFlag = 5;
               break;
            }
            //if B has been pressed, set cursor to 0, and animFrame = 0
            break;
    case(2): //running away
            if (battleInfo.isWild == 1) {
                if (battleInfo.animFrame == 0) {
                drawDialogue(0);
                if (language == 1) {
                ST7735_DrawString(1,13, "You ran away!", 0);
                } else {
                    ST7735_DrawString(1,13, "\xADTe fuiste!", 0);
                }
                }
                battleInfo.animFrame++;
                if (battleInfo.animFrame == 30) {
                    battleInfo.cursor = 0;
                    ST7735_DrawFastHLine(0, 0, 128, 0);
                    battleInfo.ScreenFlag = 0;
                    battleInfo.animFrame = 0;
                    battleInfo.fainted = 0;
                    playerState = 0;
                }
            } else {
                battleInfo.animFrame = 41;
                battleInfo.ScreenFlag = 0;

                //return to start menu
            }
    break;


    case(3): //menu screen
            playerState = 2;
            break;

    case(4): //catching
            if (battleInfo.isWild == 1) {
            if (battleInfo.ScreenFlag != 7) {
                catchPokemon();
                battleInfo.animFrame++;
            }
            } else {
                battleInfo.animFrame = 41;
                battleInfo.ScreenFlag = 0;
            }
    break;


    case(5): // battle anim phase
   player[battleInfo.playerIndex].speed = 200;
        battleInfo.cursor = 0;
        BattleAnimation();
        battleInfo.animFrame++;
        break;

    case(6): //death phase... womp womp
        deathAnimation();
        if (battleInfo.ScreenFlag == 6) {
            battleInfo.animFrame++;
        }
            break;
    case(7):
            if(battleInfo.animFrame == 0 && battleInfo.WinLose == 2) {
                drawDialogue(0);
                if (language == 1) {
                ST7735_DrawString(1,13, "You Win! Score + 10", 0);
                } else {
                    ST7735_DrawString(1,13, "\xADGanas! Puntuaci\xA2n + 10", 0);
                }
                wld.npcKill = 1;
                score += 10;
            } else if (battleInfo.animFrame == 0 && battleInfo.WinLose == 1) {
                if (language == 1) {
                ST7735_DrawString(1,13, "You Lost :(", 0);
                } else {
                    ST7735_DrawString(1,13, "\xAD Perdiste!", 0);
                }
            }
            battleInfo.animFrame++;

            if (battleInfo.animFrame == 30 && battleInfo.WinLose == 2) {
                battleInfo.cursor = 0;
                ST7735_DrawFastHLine(0, 0, 128, 0);
                battleInfo.ScreenFlag = 0;
                battleInfo.animFrame = 0;
                battleInfo.fainted = 0;
                wld.firstFlag = 0;
                playerState = 0;
            } else if (battleInfo.animFrame == 30 && battleInfo.WinLose == 1) { // lose
                battleInfo.cursor = 0;
                ST7735_DrawFastHLine(0, 0, 128, 0);
                battleInfo.ScreenFlag = 0;
                battleInfo.animFrame = 0;
                battleInfo.fainted = 0;
                battleInfo.WinLose = 0;
                playerState = 7;
            }
            break;
    }
}

