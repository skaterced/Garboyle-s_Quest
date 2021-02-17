#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
#include "weapon.h"
#include "player.h" //for the camera
//#include "vec2.h"
//#include "boss.h"

#define SPIKES_LEFT         0
#define SPIKES_DOWN         1
#define SPIKES_RIGHT        2
#define SPIKES_UP           3

#define MAX_ENNEMI_SPEED_X 1<<5
#define MAX_ENNEMI_SPEED_Y 1<<5
#define GHOST_ACC 1

Weapon ennemiBullets[MAX_PER_TYPE];

struct Coin
{
  vec2 pos;
  bool active;
};

//Coin coins[MAX_PER_TYPE];
Coin heartBonus; //because there's never more than one per level
/*
struct Key
{
  vec2 pos;
  bool active;
  bool haveKey;
};*/

//Key key = {.pos = vec2(0, 0), .active = false, .haveKey = false};

struct Bat
{
  vec2 pos;
  bool direction; //true if heading right
  int8_t HP;
  //bool hurt;
  bool active;
};

Bat bats[MAX_PER_TYPE];

struct Ghost
{
  vec2 pos;
  vec2 speed;
  bool direction; //true if heading right 
  bool canShoot;
  int8_t HP;
  //bool hurt;
  bool active;
};
Ghost ghosts[MAX_PER_TYPE];

struct Trap
{
  vec2 pos;
  bool direction; //true if heading right
  //byte type; // if more than the spitter (could also contains the "firing freq")
  bool active;
};

Trap traps[MAX_PER_TYPE];


struct Sun
{
  vec2 pos;
  //vec2 speed;
  uint8_t direction; //0 is north then clockwise
  bool clockwise;
  int8_t HP;
  //bool hurt;
  bool active;
};

Sun sun; //there could only be one per level.

struct Wizard
{
  vec2 pos;
  //bool clockwise;
  int8_t HP;
  uint8_t state; // firing, orientation, position
  uint8_t wiz_timer;
  bool active;
};

/* wizard pos. (state & 0x05)
 *     4-----3
 *   /   \ /   \
 *  2-----0-----1
 *   \   / \   /
 *     5-----6
 */

Wizard wizard; //there could only be one per level.

struct Faceless
{
  vec2 pos;
  bool direction;
  int8_t HP;
  uint8_t state; // firing, orientation, position
  uint8_t timer;
  bool active;
};

/* Faceless pos. (state & 0x02)
 *  0--------0
 *    /\  / \  
 *  1---*----1
 *    \/  \ / 
 *  2--------2
 */

Faceless faceless; //there could only be one per level.

//vec2 circleMove[8]={vec2(0,-2),vec2(1,-1),vec2(2,0),vec2(1,1),vec2(0,2),vec2(-1,1),vec2(-2,0),vec2(-1,-1)};
int8_t circleMove[16]={0,1,2,3,3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1};

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
  heartBonus.pos.x = 0;
  heartBonus.pos.y = 0;
  heartBonus.active = false;
      
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (everything){
      // Spikes
      spikes[i].pos.x = 0;
      spikes[i].pos.y = 0;
      spikes[i].pos.width = 16;
      spikes[i].pos.height = 16;      
      spikes[i].characteristics = 1;

      //traps
      traps[i].pos.x=0;
      traps[i].pos.y=0;
      traps[i].active=false;

      /*
      // Coins
      coins[i].pos.x = 0;
      coins[i].pos.y = 0;
      coins[i].active = false;*/
    }
    
    // Bats
    bats[i].pos.x = 0;
    bats[i].pos.y = 0;
    bats[i].active = false;
    bats[i].HP = 3;
    bats[i].direction = true;
  
    // Ghosts
    ghosts[i].pos.x = 0;
    ghosts[i].pos.y = 0;
    ghosts[i].speed.x = 0;
    ghosts[i].speed.y = 0;
    ghosts[i].active = false;
    ghosts[i].HP = 12;  // 19 if "can shoot"
    ghosts[i].direction = true;

    ennemiBullets[i].isActive=false;
  }
    //Sun
  sun.pos.x=0;
  sun.pos.y=0;
  sun.direction=0;
  sun.HP=30;
  sun.active=false;

  //Wizard
  wizard.pos.x=0;
  wizard.pos.y=0;
  wizard.state=0;
  wizard.wiz_timer=100;
  wizard.HP=60;
  wizard.active=false;

   //faceless
  faceless.pos.x=0;
  faceless.pos.y=0;
  faceless.direction=FACING_LEFT;
  faceless.state=2;
  faceless.timer=200;
  faceless.HP=35;
  faceless.active=false; 
