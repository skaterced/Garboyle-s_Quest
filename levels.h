#ifndef LEVELS_H
#define LEVELS_H

#include <Arduino.h>
#include "globals.h"
#include "enemies.h"
//#include "Point.h"
#include "player.h"

#define NB_BOSS 5 //the last one is reserved for the cheat
/*
#define LSTART  0
#define LFINISH 1 << 5
#define LBAT    2 << 5
#define LFAN    3 << 5
#define LSPIKES 4 << 5
#define LCOIN   5 << 5
#define LKEY    6 << 5
*/

//char gameGrid[LEVEL_ARRAY_SIZE]; // grid with cell information
// upper byte tile xxxx ____
// LSB solid ____ ___x

class BossRew {
  public:
    uint8_t lvl;
    bool alive;
    uint8_t reward;
    BossRew(uint8_t l, uint8_t r){
      lvl=l;
      reward=r;
      alive=true;
    }
};

class BossRew bossLevels[NB_BOSS]={BossRew(3,UPGRADE_FIRE),BossRew(3,UPGRADE_ARMOR),BossRew(2,UPGRADE_JUMP),BossRew(7,UPGRADE_ARMOR),BossRew(99,0)};

void BossLvlCheck(){
  for (uint8_t i=0; i<NB_BOSS; i++){
    if (bossLevels[i].lvl==level){
      if (bossLevels[i].alive){
        bossRoom = true;
      }
      else {
        enemiesInit(false);
      }
      break;
    }
  }
}

uint8_t getBossReward(bool clearReward){
  for (uint8_t i=0; i<NB_BOSS; i++){
    if ((true==bossLevels[i].alive)&&(bossLevels[i].lvl==level)){
      if (clearReward){
        bossLevels[i].alive=false;
        clearReward=false;
      }
      else
        return (bossLevels[i].reward);
    }
  }
  return 0;
}


// Cell based grid checking
bool gridGetSolid(int8_t x, int8_t y) {
  if (x < 0 || x >= LEVEL_WIDTH_CELLS)
    return 1;

  if (y < 0 || y >= LEVEL_HEIGHT_CELLS)  //no more y limitation?
    return indorLevel;

  const uint8_t *lvl = levels[level];
  lvl++; //parameters read
  byte b = pgm_read_byte(lvl + (x >> 3) + (y * (LEVEL_WIDTH_CELLS >> 3)));
  return ((b >> (x % 8)) & 0x01);
}

void clearExits(){
  for (uint8_t i=0;i<MAX_DOORS;i++){
    levelExits[i].pos=vec2(-1,-1);
    levelExits[i].destination=19; //just checking....
  }
}

void levelLoad(const uint8_t *lvl) {
  clearExits();
  
  byte i = 0;
  lvlSettings=pgm_read_byte(lvl);
  if (0x80==(lvlSettings&0x80))
    indorLevel=false;
  else
    indorLevel=true; //most of them will be I guess
  switch((lvlSettings&0x70)>>4){
    case 1:
      level_width_cell = 8;
      level_height_cell = 8;
    break;
    case 2:
      level_width_cell = 16;
      level_height_cell = 8;
    break;
    case 3:
      level_width_cell = 24;
      level_height_cell = 16;
    break;
    case 4:
      level_width_cell = 32;
      level_height_cell = 16;
      //indorLevel=false;
    break;
    case 5:
      level_width_cell = 8;
      level_height_cell = 32;
    break;
    default:
      level_width_cell = 24;
      level_height_cell = 24;
      //indorLevel=false;
  }
  lvl += 1 + (LEVEL_ARRAY_SIZE >> 3);

  byte b = pgm_read_byte(lvl+i);
  
  while (b != 0xFF)
  {
    byte id, x, y;  //h?
    id = pgm_read_byte(lvl + i) & 0xE0;
    y = (pgm_read_byte(lvl + i++) & 0x1F);
    x = pgm_read_byte(lvl + i++) & 0x1F;
    switch ( id )
    {
      case LSTART:
        {
          // Start
          startPos.x = (int)x << (FIXED_POINT + 4);
          startPos.y = (int)y << (FIXED_POINT + 4);
          //kid.actualpos.x = (int)x << (FIXED_POINT + 4);
          //kid.actualpos.y = (int)y << (FIXED_POINT + 4);
          kid.actualpos = startPos;
        }
        break;
      case LFINISH:
        {
          // Finish
          for (uint8_t j=0;j<MAX_DOORS;j++){
            if (-1==levelExits[j].pos.x){
              levelExits[j].pos.x = x << 4;
              levelExits[j].pos.y = y << 4;
              levelExits[j].destination = pgm_read_byte(lvl + i++);
              break;
            }
          }
        }
        break;
      case LBAT:
        {
          // Bat
          batsCreate(vec2(x, y));
        }
        break;
      case LBOSS:
        {
          // Boss
          uint8_t temp = (pgm_read_byte(lvl + (i - 1))&0xE0)>>5;
          switch (temp) {
            case 0:
              ghostsCreate(vec2(x, y),false);
              break;
            case 1:
              ghostsCreate(vec2(x, y),true); //can fire
              break;
            case 3:
              sunCreate(vec2(x,y));
              break;
              
          }
        }
        break;        
      case LSPIKES:
        {
          // Spikes
          spikesCreate(vec2(x, y), pgm_read_byte(lvl + (i - 1)) >> 5);
        }
        break;
      case LTRAP:
      {
        spitterCreate(vec2(x, y));
      }
      break;
      case LCOIN:
        {
          // Coins
          coinsCreate(vec2(x, y));
        }
        break;
      default: //case LKEY:
        {
          // Key
//          keyCreate(vec2(x, y));
        }
        break;
      //default:
        //break;
    }

    b = pgm_read_byte(lvl + i);
  }
  if (wichEntrance>0){ // must start in an exit
    //startPos=levelExits[wichEntrance-1].pos;
    //kid.actualpos = startPos;
    kid.actualpos.x = levelExits[wichEntrance-1].pos.x<<5;
    kid.actualpos.y = levelExits[wichEntrance-1].pos.y<<5;
    startPos = kid.actualpos;
  }
  BossLvlCheck();
}

