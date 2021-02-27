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
  cam.pos = vec2(0, 0);
  for (uint8_t i=0; i<NB_BOSS; i++){
    bossLevels[i].alive=true;
  }
  level = 0;
  wichEntrance=0;
  globalCounter = 0;
  initKid();
  deathToll=0;
  if (!difficulty)
    firePower++;
  gameState = STATE_GAME_NEXT_LEVEL;
}


void stateGameNextLevel()
{
  //toneindex = 0;
  //sound.tone(425, 20);
  setKid();
  //cam.pos = vec2(0, 0);
  //cam.pos.y = level_height_cell*16;
  //cam.offset = vec2(0, 0);
  enemiesInit();
  levelLoad(levels[level]);
  globalCounter=0;  // to prevent changing level sickness ;)
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

void upgradeKid(uint8_t wichUp){
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
    upgradeKid(wichUp);    
    bossRoom=false;
    kid.hearts=heartsMax;
    if (0==getBossReward(true)){
      gameState = STATE_GAME_PLAYING;
      #ifdef BOSS_RUSH
        level=0;
        wichEntrance=0;
        gameState = STATE_GAME_NEXT_LEVEL;
      #endif 
    }
  }
}

void stateMenuPlayLoad()
{ 
  //stateMenuPlayNew();
  cam.pos = vec2(0, 0);
  level = 0;
  wichEntrance=0;
  globalCounter = 0;
  initKid();  
  uint16_t gameSlot;
  //for (uint8_t j=0; j<2; j++){
  EEPROM.get(OFFSET_DIFFICULTY, difficulty);
  if (!difficulty)
    firePower++;
  EEPROM.get(OFFSET_LEVEL, gameSlot);
  for (uint8_t i=0; i<NB_BOSS; i++){
    if (0x01==(gameSlot&0x01)){
      bossLevels[i].alive=false;
      upgradeKid(bossLevels[i].reward);
    }
    gameSlot>>=1;
  }
  EEPROM.get(OFFSET_DEATH, deathToll);  
  
  kid.hearts=heartsMax;
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGameOver(bool showBadgeGO) // if false, means its called by pauseMenu
{
  if (showBadgeGO){
    sprites.drawSelfMasked(47, 17, badgeGameOver, 0);
  }
    if (globalCounter<30){ //why all that already? to prevent quit by mistake I guess...
      globalCounter++;
      selectDown=false;   
      if (arduboy.justPressed(B_BUTTON|A_BUTTON))
        globalCounter=100;
    }
  sprites.drawSelfMasked(44, 45, continue_bitmap, 0);
  drawSelector(39, 48);

  if (arduboy.justPressed(B_BUTTON|A_BUTTON))
  {    
    if (selectDown){ // save and quit
      uint16_t gameSlot = 0;
      for (uint8_t i=0; i<NB_BOSS-1; i++){ //the last is for cheat
        if (!bossLevels[i].alive){
          gameSlot += ( 1 << i) ;            
        }          
      }        
      EEPROM.put(OFFSET_DIFFICULTY, difficulty);
      EEPROM.put(OFFSET_LEVEL, (uint16_t)gameSlot);
      EEPROM.put(OFFSET_DEATH, (uint8_t)(++deathToll));
      gameState = STATE_MENU_INTRO ;    
    }
    else { // continue
      if (showBadgeGO){
        level=0;
        bossRoom=false;
        wichEntrance=0;
        kid.lives=3;
        deathToll++;
        gameState = STATE_GAME_NEXT_LEVEL ;    
      }
      else {
        gameState = STATE_GAME_PLAYING;
      }
    }
  }  
}

void stateGameOver(){
  stateGameOver(true);
}

void stateGamePause()
{
  //drawKid();
  sprites.drawSelfMasked(47, 17, badgePause, 0);
  //bossLevels[NB_BOSS]
  stateGameOver(false);
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
  /*
  else if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }*/
}

#endif
