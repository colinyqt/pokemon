/*
 * Animations.h
 *
 *  Created on: Apr 17, 2024
 *      Author: drewp
 */

#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

void removeSprite(uint32_t xloc, uint32_t yloc, uint32_t xdim, uint32_t ydim, const uint16_t *bg);

void drawSprite(int32_t x, int32_t y, const uint16_t *image, int32_t w, int32_t h, uint16_t transparentColor);

void drawFlashSprite(int32_t x, int32_t y, const uint16_t *image, int32_t w, int32_t h, uint32_t transparentColor, uint16_t newColor);

void playerWalk(uint8_t frame, int8_t dir);

void playerChangeFace(int8_t direction);

void pauseScreen(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause, uint8_t framenum, uint8_t cursor);

void wildTransition(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause, uint8_t framenum);

void drawDialogue(uint16_t color);

void menuScreen(int8_t move, uint8_t A, uint8_t B, uint8_t menu, uint8_t pause, uint8_t framenum, uint8_t cursor);

void drawBattleScreen (uint8_t framenum);

uint8_t languageScreen(int8_t move, uint8_t A, uint8_t previous, uint8_t framenum);

void characterScreen(int8_t move, uint8_t A, uint8_t B, uint8_t previous, uint8_t framenum);

void endScreen(uint8_t move, uint8_t A, uint8_t B, uint8_t previous, uint8_t framenum);
#endif /* ANIMATIONS_H_ */