// state info:         B0SSSMMLL;  
//                      ||||||\|
//                      |\||\| └-> L: these 2 bits are used to determine the line (0 is top, 2 is bottom)
//                      | \| └-->  M: these 2 bits are used to determine the moving direction (0 is going up, 2 is going down)   
//                      |  └--->   S: these 3 bits are used to determine the state. 
//                      └->        not used (could be direction)

}
void enemiesInit(){
  enemiesInit(true);
}

void coinsCreate(vec2 pos)
{
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  //for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!heartBonus.active)
    {
      //++coinsActive;
      heartBonus.pos = pos << 4;
      heartBonus.pos.x += 2;
      heartBonus.active = true;
      return;
    }
  }
}
/*
void keyCreate(vec2 pos)
{
  key.pos = pos << 4;
  key.active = true;
  key.haveKey = false;
}*/

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
void ghostsCreate(vec2 pos, bool canShoot_)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  //for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!ghosts[i].active)
    {
      ghosts[i].pos = pos << 4;
      //ghosts[i].pos.y += 4;
      ghosts[i].active = true;
      ghosts[i].canShoot = canShoot_;
      if (canShoot_)
        ghosts[i].HP = 19;
      return;
    }
  }
}

void sunCreate(vec2 pos) // can only be alone
{
  sun.pos = pos << 4;
  sun.active = true;
  sun.direction =true;
}

void wizardCreate(vec2 pos) // can only be alone
{
  wizard.pos = pos << 4;
  wizard.active = true;
}

void facelessCreate(vec2 pos) // can only be alone
{
  faceless.pos = pos << 4;
  faceless.active = true;
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

void spitterCreate (vec2 pos){
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!traps[i].active)
      {
        traps[i].pos = pos << 4;
        traps[i].pos.y += 4;
        traps[i].active = true;
        // Solid left
        if (gridGetSolid(pos.x - 1, pos.y))
        {
          traps[i].direction=true;
        }
        // Solid right
        else if (gridGetSolid(pos.x + 1, pos.y))
        {
          traps[i].direction=false;
          traps[i].pos.x += 8;
        }
        break;
      }
    }
}

