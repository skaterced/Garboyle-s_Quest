#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "globals.h"
#include "player.h"

#define TIMER_AMOUNT 48

void checkInputs()
{
  if (kid.hearts <= 0)
    return; // Cannot control player if dead

    
  cam.offset = vec2(0, 0);
  kid.isWalking = false;
  if (arduboy.pressed(DOWN_BUTTON))
  {
    if (kid.isClimbing){
      kid.jumpLetGo=true;
    }
    kid.isClimbing=false;
    cam.offset.y = -CAMERA_OFFSET;
  }
  else if (arduboy.pressed(UP_BUTTON))
  {
    cam.offset.y = CAMERA_OFFSET;
  }
  if (!(kid.isFiring||kid.isClimbing)) 
  {
    if (arduboy.pressed(LEFT_BUTTON))
    {
      mapTimer = TIMER_AMOUNT;
      cam.offset.x = CAMERA_OFFSET;
      kid.direction = FACING_LEFT;
      if (!(kid.isJumping || kid.isFlying || kid.isLanding))
      {
        if (!gridGetSolid((kid.pos.x - 1) >> 4, (kid.pos.y + 8) >> 4))
          kid.actualpos.x -= PLAYER_SPEED_WALKING;
        kid.isWalking = true;
        kid.speed.x = -1;
      }
      else
      {
        //kid.speed.x = max(kid.speed.x - PLAYER_SPEED_AIR, -MAX_XSPEED);
        if (kid.speed.x > -MAX_XSPEED)
          kid.speed.x -= PLAYER_SPEED_AIR;
      }
    }
    else if (arduboy.pressed(RIGHT_BUTTON))
    {
      //mapTimer = TIMER_AMOUNT;
      cam.offset.x = -CAMERA_OFFSET;
      kid.direction = FACING_RIGHT;
      if (!(kid.isJumping || kid.isFlying || kid.isLanding))
      {
        if (!gridGetSolid((kid.pos.x + 12) >> 4, (kid.pos.y + 8) >> 4))
          kid.actualpos.x += PLAYER_SPEED_WALKING;
        kid.isWalking = true;
        kid.speed.x = 1;
      }
      else
      {
        //kid.speed.x = min(kid.speed.x + PLAYER_SPEED_AIR, MAX_XSPEED);
        if (kid.speed.x < MAX_XSPEED)
          kid.speed.x += PLAYER_SPEED_AIR;
      }
    }
  }
  kid.isFiring = false;
  if (arduboy.pressed(A_BUTTON))
  {
    if (arduboy.pressed(DOWN_BUTTON))
      gameState = STATE_GAME_PAUSE;
    else //if (!kid.isFlying)
    {
      //kid.isFlying = false;
      kid.fire();
      kid.isFiring = true;
    }
  }
  /*if (arduboy.pressed(A_BUTTON + DOWN_BUTTON))  gameState = STATE_GAME_PAUSE;
  if (arduboy.pressed(A_BUTTON) && !kid.isFlying)
  {
    kid.isFiring = true;
  }
  else
    kid.isFiring = false;*/

  // Jump Button
  if (arduboy.justPressed(B_BUTTON)) //todo: check if climbing -> jump against the wall
  {
    if ((kid.speed.y == 0 && kid.isJumping == false && kid.isLanding == false)||kid.isClimbing)
    {
      //sound.tone(200, 100);
      kid.isWalking = false;
      kid.isJumping = true;
      kid.jumpLetGo = false;
      kid.isClimbing = false;
      kid.jumpTimer = PLAYER_JUMP_TIME;
      kid.speed.y = jumpVelocity;
      if (arduboy.pressed(RIGHT_BUTTON)) kid.speed.x = MAX_XSPEED;
      else if (arduboy.pressed(LEFT_BUTTON)) kid.speed.x = -MAX_XSPEED;
    }
    else if (!kid.isFlying)
    {
      if (kid.wingsJauge > 0)
      {
        kid.isFlying = true;
        //kid.balloonOffset = 16;
        kid.isJumping = false;
        kid.isLanding = true;
      }
    }
    else{
      kid.isFlying=false;
    }
  }
  if (!arduboy.pressed(B_BUTTON))
  {
    //kid.isFlying = false;
    if (kid.isJumping) kid.jumpLetGo = true;
  }
}

#endif
