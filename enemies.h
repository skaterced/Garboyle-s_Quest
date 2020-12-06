#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
//#include "vec2.h"

#define MAX_FAN_PARTICLES 4
#define FAN_POWER           5
#define FAN_UP              0
#define FAN_RIGHT           1
#define FAN_LEFT            2

#define SPIKES_LEFT         0
#define SPIKES_DOWN         1
#define SPIKES_RIGHT        2
#define SPIKES_UP           3

#define MAX_ENNEMI_SPEED_X 1<<5
#define MAX_ENNEMI_SPEED_Y 1<<5
#define GHOST_ACC 1

struct Coin
{
  vec2 pos;
  bool active;
};

Coin coins[MAX_PER_TYPE];

struct Key
{
  vec2 pos;
  bool active;
  bool haveKey;
};

Key key = {.pos = vec2(0, 0), .active = false, .haveKey = false};

struct Bat
{
  vec2 pos;
  bool direction; //true if heading right
  int8_t HP;
  bool hurt;
  bool active;
};

Bat bats[MAX_PER_TYPE];

struct Ghost
{
  vec2 pos;
  vec2 speed;
  bool direction; //true if heading right  
  int8_t HP;
  bool hurt;
  bool active;
};

Ghost ghosts[MAX_PER_TYPE];

struct Spike
{
  HighRect pos;
  byte characteristics;//B00000000;   //this byte holds all the enemies characteristics
  //                      ||||||||
  //                      |||||||└->  0 \ these 2 bits are used to determine the spike type
  //                      ||||||└-->  1 /  
  //                      |||||└--->  2   the spike is active    (0 = false / 1 = true)
};

Spike spikes[MAX_PER_TYPE];

void enemiesInit(bool everything)
{
  coinsActive = 0;
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (everything){
      // Spikes
      spikes[i].pos.x = 0;
      spikes[i].pos.y = 0;
      spikes[i].pos.width = 16;
      spikes[i].pos.height = 16;
      
      spikes[i].characteristics = 1;
      
      // Coins
      coins[i].pos.x = 0;
      coins[i].pos.y = 0;
      coins[i].active = false;
    }    
    // Bats
    bats[i].pos.x = 0;
    bats[i].pos.y = 0;
    bats[i].active = false;
    bats[i].HP = 15;
    bats[i].direction = true;
    bats[i].hurt = false;
  
    // Ghosts
    ghosts[i].pos.x = 0;
    ghosts[i].pos.y = 0;
    ghosts[i].speed.x = 0;
    ghosts[i].speed.y = 0;
    ghosts[i].active = false;
    ghosts[i].HP = 50;
    ghosts[i].direction = true;
    ghosts[i].hurt = false;       
  }
}
void enemiesInit(){
  enemiesInit(true);
}
void coinsCreate(vec2 pos)
{
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!coins[i].active)
    {
      ++coinsActive;
      coins[i].pos = pos << 4;
      coins[i].pos.x += 2;
      coins[i].active = true;
      return;
    }
  }
}

void keyCreate(vec2 pos)
{
  key.pos = pos << 4;
  key.active = true;
  key.haveKey = false;
}

void batsCreate(vec2 pos)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  //for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!bats[i].active)
    {
      bats[i].pos = pos << 4;
      bats[i].pos.y += 4;
      bats[i].active = true;
      return;
    }
  }
}

void ghostsCreate(vec2 pos)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  //for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!ghosts[i].active)
    {
      ghosts[i].pos = pos << 4;
      ghosts[i].pos.y += 4;
      ghosts[i].active = true;
      return;
    }
  }
}

void spikesCreate(vec2 pos, byte l)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!bitRead(spikes[i].characteristics, 2))
    {
      int len = 16 * (l + 1);
      spikes[i].pos.x = pos.x << 4;
      spikes[i].pos.y = pos.y << 4;
      
      // Solid below
      if (gridGetSolid(pos.x, pos.y + 1))
      {
        spikes[i].characteristics = B00000101;
        spikes[i].pos.width = len;
        spikes[i].pos.height = 8;
        spikes[i].pos.y += 8;
      }
      // Solid left
      else if (gridGetSolid(pos.x - 1, pos.y))
      {
        spikes[i].characteristics = B00000100;
        spikes[i].pos.width = 8;
        spikes[i].pos.height = len;
      }
      // Solid right
      else if (gridGetSolid(pos.x + 1, pos.y))
      {
        spikes[i].characteristics = B00000110;
        spikes[i].pos.width = 8;
        spikes[i].pos.height = len;
        spikes[i].pos.x += 8;
      }
      // Solid above
      else if (gridGetSolid(pos.x, pos.y - 1))
      {
        spikes[i].characteristics = B00000111;
        spikes[i].pos.width = len;
        spikes[i].pos.height = 8;
      }
      return;
    }
  }
}