void enemiesUpdate()
{
  randomSeed(globalCounter);
  uint8_t ennemiesLeft=0;
  // Draw spikes first  
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
     //traps
    if (traps[i].active){
      int commonx = traps[i].pos.x - cam.pos.x;
      int commony = traps[i].pos.y - cam.pos.y;
      sprites.drawOverwrite(commonx, commony, SpitterSprite, traps[i].direction);
      if (arduboy.everyXFrames(150)){
        for (uint8_t j=0; j<MAX_PER_TYPE; j++){
            if (!ennemiBullets[j].isActive){
              ennemiBullets[j].isActive=true;
              ennemiBullets[j].radius = 0;
              ennemiBullets[j].pos.y=traps[i].pos.y+5;
              //ennemiBullets[j].pos.x=ghosts[i].pos.x+4;
              ennemiBullets[j].actualpos.x=(traps[i].pos.x+6)<<5;//+64;
              //bool dir = direction;            
              ennemiBullets[j].speed.x= (traps[i].direction? 1:-1)*WEAPON_SPEED;
              //ennemiBullets[j].speed.y=0;
              ennemiBullets[j].timer=SHOT_TIMER;
              break;
            }
        }
      }
    }
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
        {
          bats[i].pos.x += bats[i].direction? 1:-1;
        }
        else
        {
          bats[i].direction = !bats[i].direction;
        }
      }

      if (bats[i].HP>0){        
        sprites.drawSelfMasked(bats[i].pos.x - cam.pos.x, bats[i].pos.y - cam.pos.y, BatSprite, (globalCounter&(0x04))>>2 + (bats[i].HP <= 0) * 2);
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
    if (ghosts[i].active)
    {
      ennemiesLeft++;
      HighRect ennemiRect = {.x = ghosts[i].pos.x, .y = ghosts[i].pos.y, .width = 100, .height = 70};
      HighRect kidPoint = {.x=kid.pos.x, .y=kid.pos.y, .width=100,.height=70}; 
      if (collide(kidPoint,ennemiRect) && ghosts[i].HP > 0){
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
        if (arduboy.everyXFrames(150)&&ghosts[i].canShoot){  //ghost is firing
          for (uint8_t j=0; j<MAX_PER_TYPE; j++){
            if (!ennemiBullets[j].isActive){
              ennemiBullets[j].isActive=true;
              ennemiBullets[j].radius = 0;
              ennemiBullets[j].pos.y=ghosts[i].pos.y+4;
              //ennemiBullets[j].pos.x=ghosts[i].pos.x+4;
              ennemiBullets[j].actualpos.x=(ghosts[i].pos.x+6)<<5;//+64;
              //bool dir = direction;            
              ennemiBullets[j].speed.x= (ghosts[i].direction? 1:-1)*WEAPON_SPEED;
              //ennemiBullets[j].speed.y=0;
              ennemiBullets[j].timer=SHOT_TIMER;
              break;
            }
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
  
    // bullets
    if (ennemiBullets[i].isActive){
      ennemiBullets[i].check();
      ennemiBullets[i].draw(cam.pos);
    }
  } // end of "for (i..."

    // Coins
    if (heartBonus.active)
    {
      sprites.drawOverwrite(heartBonus.pos.x - cam.pos.x, heartBonus.pos.y - cam.pos.y, elements, 0);
    }

  //Sun
  if (sun.active)
  {
    ennemiesLeft++;
    HighRect ennemiRect = {.x = sun.pos.x-8, .y = sun.pos.y-8, .width = 100, .height = 70};
    HighRect kidPoint = {.x=kid.pos.x, .y=kid.pos.y, .width=100,.height=70}; 
    if (collide(kidPoint,ennemiRect) && sun.HP > 0){        
      if (0==globalCounter%3){
        sun.pos.x+=circleMove[sun.direction];
        sun.pos.y+=circleMove[(sun.direction+4)%16];
        if ((gridGetSolid(sun.pos.x-8 >> 4 ,sun.pos.y-8 >> 4))){
          sun.direction=(sun.direction+8)%16;
          sun.pos.x+=circleMove[sun.direction];
          sun.pos.y+=circleMove[(sun.direction+4)%16];
        }
        else if (random(100)<5){
          sun.clockwise=!sun.clockwise;
        }          
        if (0==globalCounter%9){
          if (sun.clockwise){
            if (++sun.direction>15)
              sun.direction=0;
          }
          else{
            if (sun.direction--==0)
              sun.direction=15;
          }
        }
      }
    }
    if (sun.HP>0)
      sprites.drawSelfMasked(sun.pos.x-8 - cam.pos.x, sun.pos.y-8 - cam.pos.y, SunSprite, (globalCounter&0x02)>>1);
    else{
      sun.HP--;
      if (arduboy.everyXFrames(2))
        sprites.drawSelfMasked(sun.pos.x-8 - cam.pos.x, sun.pos.y-8 - cam.pos.y, SunSprite, 2);
      if (sun.HP<-30){
        sun.active=false;
      }
    }
  }

  //Wizard
  if (wizard.active)
  {
    bool direction = (wizard.pos.x<kid.pos.x);
    ennemiesLeft++;
    HighRect ennemiRect = {.x = wizard.pos.x, .y = wizard.pos.y, .width = 100, .height = 70};
    HighRect kidPoint = {.x=kid.pos.x, .y=kid.pos.y, .width=100,.height=70}; 
    if (collide(kidPoint,ennemiRect) && wizard.HP > 0){   
      //uint8_t wizRand = random(100);      
      if (--wizard.wiz_timer==0){
        uint8_t wizRand = random(100);
        if (0!=(wizard.state&0x38)){
          wizard.state&=0xC7; //mask orientation
          wizard.wiz_timer=50; // wait a bit
        }
        else {
          if ((wizRand<20)&&(0==(wizard.state&0x40))){ //wait          
            wizard.state|=0x40;
            wizard.wiz_timer=100;
          }
          else if ((wizRand<40)&&(0==(wizard.state&0x80))){ //fire            
            wizard.state|=0x80;
            wizard.wiz_timer=100; //actual firing when timer is at 50
          }
          else { //move
            wizard.wiz_timer=80;
            wizRand = random(60);
            switch (wizard.state&0x07){
              case 0:
                wizRand=wizRand/10+1;
                wizard.state = (wizRand|(wizRand<<3));
              break;
              case 1:
                if (wizRand<20)
                  wizard.state = (3|(4<<3));
                else if (wizRand<40)
                  wizard.state = (0|(2<<3));
                else
                  wizard.state = (6|(5<<3));
              break;
              case 2:
                if (wizRand<20)
                  wizard.state = (4|(3<<3));
                else if (wizRand<40)
                  wizard.state = (0|(1<<3));
                else
                  wizard.state = (5|(6<<3));
              break;
              case 3:
                if (wizRand<20)
                  wizard.state = (4|(2<<3));
                else if (wizRand<40)
                  wizard.state = (0|(5<<3));
                else
                  wizard.state = (1|(6<<3));
              break;
              case 4:
                if (wizRand<20)
                  wizard.state = (3|(1<<3));
                else if (wizRand<40)
                  wizard.state = (0|(6<<3));
                else
                  wizard.state = (2|(5<<3));
              break;
              case 5:
                if (wizRand<20)
                  wizard.state = (2|(4<<3));
                else if (wizRand<40)
                  wizard.state = (0|(3<<3));
                else
                  wizard.state = (6|(1<<3));
              break;
              case 6:
                if (wizRand<20)
                  wizard.state = (5|(2<<3));
                else if (wizRand<40)
                  wizard.state = (0|(4<<3));
                else
                  wizard.state = (1|(3<<3));
              break;
              //default:
                // should never happens, unknown position
                //arduboy.fillCircle(wizard.pos.x - cam.pos.x  , wizard.pos.y - cam.pos.y ,5,1);
              
            }
          }
        }
      }
      else { // wiz_timer is running
        if (0!=(wizard.state&0x80)&&(wizard.wiz_timer==50)){
          for (uint8_t j=0; j<MAX_PER_TYPE; j++){              
              if (!ennemiBullets[j].isActive){
                ennemiBullets[j].isActive=true;
                ennemiBullets[j].radius = 2;
                ennemiBullets[j].pos.y=wizard.pos.y+9;                
                ennemiBullets[j].actualpos.x=(wizard.pos.x+5*direction)<<5;//+64; // to change
                //bool dir = direction;            
                ennemiBullets[j].speed.x= (direction? 1:-1)*WEAPON_SPEED;
                //ennemiBullets[j].speed.y=0;
                ennemiBullets[j].timer=SHOT_TIMER;
                break;
              }
            }
        }
        if (0==globalCounter%2){
          switch ((wizard.state&0x38)>>3) { //moving
            case 1:
              wizard.pos.x+=2;
            break;
            case 2:
              wizard.pos.x-=2;
            break;
            case 3:
              wizard.pos.x+=1;
              wizard.pos.y-=1;
            break;
            case 4:
              wizard.pos.x-=1;
              wizard.pos.y-=1;
            break;
            case 5:
              wizard.pos.x-=1;
              wizard.pos.y+=1;
            break;
            case 6:
              wizard.pos.x+=1;
              wizard.pos.y+=1;
            break;
          }
        }
      }
    }
    if (wizard.HP>0){
      if (0==(wizard.state&0x80)){
        arduboy.fillCircle(wizard.pos.x - cam.pos.x +6 , wizard.pos.y - cam.pos.y +5 ,4,0);
        sprites.drawSelfMasked(wizard.pos.x - cam.pos.x, wizard.pos.y - cam.pos.y, WizardSprite, 0);
      }
      else{
        sprites.drawSelfMasked(wizard.pos.x - cam.pos.x - 5 + direction, wizard.pos.y - cam.pos.y, WizardFiringSprite, direction);
      }
      /*if (0==wizard.state&0x07)
        arduboy.fillCircle(wizard.pos.x - cam.pos.x  , wizard.pos.y - cam.pos.y ,5,1);
      if (0==wizard.state&0x38)
        arduboy.fillCircle(wizard.pos.x - cam.pos.x +2  , wizard.pos.y + 10 - cam.pos.y ,5,1);*/
    }
    else{
      wizard.HP--;
      if (arduboy.everyXFrames(2)) {
        arduboy.fillCircle(wizard.pos.x - cam.pos.x +8 , wizard.pos.y - cam.pos.y +4 ,4,0);
        sprites.drawSelfMasked(wizard.pos.x - cam.pos.x, wizard.pos.y - cam.pos.y, WizardSprite, 1);
      }
      if (wizard.HP<-70){
        wizard.active=false;
      }
    }
  }
  
  // ------------------- Faceless -------------------------
  
  if (faceless.active)
  {    
    ennemiesLeft++;
    //HighRect ennemiRect = {.x = faceless.pos.x, .y = faceless.pos.y, .width = 100, .height = 70};
    //HighRect kidPoint = {.x=kid.pos.x, .y=kid.pos.y, .width=100,.height=70}; 
    if (/*collide(kidPoint,ennemiRect) && */ faceless.HP > 0){             
      if (--faceless.timer==0){
        faceless.timer=80; //default wait
        //uint8_t formerPos = (faceless.state&0x07);
        uint8_t FaceRand = random(60); 
        switch ((faceless.state&0x70)>>4){
          case 2:
            faceless.timer=40;
          break;
          case 1:    //fire
            faceless.timer=20;
            /*  No more fire :( too easy if you ask me
            for (uint8_t j=0; j<MAX_PER_TYPE; j++){    // we could skip the for, only one bullet in this case... 
              if (!ennemiBullets[j].isActive){
                ennemiBullets[j].isActive=true;
                ennemiBullets[j].radius = firePower;
                ennemiBullets[j].pos.y=faceless.pos.y+4;
                ennemiBullets[j].actualpos.x=(faceless.pos.x+5*faceless.direction)<<5;
                ennemiBullets[j].speed.x= (faceless.direction? -1:1)*WEAPON_SPEED;
                //ennemiBullets[j].speed.y=0;
                ennemiBullets[j].timer=SHOT_TIMER;
                break;
              }
            }*/
            //end of case 2 -> transform back in a Faceless
          break;
          case 3: case 4:
            faceless.state &= 0xF3;  // mask "moving direction"
            faceless.timer=98;            
//            uint8_t FaceRand = random(60);
            if ((FaceRand<20)&&((faceless.state&0x03)>0)){ //up
              //faceless.state &= moving direction already 0
              faceless.state --;     // changing lane  
            }
            else if ((FaceRand>40)&&((faceless.state&0x03)<2)) { //down
              faceless.state|=0x08; // heading down
              faceless.state++;  // changing lane
            }
            else {
              faceless.state|=0x04; //middle
            }
            
          break;
          case 5:
            faceless.direction=!faceless.direction;
          break;
          case 6:
            faceless.timer=20;
          break;
          case 7:
            faceless.timer=100;
            faceless.state &= ~(0x70); // "state" = 0  -> transform into a Gargoyle    
          break;
        }
        faceless.state+=0x10;
      }
      else { // faceless.timer is running
        if ((0x50==(faceless.state&0x70)||(0x40==(faceless.state&0x70)))){
          faceless.pos.x += faceless.direction? -1:1;
          if (0==globalCounter%2)
            faceless.pos.y -= (1-((faceless.state&0x0C)>>2));
        }
      }
    }
    if (faceless.HP>0){
      if (true){
        uint8_t state_temp = ((faceless.state&0x70)>>4);
        /*if (2==state_temp){
          sprites.drawSelfMasked(faceless.pos.x - cam.pos.x, faceless.pos.y - cam.pos.y, FiringSprite,  3*faceless.direction);
        }*/
        if (state_temp<3){
          if(!((2==state_temp)&&(0==globalCounter%2)))
            sprites.drawSelfMasked(faceless.pos.x - cam.pos.x, faceless.pos.y - cam.pos.y, kidSprite , 7 * faceless.direction);
        }
        else {
          //arduboy.fillCircle(faceless.pos.x - cam.pos.x +6 , faceless.pos.y - cam.pos.y +5 ,4,0);
          if (!((7==state_temp)&&(0==globalCounter%2)))
            sprites.drawSelfMasked(faceless.pos.x - cam.pos.x, faceless.pos.y - cam.pos.y, FacelessSprite, 0);
        }
      }
      else{
//        sprites.drawSelfMasked(faceless.pos.x - cam.pos.x - 5 + direction, faceless.pos.y - cam.pos.y, WizardFiringSprite, direction);
      }
      /*if (0==wizard.state&0x07)
        arduboy.fillCircle(wizard.pos.x - cam.pos.x  , wizard.pos.y - cam.pos.y ,5,1);
      if (0==wizard.state&0x38)
        arduboy.fillCircle(wizard.pos.x - cam.pos.x +2  , wizard.pos.y + 10 - cam.pos.y ,5,1);*/
    }
    else{
      faceless.HP--;
      if (arduboy.everyXFrames(2)) {
        //arduboy.fillCircle(faceless.pos.x - cam.pos.x +8 , faceless.pos.y - cam.pos.y +4 ,4,0);
        sprites.drawSelfMasked(faceless.pos.x - cam.pos.x, faceless.pos.y - cam.pos.y, FacelessSprite, 1);
      }
      if (faceless.HP<-70){
        faceless.active=false;
      }
    }
  }

// End of 
  
  if (bossRoom&&(0==ennemiesLeft)){
    //LvlUp
    gameState=STATE_GAME_LVLUP;
  }
}


#endif
