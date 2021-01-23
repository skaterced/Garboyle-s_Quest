#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

/*
#define TOTAL_TONES 10
PROGMEM const byte tones[] = {
  //200, 100, 250, 125, 300, 150, 350, 400, 425, 475
  131, 145, 139, 152, 131, 172, 200, 188, 213, 255
};

byte toneindex = 0;
*/
void stateMenuPlayNew()
{
  level = LEVEL_TO_START_WITH - 1;
 // coinsCollected = 0;
 // totalCoins = 0;
 // balloonsLeft = 0;
//  scorePlayer = 0;
  globalCounter = 0;
  initKid();
  gameState = STATE_GAME_NEXT_LEVEL;
 // scoreIsVisible = false;
 // nextLevelIsVisible = true;
 // pressKeyIsVisible = false;
}

void stateMenuPlayContinue()
{ 
  stateMenuPlayNew();
  /*
  level = EEPROM.read(OFFSET_LEVEL);
  totalCoins = EEPROM.read(OFFSET_COINS);
  coinsCollected = 0;
  balloonsLeft = 0;
  //scorePlayer = 0;
  EEPROM.get(OFFSET_SCORE, scorePlayer);
  globalCounter = 0;
  initKid();
  gameState = STATE_GAME_NEXT_LEVEL;
  scoreIsVisible = false;
  nextLevelIsVisible = true;
  pressKeyIsVisible = false;
  */
}


void stateGameNextLevel()
{

  /*
    scorePlayer=0; //bossesAlive;
  //if (level < TOTAL_LEVELS)
  //{
    if (arduboy.everyXFrames(20))
    {
 
    }

  // Update EEPROM
  
  EEPROM.put(OFFSET_LEVEL, level);
  EEPROM.put(OFFSET_COINS, totalCoins);
  EEPROM.put(OFFSET_SCORE, scorePlayer);
  */

  //toneindex = 0;
  //sound.tone(425, 20);
  setKid();
  cam.pos = vec2(0, 0);
  cam.pos = vec2(0, LEVEL_HEIGHT - 64);
  cam.offset = vec2(0, 0);
  enemiesInit();
  levelLoad(levels[level]);
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  globalCounter++;
  
  checkInputs();
  checkKid();
  updateCamera();

  drawGrid();
  enemiesUpdate();

  drawKid();
  drawHUD();

  checkCollisions();
}


void stateGamePause()
{
  drawKid();
  sprites.drawSelfMasked(47, 17, badgePause, 0);
  //bossLevels[NB_BOSS]
  if (arduboy.justPressed(B_BUTTON)){
    if (arduboy.pressed(LEFT_BUTTON)){ //cheat
      bossLevels[NB_BOSS-1].alive=true;
      bossLevels[NB_BOSS-1].lvl=level;
      bossLevels[NB_BOSS-1].reward=UPGRADE_JUMP;
      gameState = STATE_GAME_LVLUP;
    }
    else if (arduboy.pressed(RIGHT_BUTTON)){ //cheat
      bossLevels[NB_BOSS-1].alive=true;
      bossLevels[NB_BOSS-1].lvl=level;
      bossLevels[NB_BOSS-1].reward=UPGRADE_WINGS;
      gameState = STATE_GAME_LVLUP;
    }
    else if (arduboy.pressed(DOWN_BUTTON)){ //cheat
      bossLevels[NB_BOSS-1].alive=true;
      bossLevels[NB_BOSS-1].lvl=level;
      bossLevels[NB_BOSS-1].reward=UPGRADE_FIRE;
      gameState = STATE_GAME_LVLUP;
    }
  }
  else if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}

void stateGameLvlUp()
{
  uint8_t wichUp=getBossReward(false);
  switch (wichUp){
    case UPGRADE_JUMP:
      sprites.drawSelfMasked(30, 12, BadgeJump, 0);
    break;
    case UPGRADE_FIRE:
      sprites.drawSelfMasked(32, 12, BadgeFire, 0);
    break;
    case UPGRADE_WINGS:
      sprites.drawSelfMasked(26, 12, BadgeWings, 0);
    break;
    case UPGRADE_ARMOR:
      sprites.drawSelfMasked(20, 12, BadgeArmor, 0);
    break;
  }
  drawKid();
  sprites.drawSelfMasked(60, 12, BadgeLevelUp, 0);  
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {    
    switch (wichUp){
      case UPGRADE_JUMP:
        jumpVelocity+=10;
      break;
      case UPGRADE_FIRE:
        firePower++;
      break;
      case UPGRADE_WINGS:
        wingLvl++;
      break;
      case UPGRADE_ARMOR:
        heartsMax++;
      break;
    }
    
    bossRoom=false;
    kid.hearts=heartsMax;
    if (0==getBossReward(true)){
      gameState = STATE_GAME_PLAYING;
    }
  }
}


void stateGameOver()
{
  byte x = 35 + 12;
  /*
  if (level < TOTAL_LEVELS)
  {
    drawNumbers(78, 26, FONT_BIG, DATA_LEVEL);
    x -= 12;
  }*/
  sprites.drawSelfMasked(x, 17, badgeGameOver, 0);
  //drawNumbers(43, 49, FONT_BIG, DATA_SCORE);
  /*
  unsigned long highscore = 0;
  EEPROM.get(OFFSET_HSCORE, highscore);
  if (scorePlayer > highscore) {
    EEPROM.put(OFFSET_COINSHS, totalCoins);
    EEPROM.put(OFFSET_HSCORE, scorePlayer);
  }
    */
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    bossRoom=false;
    gameState = STATE_MENU_MAIN;
  }
}

#endif
