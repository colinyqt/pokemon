/*
 * Animations.c
 *
 *  Created on: Apr 17, 2024
 *      Author: drewp
 *      the unfortunate workhorse of the overworld
 */

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "images/images.h"
#include "Overworld.h"
#include "Pokemon.h"
#include "global.h"
#include "battle.h"
#include "Sound.h"


int8_t nextMoveX;
int8_t nextMoveY;
//DOES NOT WORK FOR THE TOP ROW OF SPRITES
void removeSprite(uint32_t xloc, uint32_t yloc, uint32_t xdim, uint32_t ydim, const uint16_t *bg) {
    for (int i = xloc; i < (xloc+xdim); i++) {
        for (int j = yloc; j > (yloc-ydim); j--) {
            short newpixelcolor = bg[(((~(j-160))+1)*128)+i];
            ST7735_DrawPixel(i, j, newpixelcolor);
        }
    }
}



//TODO: HARDFULTS AT THE BOTTOM OF THE SCREEN WHEN OFF-SCREEN
void drawSprite(int32_t x, int32_t y, const uint16_t *image, int32_t w, int32_t h, uint32_t transparentColor) {
    int saveY = y;
    for (int16_t i = 0; i < w; i++) {
        y = saveY;
        for (int16_t j = 0; j < h; j++) {
            // Get the color of the current pixel
            uint16_t color = image[j*w+i];
            // Only draw the pixel if it's not the transparent color
            if (color != transparentColor) {
                ST7735_DrawPixel(x, y, color);
            }
            y--;
        }
        x++;
    }
}

void drawFlashSprite(int32_t x, int32_t y, const uint16_t *image, int32_t w, int32_t h, uint32_t transparentColor, uint16_t newColor) {
    int saveY = y;
    for (int16_t i = 0; i < w; i++) {
        y = saveY;
        for (int16_t j = 0; j < h; j++) {
            // Get the color of the current pixel
            uint16_t color = image[j*w+i];
            // Only draw the pixel if it's not the transparent color
            if (color != transparentColor) {
                ST7735_DrawPixel(x, y, newColor);
            }
            y--;
        }
        x++;
    }
}

void drawDialogue(uint16_t color) {
    ST7735_FillRect(0, 120, 128, 40, color);
    ST7735_FillRect(5, 125, 118, 30, 65535);
}

void playerWalk(uint8_t frame, int8_t dir) {
    switch(frame){
    case(1): //move player slightly
            // determine direction and save in struct
            removeSprite(wld.x, wld.y, brendan.x[wld.sprite], brendan.y[wld.sprite], npcmap);
        wld.face = dir;
        switch (dir) {
            case(-2): //up(-)
                wld.sprite = 4;
                wld.y -= 8;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                nextMoveX = 0;
                nextMoveY = -8;
                break;
            case(2):
                wld.sprite = 5;
                wld.y += 8;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                nextMoveX = 0;
                nextMoveY = 8;
                break;
            case(1):
                wld.sprite = 7;
                wld.x += 8;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                nextMoveX = 8;
                nextMoveY = 0;
                break;
            case(-1):
                wld.sprite = 6;
                wld.x -= 8;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                nextMoveX = -8;
                nextMoveY = 0;
                break;
            }
        break;
    case(7): // stationary next grid
         removeSprite(wld.x, wld.y, brendan.x[wld.sprite], brendan.y[wld.sprite], npcmap);
         wld.x += nextMoveX;
         wld.y += nextMoveY;
         wld.sprite -= 4;
         drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
         nextMoveX = 0;
         nextMoveY = 0;
         wld.moveCount = 0;
         wld.walkFrame = 0;
         wld.WildPokemon = 1;

        break;
    }
}

