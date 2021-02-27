#ifndef WEAPON_H
#define WEAPON_H

#include <Arduino.h>
#include "globals.h"
//#include "levels.h"
#include "vec2.h"

#define MAX_WEAPON 2
#define WEAPON_SPEED 60
#define SHOT_TIMER 100

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
    byte timer;
    uint8_t radius;

    //byte balloonOffset;
//    vec2 particles[PLAYER_PARTICLES];

    void draw(vec2 cam)
    {
      if (isActive)
      {
        for (uint8_t i=radius+2; i>1; i--){
          arduboy.drawCircle(pos.x-cam.x,pos.y-cam.y,i,(((2+radius)==i)||((globalCounter&0x02)>>1)));
        }
      }
    }
    
    void check(){
      if (isActive){
        if (--timer==0){
          isActive=false;
        }
        else {
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
    }
};



#endif
