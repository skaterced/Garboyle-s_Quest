#ifndef WEAPON_H
#define WEAPON_H

#include <Arduino.h>
#include "globals.h"
//#include "levels.h"
#include "vec2.h"

#define MAX_WEAPON 2
#define WEAPON_SPEED 40

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
    boolean direction; //need?
    byte timer; //need?
    byte type;

    //byte balloonOffset;
//    vec2 particles[PLAYER_PARTICLES];

    void draw(vec2 cam)
    {
      if (isActive)
      {
        arduboy.drawCircle(pos.x-cam.x,pos.y-cam.y,2);
      }
    }
    
    void check(){
      if (isActive){
        int tx2 = (((actualpos.x + speed.x) >> 5/*FIXED_POINT*/) - 2 + (speed.x > 0) * 4) >> 4;
        boolean solidH = gridGetSolid(tx2, (pos.y + 1) >> 4) || gridGetSolid(tx2, (pos.y + 3) >> 4);      
         // -X Position
        if (!solidH)
        {
          actualpos.x += speed.x;
        }
        else
        {
          isActive=false;
        }
        pos.x = (actualpos.x >> 5);   
      }      
    }
};

//Players kid;
//Camera cam;

/*
void checkKid()
{

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
*/

#endif