void enemiesUpdate()
{
  if (arduboy.everyXFrames(6))
  {
    walkerFrame = (++walkerFrame) % 2;
    //coinFrame = (++coinFrame) % 4;
  }

  if (key.active)
  {
    int commonx = key.pos.x - cam.pos.x;
    int commony = key.pos.y - cam.pos.y;
    //sprites.drawOverwrite(commonx, commony, elements, 1);
    arduboy.fillCircle(commonx, commony, 4);
  }

  uint8_t ennemiesLeft=0;
  // Draw spikes first  
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (bitRead(spikes[i].characteristics, 2)) // spike active
    {
      int commonx = spikes[i].pos.x - cam.pos.x;
      int commony = spikes[i].pos.y - cam.pos.y;
      sprites.drawOverwrite(commonx, commony, sprSpikes,  spikes[i].characteristics & B00000011);
      //sprites.drawErase(commonx, commony, sprSpikes,  spikes[i].characteristics & B00000011);
      if (!bitRead(spikes[i].characteristics, 0)) {
        for (int l = 8; l < spikes[i].pos.height; l += 8)
          sprites.drawOverwrite(commonx, commony + l, sprSpikes,  spikes[i].characteristics & B00000011);
      }
      else {
        for (int l = 8; l < spikes[i].pos.width; l += 8)
          sprites.drawOverwrite(commonx + l, commony, sprSpikes,  spikes[i].characteristics & B00000011);
      }
    }
  }

//  if (arduboy.everyXFrames(4)) fanFrame = (++fanFrame) % 3;
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {

    // Bats
    if (bats[i].active)
    {
      ennemiesLeft++;
      if (arduboy.everyXFrames(2) && bats[i].HP > 0 )
      {
        if (!gridGetSolid((bats[i].pos.x + (bats[i].direction? 9:0)) >> 4, bats[i].pos.y >> 4))
            /*&& gridGetSolid((bats[i].pos.x + 4 + (bats[i].direction * 5)) >> 4, (bats[i].pos.y >> 4) + 1))*/
        {
          bats[i].pos.x += bats[i].direction? 1:-1;
        }
        else
        {
          bats[i].direction = !bats[i].direction;
        }
      }

      if (bats[i].HP>0){        
        sprites.drawSelfMasked(bats[i].pos.x - cam.pos.x, bats[i].pos.y - cam.pos.y, BatSprite, walkerFrame + (bats[i].HP <= 0) * 2);
        // Bat's eyes
        arduboy.drawPixel(bats[i].pos.x - cam.pos.x + (bats[i].direction? 4:3), bats[i].pos.y - cam.pos.y+3,0);
        arduboy.drawPixel(bats[i].pos.x - cam.pos.x + (bats[i].direction? 6:5), bats[i].pos.y - cam.pos.y+3,0);
      }
      else{
        bats[i].HP--;
        if (arduboy.everyXFrames(2))
          sprites.drawSelfMasked(bats[i].pos.x - cam.pos.x, bats[i].pos.y - cam.pos.y, BatSprite, 2);
        if (bats[i].HP<-30){
          bats[i].active=false;
        }
      }
    }

    // Ghosts
    //25746 - 25758
    if (ghosts[i].active)
    {
      ennemiesLeft++;
      HighRect ennemiRect = {.x = ghosts[i].pos.x, .y = ghosts[i].pos.y, .width = 100, .height = 60};
      HighRect kidPoint = {.x=kid.pos.x, .y=kid.pos.y, .width=100,.height=60}; 
      if (collide(kidPoint,ennemiRect) && ghosts[i].HP > 0){
        //23714 - 23722
        if (((0x04==(globalCounter&0x07))||(0x10==(globalCounter&0x31))||(0x17==(globalCounter&0x3F))) && (0x40!=(globalCounter&0x60))  ){
        //if (arduboy.everyXFrames(5-((globalCounter&0x30)>>4))){
          ghosts[i].direction = ghosts[i].pos.x<kid.pos.x;
          if ((kid.pos.x-ghosts[i].pos.x)>2){
            ghosts[i].pos.x++;
          }
          else if ((ghosts[i].pos.x-kid.pos.x)>2){
            ghosts[i].pos.x--;
          }
          if ((kid.pos.y-ghosts[i].pos.y)>2){
            ghosts[i].pos.y++;
          }
          else if ((ghosts[i].pos.y-kid.pos.y)>2){
            ghosts[i].pos.y--;
          }
        }
      }
      arduboy.fillRect(ghosts[i].pos.x - cam.pos.x + 1 , ghosts[i].pos.y - cam.pos.y + 3, 10, 7, 0);
      if (ghosts[i].HP>0){        
        sprites.drawSelfMasked(ghosts[i].pos.x - cam.pos.x, ghosts[i].pos.y - cam.pos.y, GhostSprite, ((0x40==(globalCounter&0x60))? 1:0) + ghosts[i].direction * 2);
      }
      else{
        ghosts[i].HP--;
        if (arduboy.everyXFrames(2))
          sprites.drawSelfMasked(ghosts[i].pos.x - cam.pos.x, ghosts[i].pos.y - cam.pos.y, GhostSprite, 4);
        if (ghosts[i].HP<-30){
          ghosts[i].active=false;
        }
      }
    }    

    // Coins
    if (coins[i].active)
    {
      sprites.drawOverwrite(coins[i].pos.x - cam.pos.x, coins[i].pos.y - cam.pos.y, elements, 0);
    }
  }
  if (bossRoom&&(0==ennemiesLeft)){
    //LvlUp
    gameState=STATE_GAME_LVLUP;
  }
}


#endif
