#ifndef WEAPON_H
#define WEAPON_H

#include <Arduino.h>
#include "globals.h"
//#include "levels.h"
#include "vec2.h"
/*
#define FIXED_POINT 5
#define PLAYER_SPEED_WALKING 1 << FIXED_POINT
#define PLAYER_SPEED_AIR 2
#define PLAYER_PARTICLES 3
#define PLAYER_JUMP_VELOCITY (1 << FIXED_POINT) + 8
#define GRAVITY 3
#define FRICTION 1 // for horizontal speed
#define MAX_XSPEED PLAYER_SPEED_WALKING
#define MAX_XSPEED_FAN 54
#define MAX_YSPEED 3 * (1 << FIXED_POINT)
#define CAMERA_OFFSET 16
*/
#define MAX_WEAPON 2

extern bool gridGetSolid(int8_t x, int8_t y);
//extern void kidHurt();
//extern void windNoise();
/*
struct Camera
{
  // x and y are 9.6 signed fixed vec2 values
  vec2 pos;
  vec2 offset;
};*/

struct Weapon
{
  public:
    // x and y are 9.6 signed fixed vec2 values
    vec2 pos;
    vec2 actualpos;
    vec2 speed;
    boolean isActive;
    boolean direction;
    byte timer; //need?
    byte type;

    //byte balloonOffset;
    vec2 particles[PLAYER_PARTICLES];

    void draw(vec2 cam)
    {
      if (isActive)
      {
        arduboy.drawCircle(pos.x-cam.x,pos.y-cam.y,2);
        /*vec2 kidcam;
        kidcam.x = kid.pos.x - cam.pos.x;
        kidcam.y = kid.pos.y - cam.pos.y;
        // Fall off earth
       
        if (!kid.isSucking)
        {
          //sprites.drawSelfMasked(kidcam.x, kidcam.y, kidSprite, 12 + kid.direction);
          //sprites.drawErase(kidcam.x, kidcam.y, kidSprite, kid.frame + 6 * kid.direction + ((kid.isJumping << 2) + 5 * (kid.isLanding || kid.isFlying)) * !kid.isSucking);
          sprites.drawOverwrite(kidcam.x, kidcam.y, kidSprite, kid.frame + 7 * kid.direction + ((kid.isJumping << 2) + 5 * (kid.isLanding || kid.isFlying)) * !kid.isSucking + kid.isClimbing*6 );
        }*/
    
      }
    }

    
};

//Players kid;
//Camera cam;