void drawGrid() {
  //Serial.println("Start of tile drawing");
  int spacing = 16;
  for (byte x = 8; x < 9; --x)
  {
      for (byte y = 5; y < 6; --y)
      {
            //sprites.drawSelfMasked(x * spacing - (cam.pos.x >> 2) % spacing, (int)y * spacing - (cam.pos.y >> 2) % spacing, tileSetTwo, 16);
            sprites.drawSelfMasked(x * spacing - (cam.pos.x >> 2) % spacing, (int)y * spacing - ((cam.pos.y + 64) >> 2) % spacing, tileSetTwo, 16);
      }
  }
  for ( int x = (cam.pos.x >> 4); x <= (cam.pos.x >> 4) + 8; ++x)
  {
    for ( int y = (cam.pos.y >> 4); y <= (cam.pos.y >> 4) + 4; ++y)
    {
      {
        //byte tile = gridGetTile(x, y);
        if (gridGetSolid(x,y))
          sprites.drawOverwrite((x << 4) - cam.pos.x, (y << 4) - cam.pos.y, tileSetTwo, lvlSettings&0x0F);
      }
    }
  }

  int commonx;
  int commony;
  for (uint8_t i=0; i<MAX_DOORS; i++){
    if (-1!=levelExits[i].pos.y){
      commonx = levelExits[i].pos.x - cam.pos.x;
      commony = levelExits[i].pos.y - cam.pos.y;
    
      sprites.drawOverwrite(commonx, commony, door, 0);// (key.haveKey));
    }
  }
}

void windNoise()
{
  //if (arduboy.everyXFrames(2)) sound.tone(320 + random(20), 30);
}

void kidHurt()
{
  if (kid.hearts == 0)
    return;
  /*if (kid.hearts == 1)
  {
    // dead
    gameState = STATE_GAME_OVER;
  }
  else
  {*/
  #ifndef CHEAT
    kid.isFlying = false;
    kid.isClimbing = false;
    kid.hearts--;
    //sound.tone(420, 100);
    kid.isImune = true;
    kid.imuneTimer = 0;
  //}
  #endif
}

