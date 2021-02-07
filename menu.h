#ifndef MENU_BITMAPS_H
#define MENU_BITMAPS_H

#include <Arduino.h>
#include "globals.h"

#define FONT_TINY                 0
#define FONT_SMALL                1
#define FONT_BIG                  2

#define DATA_TIMER                0
#define DATA_SCORE                1
#define DATA_LEVEL                2

//byte blinkingFrames = 0;
//byte sparkleFrames = 0;
uint8_t cont = 0;

extern void drawNumbers(byte numbersX, byte numbersY, byte fontType, byte data);

void drawTitleScreen()
{
  sprites.drawSelfMasked(0,0, titleScreen, 0);
  /*
  if (arduboy.everyXFrames(8))blinkingFrames = (++blinkingFrames) % 32;
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, titleScreen, i);
  sprites.drawSelfMasked(85, 45, badgeMysticBalloon, 0);
  sprites.drawSelfMasked(79, 43, stars, sparkleFrames);
  sprites.drawSelfMasked(9, 9, leftGuyLeftEye, pgm_read_byte(&blinkingEyesLeftGuy[blinkingFrames]));
  sprites.drawSelfMasked(15, 13, leftGuyRightEye, pgm_read_byte(&blinkingEyesLeftGuy[blinkingFrames]));
  sprites.drawSelfMasked(109, 34, rightGuyEyes, pgm_read_byte(&blinkingEyesRightGuy[blinkingFrames]));
  */
}

void stateMenuIntro()
{
  globalCounter++;
  drawTitleScreen();
  if (/*(globalCounter > 250) ||*/ arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    //sound.tone(425, 20);
  }
}

void stateMenuMain()
{

  drawTitleScreen();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
    //gameState=STATE_GAME_PLAYCONTNEW;
    gameState=STATE_MENU_PLAY;
  /*
  sprites.drawOverwrite(101, 9, mainMenu, 0);
  if (arduboy.justPressed(DOWN_BUTTON) && (menuSelection < 5))
  {
    menuSelection++;
    //sound.tone(300, 20);
  }
  if (arduboy.justPressed(UP_BUTTON) && (menuSelection > 2))
  {
    menuSelection--;
    //sound.tone(300, 20);
  }
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = menuSelection;
    //sound.tone(425, 20);
  }
  sprites.drawPlusMask(96, 9 + 9 * (menuSelection - 2), selector_plus_mask, 0);
  */
}

void stateMenuHelp()
{/*
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(32, 32 * i, qrcode, i);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    //sound.tone(425, 20);
  }*/
}


void stateMenuInfo()
{
  /*
  sprites.drawSelfMasked(43, 2, badgeMysticBalloon, 0);
  sprites.drawSelfMasked(37, 0, stars, sparkleFrames);
  sprites.drawSelfMasked(40, 48, madeBy, 0);
  EEPROM.get(OFFSET_HSCORE, scorePlayer);
  if (EEPROM.read(OFFSET_COINSHS) == TOTAL_COINS)
  {
    sprites.drawSelfMasked(21, 28, badgeSuper, 0);
  }
  else
  {
    sprites.drawSelfMasked(28, 28, badgeBorder, 0);
  }
  sprites.drawSelfMasked(30, 28, badgeHighScore, 0);
  drawNumbers(55, 30, FONT_BIG, DATA_SCORE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    //sound.tone(425, 20);
  }*/
}

void stateMenuGameOver() // finalement non, je mets tout dans game.h pour avoir accès aux autres .h
{

  /*
  drawTitleScreen();
  sprites.drawOverwrite(51, 9, soundMenu, 0);
  if (arduboy.justPressed(DOWN_BUTTON))
  {
    //arduboy.audio.on();
    //sound.tone(300, 20);
  }
  //if (arduboy.justPressed(UP_BUTTON)) arduboy.audio.off();
  sprites.drawPlusMask(54, 18 + 9 * arduboy.audio.enabled(), selector_plus_mask, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
    //sound.tone(425, 20);
  }
  */
}

void stateMenuPlaySelect()
{
  drawTitleScreen();
  sprites.drawOverwrite(52, 17, loadMenu_bitmap, 0);
  if (arduboy.justPressed(DOWN_BUTTON))
  {
    cont = 1;
    //sound.tone(300, 20);
  }
  if (arduboy.justPressed(UP_BUTTON))
  {
    cont = 0;
    //sound.tone(300, 20);
  }
//  sprites.drawPlusMask(58, 18 + 9 * cont, selector_plus_mask, 0);
  arduboy.fillCircle(47, 22 + 9 * cont, 3, 1);
  arduboy.fillCircle(47, 22 + 9 * cont, 1, 0);
  if (arduboy.justPressed(B_BUTTON|A_BUTTON))
  {
    gameState = STATE_GAME_PLAYCONTNEW + 1 - cont;
    cont = 0;
    //sound.tone(425, 20);
  }
  /*if (arduboy.justPressed())
  {
    gameState = STATE_MENU_MAIN;
    //sound.tone(425, 20);
  }*/
}


#endif
