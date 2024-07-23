///*
// * Overworld.c
// *
// *  Created on: Apr 17, 2024
// *      Author: drewp
// *  This file handles all of the Overworld processing
// */
//

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "images/images.h"
#include "Animations.h"
#include "Overworld.h"
#include "global.h"
#include "Sound.h"

struct Overworld wld = {0, 158, 0, 0, 0, 0, 0, 0, 0};

void Overworld(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause) {

    if (wld.firstFlag == 0) {
        ST7735_DrawBitmap(0, 160, npcmap, 128, 160);
        drawSprite(wld.x, wld.y , brendan.sprites[wld.sprite], brendan.x[wld.sprite], brendan.y[wld.sprite], 57375);
        wld.firstFlag = 1;
        return;
    }

    if (pause != 0) {
        playerState = 1; // go to pause screen handler
        wld.firstFlag = 0;
        return;
    }


    if (menu != 0) {
        playerState = 2; // menu handler
        wld.firstFlag = 0;
        return;
    }

    // if A press
    if(((A != 0 && wld.moveCount != 6) || wld.npcFlag != 0) && wld.npcKill != 1) {
        int check = isSpecial();
        if (check == 1) {
            wld.npcFlag = 1;
            healPokemon();
            wld.walkFrame++;
            if (wld.walkFrame > 16 && A != 0) {
                if(wld.walkFrame > 30 && A!= 0 ) {
                removeSprite(0, 160, 128, 41, npcmap);
                wld.walkFrame = 0;
                wld.npcFlag = 0;
                wld.npcKill = 1;
            }
            }
            return;
        } else if (check == 2) {

            wld.npcFlag = 1;
            YalePatt();
            wld.walkFrame++;
        }
    }
    // Is there an NPC, and am I currently not in a running animation?

    // movement -> check if the tile infront is special(imp later probs compare x and y) - if not, then move - else skip
    int8_t collision = canMove(move);
    if (collision == 0) {
        if (wld.moveCount < 6 && move !=0) {
            wld.moveCount = wld.moveCount + 1;
        } else if (move == 0 && wld.moveCount != 6) {
            wld.moveCount = 0;
        }
        if (wld.moveCount == 6) {
            wld.walkFrame++;
            playerWalk(wld.walkFrame, move);
            if (wld.WildPokemon == 1) {
                wld.WildPokemon = 0;
                wld.npcKill = 0;
                checkEncounter();
            }
        }
    } else if (wld.walkFrame == 0) {
        playerChangeFace(move);
    }
}



int8_t canMove(int8_t direction) { //if this returns 1, then there aint NO MOVING!!!
    if(direction == -2) { // up
        if (wld.x >= 96) {  //NPC check
            return wld.y-16 < 36;
        }
        return wld.y-16 < 20;

    } else if (direction == 2) { // down
        return wld.y+16 > 170;

    } else if (direction == 1) { //right
        if (wld.y <= 30) {
            return wld.x+16 > 95;
        }
        return wld.x+16 >= 128;

    } else if (direction == -1) {
        return wld.x-16 < -10;
    } else {
        return 0;
    }
}

void checkEncounter() {
    if (wld.x < 64 && wld.y < 110) {
        int randomCheck = Random(100);
        if(randomCheck < 20) {
            playerState = 3; // battle transition
            wld.firstFlag = 0;
        }
    }
}

int isSpecial() {
    if ((wld.x + 16 >= 112 && wld.x+16 < 128) && (wld.y - 16 < 40) && (wld.face == -2)) {
        return 1;
    } else if ((wld.x + 16 == 128) && (wld.y - 16 < 40) && (wld.face == -2)){
        return 2;
    } else {
        return 0;
    }
}

void healPokemon() {
    switch (wld.walkFrame) {
    case(0):
                    Sound_Button();
    case(10):
            drawDialogue(64082);
            if(language == 1) {
            ST7735_DrawString(1, 13, "I will heal you", 0);
            } else {
            ST7735_DrawString(1, 13, "Te voy a curar", 0);
            }
            for (int r = 0; r < totalPokemon; r++) {
                player[r].hp = allPokemon[player[r].index].hp;
            }
            break;
    }
}

void YalePatt() {
    switch (wld.walkFrame) {
    case(0):
            Sound_Button();
    case(10):
            drawDialogue(64082);
            if (language == 1){
            ST7735_DrawString(1, 13, "I am Yale Patt", 0);
            } else {
            ST7735_DrawString(1, 13, "Me llamo Yale N. Patt", 0);
            }
            break;
    case(40):
            ST7735_FillRect(5, 125, 118, 30, 65535);
            if (language == 1) {
            ST7735_DrawString(1, 13, "Fear Me.", 0);
            } else {
            ST7735_DrawString(1, 13, "Miedo.", 0);
            }
            break;
    case(70):
            playerState = 3;

            wld.walkFrame = 0;
    }

}