void checkCollisions()
{
  if (kid.hearts == 0)
    return;

  HighRect playerRect = {.x = kid.pos.x + 2, .y = kid.pos.y + 2, .width = 8, .height = 12};
  //HighRect playerSuckRect = {.x = kid.pos.x + ((kid.direction ^ 1) * 16) - (kid.direction * 16), .y = kid.pos.y + 2, .width = 16, .height = 14};

  // Key
/*  HighRect keyRect = {.x = key.pos.x, .y = key.pos.y, .width = 8, .height = 16};
  if (collide(keyRect, playerRect) && key.active)
  {
    key.active = false;
    key.haveKey = true;
    //sound.tone(420, 200);
  }*/

  // Level exit
  for (uint8_t i=0; i<MAX_DOORS ; i++){
    if (-1!=levelExits[i].pos.y){
      HighRect exitRect = {.x = levelExits[i].pos.x + 4, .y = levelExits[i].pos.y, .width = 8, .height = 16};
      if ((collide(exitRect, playerRect) && arduboy.justPressed(UP_BUTTON) ) && false==bossRoom) //one does not simply leave the room whithout killing the Boss
      {
        //balloonsLeft = kid.hearts;
        scoreIsVisible = true;        
        level=levelExits[i].destination&0x1F;        
        wichEntrance=((levelExits[i].destination&0xE0)>>5);
        gameState = STATE_GAME_NEXT_LEVEL;
      }
    }
  }

    // Heart Bonus
    if (heartBonus/*coins[i]*/.active)
    {
      HighRect coinrect = {.x = heartBonus.pos.x, .y = heartBonus.pos.y, .width = 10, .height = 12};
      if (collide(playerRect, coinrect))
      {
        if (kid.hearts<heartsMax){
          heartBonus.active=false;
          kid.hearts++;
        }
      }
    }

  // Enemies and objects
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    // Bats
    if (bats[i].active)
    {
      HighRect ennemiRect = {.x = bats[i].pos.x, .y = bats[i].pos.y, .width = 10, .height = 8};
      for (uint8_t j=0; j<MAX_WEAPON; j++){
        if (kid.fireBalls[j].isActive){
          HighRect projectileRect = {.x = kid.fireBalls[j].pos.x-firePower, .y = kid.fireBalls[j].pos.y-firePower, .width = 1+2*(firePower), .height = 1+2*(firePower)};
          if (collide(projectileRect, ennemiRect))
          {
            bats[i].HP-=firePower;
            kid.fireBalls[j].isActive=false;
            //bats[i].hurt = true;
            /*if (bats[i].HP <= 0) {
              bats[i].active = false;
            }*/
          }
          //else
            //bats[i].hurt = false;
        }
      }
      // Hurt player
      if (collide(playerRect, ennemiRect) && bats[i].HP > 0 && !kid.isImune)
      {
        kidHurt();
        kid.speed.y = jumpVelocity/2;
        kid.speed.x = max(min((kid.pos.x - bats[i].pos.x - 2), 2), -2) << FIXED_POINT;
      }      
    }

    if (ghosts[i].active)
    {
      HighRect ennemiRect = {.x = ghosts[i].pos.x, .y = ghosts[i].pos.y, .width = 12, .height = 16};
      for (uint8_t j=0; j<MAX_WEAPON; j++){
        if (kid.fireBalls[j].isActive){
          HighRect projectileRect = {.x = kid.fireBalls[j].pos.x-firePower, .y = kid.fireBalls[j].pos.y-firePower, .width = 1+2*(firePower), .height = 1+2*(firePower)};
          if (collide(projectileRect, ennemiRect))
          {
            ghosts[i].HP-=firePower;
            kid.fireBalls[j].isActive=false;
            //ghosts[i].hurt = true;
          }
          //else
            //ghosts[i].hurt = false;
        }
      }
      // Hurt player
      if (collide(playerRect, ennemiRect) && ghosts[i].HP > 0 && !kid.isImune)
      {
        kidHurt();
        kid.speed.y = jumpVelocity/2;
        kid.speed.x = max(min((kid.pos.x - ghosts[i].pos.x - 2), 2), -2) << FIXED_POINT;
      }      
    }
    //bullets
    if (ennemiBullets[i].isActive){
      HighRect ennemiProjectileRect = {.x = ennemiBullets[i].pos.x, .y = ennemiBullets[i].pos.y, .width = 3, .height = 3};
      if (collide(playerRect, ennemiProjectileRect) && !kid.isImune){
        kidHurt();
        ennemiBullets[i].isActive=false;
      }
    }
    // Spikes
    if (!kid.isImune && bitRead(spikes[i].characteristics, 2) && collide(playerRect, spikes[i].pos))
    {
      kidHurt();
      if (kid.pos.y < spikes[i].pos.y) kid.speed.y = jumpVelocity;
    }
  }
  if (sun.active){
    HighRect ennemiRect = {.x = sun.pos.x, .y = sun.pos.y, .width = 16, .height = 16};
    for (uint8_t j=0; j<MAX_WEAPON; j++){
      if (kid.fireBalls[j].isActive){
        HighRect projectileRect = {.x = kid.fireBalls[j].pos.x-firePower, .y = kid.fireBalls[j].pos.y-firePower, .width = 1+2*(firePower), .height = 1+2*(firePower)};
        if (collide(projectileRect, ennemiRect))
        {
          sun.HP-=firePower;
          kid.fireBalls[j].isActive=false;
        }
      }
    }
    // Hurt player
    if (collide(playerRect, ennemiRect) && sun.HP > 0 && !kid.isImune)
    {
      kidHurt();
      kid.speed.y = jumpVelocity/2;
      kid.speed.x = max(min((kid.pos.x - sun.pos.x - 2), 2), -2) << FIXED_POINT;
    }      
  }
}

void drawHUD()
{
  //for (byte i = 0; i < 16; i++)

  arduboy.fillRect(0,0,7,64,1);
  arduboy.drawLine(1,62,1,2+60-kid.wingsJauge,0);
  //drawNumbers(91, 0, FONT_SMALL, DATA_SCORE); //for test
  drawLives();
}
#endif
