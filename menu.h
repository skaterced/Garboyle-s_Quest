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
//uint8_t cont = 0;
bool selectDown = false;

extern void drawNumbers(byte numbersX, byte numbersY, byte fontType, byte data);

void drawTitleScreen()
{
  sprites.drawSelfMasked(0,0, titleScreen, 0);
  if (finalBossBeaten){
    arduboy.fillRect(16,44,91,19); //28466 ->28500
    sprites.drawOverwrite(18,45, Title_end_bitmap, 0);    
    if (++globalCounter>0xF8){
      sprites.drawOverwrite(55,14, cligne_bitmap, 0);
    }
    if (difficulty&&((globalCounter>0xE0)||(globalCounter<10))){
      sprites.drawOverwrite(81,29, win_bitmap, 0);
    }
    #ifdef COUNT_DEATH
      // Draw    
      uint8_t temp=deathToll/5;
      for (uint8_t i=0; i<temp; i++){
        arduboy.drawLine(18+i*10, 63, 27+i*10, 58, 0);
      }
      for (uint8_t i=1; i<deathToll+1; i++){
        if (i%5)
          arduboy.drawLine(17+i*2, 58, 17+i*2, 63, 0);
      }
    #endif
  }
  
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

void drawSelector (uint8_t x, uint8_t y){
  if (arduboy.justPressed(DOWN_BUTTON))
  {
    selectDown = true;
    //sound.tone(300, 20);
  }
  if (arduboy.justPressed(UP_BUTTON))
  {
    selectDown = false;
    //sound.tone(300, 20);
  }
  // Up: 48, 21  Down: 39, 48
  arduboy.fillCircle(x, y + 9 * selectDown, 3, 1);
  arduboy.fillCircle(x, y + 9 * selectDown, 1, 0);  
}

void stateMenuIntro()
{
  //globalCounter++;
  drawTitleScreen();
  if (/*(globalCounter > 250) ||*/ arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    finalBossBeaten=false; 
    //sound.tone(425, 20);
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
    //gameState=STATE_GAME_PLAYCONTNEW;
    selectDown = false;
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

void stateMenuDifficulty()
{
  drawTitleScreen();
  sprites.drawOverwrite(52, 17, easy_hard_bitmap, 0);
  drawSelector(48,21);
  if (arduboy.justPressed(B_BUTTON|A_BUTTON))
  {
    difficulty = selectDown;
    gameState =  STATE_GAME_PLAYNEW;
    selectDown = false;    
  }
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
  drawSelector(48,21);
  if (arduboy.justPressed(B_BUTTON|A_BUTTON))
  {
    gameState = selectDown? STATE_GAME_PLAYLOAD:STATE_MENU_DIFF;
    selectDown = false;
    //sound.tone(425, 20);
  }
}


#endif