void checkKid()
{
  if (kid.isImune)
  {
    if (arduboy.everyXFrames(2)) kid.isActive = !kid.isActive;
    kid.imuneTimer++;
    if (kid.imuneTimer > 60)
    {
      kid.imuneTimer = 0;
      kid.isImune = false;
      kid.isActive = true;
    }
  }

  if (kid.isWalking || kid.isSucking)
  {
    if (arduboy.everyXFrames(8))
    {
      kid.frame = (++kid.frame) % 4;
      //if (kid.frame % 2 == 0)
        //sound.tone(150, 20);
    }
  }
  else
    kid.frame = 0;

  // Kid is moving up
  if (!kid.isFlying && kid.speed.y > 0)
  {
    kid.isJumping = true;
    kid.isLanding = false;
    if (!kid.jumpLetGo && kid.jumpTimer > 0)
    {
      kid.speed.y += GRAVITY + 2;
      kid.jumpTimer--;
    }
  }
  else if (kid.speed.y < 0)
  {
    // Kid is moving down
    kid.isJumping = false;
    kid.isLanding = true;    
  }

  // Update position---
  // -Solid checking
  int tx = (kid.pos.x + 6) >> 4;
  int ty = (kid.pos.y + 8) >> 4;
  boolean solidbelow = gridGetSolid(tx, (kid.pos.y + 16) >> 4);
  //boolean solidabove = gridGetSolid(tx, (kid.pos.y - 1) >> 4);
  //boolean solidleft = gridGetSolid((kid.pos.x - 1) >> 4, ty);
  //boolean solidright = gridGetSolid((kid.pos.x + 13) >> 4, ty);
  int tx2 = (((kid.actualpos.x + kid.speed.x) >> FIXED_POINT) - 1 + (kid.speed.x > 0) * 14) >> 4;
  boolean solidH = gridGetSolid(tx2, (kid.pos.y + 2) >> 4)
                   || gridGetSolid(tx2, (kid.pos.y + 13) >> 4);
  int ty2 = (((kid.actualpos.y - kid.speed.y) >> FIXED_POINT) + (kid.speed.y < 0) * 17) >> 4;
  boolean solidV = gridGetSolid((kid.pos.x + 2) >> 4, ty2)
                   || gridGetSolid((kid.pos.x + 10) >> 4, ty2);

  // Gravity
  if (kid.hearts == 0 || kid.speed.y > 0 || !solidbelow)
  {
    //kid.speed.y += GRAVITY;
    if (!kid.isClimbing) {
      kid.speed.y = (kid.speed.y > -MAX_YSPEED) ? kid.speed.y - GRAVITY : -MAX_YSPEED;
      if (kid.isFlying)
      {
        kid.speed.y=0;
        if (arduboy.everyXFrames(2)) //must depend on wings lvl
          kid.wingsJauge -=1 ; 
        if (kid.wingsJauge<=0)
          kid.isFlying=false;
        /*
        //cam.offset.y += 3;
        if (kid.balloonOffset > 0)
          kid.balloonOffset -= 2;
        else
        {
          //kid.speed.y = max(-((8 / kid.hearts) >> 1), kid.speed.y);
          kid.speed.y = max(kid.hearts - 5, kid.speed.y);
        }*/
      }
    }
  }

  // Kid on ground
  if (kid.hearts > 0 && kid.speed.y <= 0 && (solidV || solidbelow))
  {
    //if (kid.isLanding) sound.tone(80, 30);
    kid.wingsJauge = 60;
    kid.speed.y = 0;
    kid.speed.x = 0;
    kid.isLanding = false;
    kid.isJumping = false;
    kid.isFlying = false;
    int8_t ysnap = (((kid.actualpos.y >> FIXED_POINT) + 12) >> 4);
    //if (!gridGetSolid(ysnap, kid.pos.x >> 4))
    kid.actualpos.y = ysnap << (FIXED_POINT + 4);

    // Fall off edge
    /*
    //if (abs(((kid.pos.x + 6) % 16) - 8) >= 4)
    //if (!arduboy.pressed(RIGHT_BUTTON) && !arduboy.pressed(LEFT_BUTTON))
    if (!arduboy.pressed(RIGHT_BUTTON | LEFT_BUTTON))
    {
      int yy = (kid.pos.y + 16) >> 4;
      bool sl = gridGetSolid((kid.pos.x + 4) >> 4, yy);
      bool sr = gridGetSolid((kid.pos.x + 8) >> 4, yy);
      if (!sl & gridGetSolid((kid.pos.x + 11) >> 4, yy))
        kid.actualpos.x -= FIXED_POINT << 2;
      else if (!sr && gridGetSolid((kid.pos.x) >> 4, yy))
        kid.actualpos.x += FIXED_POINT << 2;
    }*/
  }
  else
  {
    // Friction in air
    if (abs(kid.speed.x) > FRICTION)
    {
      if (arduboy.everyXFrames(4))
      {
        if (kid.speed.x > 0) kid.speed.x -= FRICTION;
        else if (kid.speed.x < 0) kid.speed.x += FRICTION;
      }
    }
    else
    {
      kid.speed.x = 0;
    }
  }

  // Move out of walls
  if (!gridGetSolid(tx, ty))
  {
    if (gridGetSolid((kid.pos.x) >> 4, ty))
      kid.actualpos.x += 8;
    else if (gridGetSolid((kid.pos.x + 11) >> 4, ty))
      kid.actualpos.x -= 8;
  }

  if (kid.hearts == 0 || (!solidV && kid.speed.y != 0))
  {
    kid.actualpos.y -= kid.speed.y;
  }
  else
  {
    if (solidV && kid.speed.y > 0)
    {
      kid.actualpos.y = ((kid.pos.y + 8) >> 4) << (FIXED_POINT + 4);
      kid.speed.y = 0;
      //sound.tone(80, 30);
    }
  }

  // -X Position
  //if ((kid.speed.x < 0 && !solidH) || (kid.speed.x > 0 && !solidH))
  if (kid.speed.x != 0)
  {
    /*boolean solidH = gridGetSolid(tx2, (kid.pos.y + 2) >> 4)
                   || gridGetSolid(tx2, (kid.pos.y + 13) >> 4);*/
    if (!solidH)
    {
      kid.actualpos.x += kid.speed.x;
    }
    else
    {
      kid.speed.x = 0;
      
      
    }
  }

  kid.pos = (kid.actualpos >> FIXED_POINT);

  if (kid.isSucking) windNoise();//sound.tone(300 + random(10), 20);
}


#endif