void playerChangeFace(int8_t direction) {
    if (wld.face != direction) {
        removeSprite(wld.x, wld.y, brendan.x[wld.sprite], brendan.y[wld.sprite], npcmap);
        wld.face = direction;
        switch (direction) {
        case(-2):
                wld.sprite = 0;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                break;
        case(2):
                wld.sprite = 1;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                break;
        case(1):
                wld.sprite = 3;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                break;
        case(-1):
                wld.sprite = 2;
                drawSprite(wld.x, wld.y, brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
                break;
        }
    }
}

void pauseScreen(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause, uint8_t framenum, uint8_t cursor) {
    //make sure to only redraw brendan if he is in box
    // save whats on screen first probably using global struct array
    switch (framenum) {
    case (0):
        ST7735_FillRect(58,77, 12, 6, 0);
    break;

    case(2):
        ST7735_FillRect(44, 70, 40, 20, 0);
    break;

    case(4):
        ST7735_FillRect(24, 60, 80, 40, 0);
    break;

    case(6):
        ST7735_FillRect(4, 50, 120, 60, 0);

        ST7735_FillRect(8, 54, 2, 10, 65535);
        ST7735_FillRect(12, 54, 2, 10, 65535);

        ST7735_SetCursor(8, 6);
        ST7735_OutString("Score:");
        ST7735_SetCursor(15, 6);
        ST7735_OutUDec(score, 65353, 0);
        //do against after implementing score

        ST7735_SetCursor(3, 8);
        ST7735_OutString("Resume?");

        ST7735_SetCursor(12, 8);
        ST7735_DrawSmallCircle(33, 90, 65535);
        cursor = 1;

    break;

    case(7):
    if (pause != 0 || B != 0) {
        playerState = 0;
        Sound_Button();
    }

    if (A != 0 && cursor == 1) {
        Sound_Button();
        playerState = 0;
    }
    break;
    }
}






void wildTransition(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause, uint8_t framenum) {
    switch (framenum) {
        case (0): //begins with the exclaim
            drawSprite((wld.x+6), (wld.y - 23), exclaim, 7, 12, 65535);
            Sound_Button();
            // playsound
            break;
        case(15): // start the fade to white
            ST7735_FillRect(0, 0, 10, 160, 65535);
            break;
        case(16):
            ST7735_FillRect(0, 0, 20, 160, 65535);
            break;
        case(17):
            ST7735_FillRect(0, 0, 40, 160, 65535);
            break;
        case(18):
            ST7735_FillRect(0, 0, 80, 160, 65535);
            break;
        case(19):
            ST7735_FillRect(0, 0, 128, 160, 65535);


        if (wld.npcFlag != 0) {
            battleInfo.isWild = 0;
            battleInfo.enemy[0] = rayquaza;
            int throw = getPokemonSDC(rayquaza.file);
            (GPIOA->DOUTSET31_0 = (1<<12)); //SDC_CS_HIGH();
            (GPIOB->DOUTCLR31_0 = (1<<6)); //TFT_CS_LOW();
        } else {
        int random = Random(9);
        int throw = getPokemonSDC(allPokemon[random].file);
        (GPIOA->DOUTSET31_0 = (1<<12)); //SDC_CS_HIGH();
        (GPIOB->DOUTCLR31_0 = (1<<6)); //TFT_CS_LOW();
        battleInfo.enemyIndex = 0;
        battleInfo.enemy[battleInfo.enemyIndex] = allPokemon[random];
        battleInfo.isWild = 1;
        }
        int g;
        for (g = 0; g < totalPokemon; g++) {
            if(player[g].hp > 0) {
              break;
            }
        }
        battleInfo.playerIndex = g;
            break;


        case(45):
            ST7735_DrawBitmap(64, 60, PokeBuffer , 64, 64);
            break;

        case(48):
            drawDialogue(24167);
        if (battleInfo.isWild == 1) {
        if (language == 1) {
            ST7735_DrawString(1, 13, "Wow! It's a wild", 0);
        } else {
            ST7735_DrawString(1, 13, "\xADUn pokemon salvaje!", 0);
        }
            ST7735_DrawString(1, 14, battleInfo.enemy[battleInfo.enemyIndex].name, 0);
        } else {
            if (language == 1) {
                ST7735_DrawString(1, 13, "Patt Challenges You", 0);
        } else {
            ST7735_DrawString(1, 13, "Patt te desafía...", 0);
        }
            break;
        case(49):
            if (A == 1) {
                playerState = 4; //oh yeah its battle time baby!!11
                break;
            }
            break;
    }
}
}

void menuScreen(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t buffer, uint8_t framenum, uint8_t previous) {
    // from above, bring down a screen
    uint8_t j = 0;
    switch (framenum) {

        case(0):
            ST7735_FillRect(0, 0, 128, 5, 0);
            break;
        case(4):
            ST7735_FillRect(0, 0, 128, 30, 0);

            break;
        case(7):
            ST7735_FillRect(0, 0, 128, 80, 0);

            break;
        case(10):
            ST7735_FillRect(0, 0, 128, 130, 0);

            break;
        case(20):
            ST7735_FillRect(0, 0, 128, 160, 0);
            drawSprite(20, 20, pokbal, 14, 14, 19844);
            drawSprite(20, 40, pokbal, 14, 14, 19844);
            drawSprite(20, 60, pokbal, 14, 14, 19844);
            drawSprite(20, 80, pokbal, 14, 14, 19844);
            drawSprite(20, 100, pokbal, 14, 14, 19844);
            drawSprite(20, 120, pokbal, 14, 14, 19844);
            break;
        //From here on out is the display
        case(21):

            for (uint8_t i = 0; i <= 6; i++) {
                ST7735_DrawString(6, j+1, player[i].name, 65535);
                j +=2;
            }
                ST7735_DrawString(1, 13, "Here is your team :)", 65535);
                break;
        case(22):
                if(battleInfo.ScreenFlag != 0) {
                    ST7735_DrawSmallCircle(15, 10, 65535);
                    menuY = 10;
                    ST7735_DrawString(1, 14, "Choose a Pokemon", 65535);
                } else {
                    if (B != 0) {
                        playerState = 0;
                    }
                }
                break;
        case(23):
                if(battleInfo.ScreenFlag != 0) {
                    if (move != 0 && menuBuffer < 4) {
                        menuBuffer++;
                    } else if (move == 0) {
                        menuBuffer = 0;
                    }
                    if (move == -2 && menuBuffer == 4 && (menuY > 10)) {
                        ST7735_DrawSmallCircle(15, menuY, 0);
                        ST7735_DrawSmallCircle(15, menuY-20, 65535);
                        menuY -= 20;
                        menuBuffer = 0;
                        return;
                    } else if (move == 2 && menuBuffer == 4 && (menuY < 110)) {
                        ST7735_DrawSmallCircle(15, menuY, 0);
                        ST7735_DrawSmallCircle(15, menuY+20, 65535);
                        menuY += 20;
                        menuBuffer = 0;
                        return;
                    }

                    if(A != 0) {
                        if (player[(menuY-10)/20].hp >= 0 && player[(menuY-10)/20].attack != 0) {
                            Sound_Button();
                            battleInfo.playerIndex = ((menuY-10)/20);
                            battleInfo.animFrame = 0;
                            battleInfo.ScreenFlag = 0;
                            battleInfo.cursor = 0;
                            ST7735_FillRect(0, 0, 128, 160, 65535);
                            drawSprite(64, 60, battleInfo.enemy[battleInfo.enemyIndex].sprite, 64, 64, 65535);
                            playerState = 4;

                        }
                    }

                } else {
                    if (B != 0) {
                        Sound_Button();
                        playerState = 0;
                    }
                }
    }
}


uint8_t languageScreen(int8_t move, uint8_t A, uint8_t previous, uint8_t framenum) {
    if (framenum == 0) {
        drawSprite(0, 14, pokbal, 14, 14, 19844);
        ST7735_SetCursor(4, 3);
        ST7735_OutString("Please Select");
        ST7735_SetCursor(6, 4);
        ST7735_OutString("A Language");
        ST7735_SetCursor(6, 6);
        ST7735_OutString("Seleccione");
        ST7735_SetCursor(7, 7);
        ST7735_OutString("un idioma");
        //display text and options
        ST7735_SetCursor(7, 10);
        ST7735_OutString("English");
        ST7735_SetCursor(7, 12);
        ST7735_OutString("Espa\xA4ol");
        ST7735_DrawSmallCircle(33, 100, 65535);
        return 1;
    }

    if (move == -2 && previous != 1) {
        ST7735_DrawSmallCircle(33, 120, 0);
        ST7735_DrawSmallCircle(33, 100, 65535);
        language = 1;
        return 1;
    } else if (move == 2 && previous != 2) {
        ST7735_DrawSmallCircle(33, 100, 0);
        ST7735_DrawSmallCircle(33, 120, 65535);
        language = 2;
        return 2;
    }

    if (A != 0) {
        Sound_Button();
        playerState = 6;
    }
}

uint8_t pokeballBuffer;
uint8_t pokeball = 0;
void characterScreen(int8_t move, uint8_t A, uint8_t B, uint8_t previous, uint8_t framenum) {
    switch (framenum) {
    case (0):
        //makes entire screen black
        ST7735_FillRect(0, 0, 120, 160, 0);
        //output text
        if(language == 1) {
            ST7735_DrawString(4, 3, "Pick a partner!", ST7735_YELLOW);
        }
        else {
            ST7735_DrawString(3, 3, "\xAD Elige un socio!", ST7735_YELLOW);
        }
        //use the pokeball as the cursor
        drawSprite(25, 90, pokbal, 14, 14, 19844);
        pokeball = 90; // 20 apart
        //draw the three pokemon sprites
        //assuming torchic is 0
        ST7735_SetCursor(7,8);
        ST7735_OutString("Torchic");
        ST7735_SetCursor(7,10);
        ST7735_OutString("Treecko");
        ST7735_SetCursor(7,12);
        ST7735_OutString("Mudkip");
        //im assuming these are button functions
        //pokeball already drawn
        break;
        //same joystick movement as above - cursor == index of move array
    case (12):

        if (move != 0 && pokeballBuffer < 4) {
            pokeballBuffer++;
        } else if (move == 0) {
            pokeballBuffer = 0;
        }
        if (move == -2 && pokeballBuffer == 4) {
            if (pokeball > 90) {
                //prevents it from going out of bounds
                ST7735_FillRect(25, pokeball-14, 14, 14, 0);
                pokeball -= 20;
                drawSprite(25, pokeball, pokbal, 14, 14, 19844);
                pokeballBuffer = 0;

            }
        } else if (move == 2 && pokeballBuffer == 4) {
            if (pokeball < 130) {
                ST7735_FillRect(25, pokeball-14, 14, 14, 0);
                pokeball += 20;
                drawSprite(25, pokeball, pokbal, 14, 14, 19844);
                pokeballBuffer = 0;
            }
        }
        if(A == 1) {
            Sound_Button();
            if(pokeball == 90) {
            player[0] = allPokemon[0];
            } else if (pokeball == 110) {
                player[0] = allPokemon[1];
            } else if (pokeball == 130) {
                player[0] = allPokemon[2];
            }
            framenum = 0;
            playerState = 0;
        }
        break;
    }
}


void endScreen(uint8_t move, uint8_t A, uint8_t B, uint8_t previous, uint8_t framenum) {
    switch (framenum) {
    case(0):
        ST7735_FillRect(0, 0, 128, 160, 0);
        ST7735_SetCursor(5, 3);
            if(language == 1) {
                ST7735_OutString("You have lost!");
                ST7735_SetCursor(5, 5);
                ST7735_OutString("Your score is:");
                ST7735_SetCursor(5, 7);
                ST7735_OutUDec(score, 65535, 0);
                break;
            } else {
                ST7735_OutString("Has perdido!");
                ST7735_SetCursor(5, 5);
                ST7735_OutString("Puntos:");
                ST7735_SetCursor(5, 7);
                ST7735_OutUDec(score, 65535, 0);
                break;
            }
        break;
    }
}
