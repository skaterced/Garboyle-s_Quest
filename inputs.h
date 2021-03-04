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

    
  //cam.offset = vec2(0, 0);
  kid.isWalking = false;
  if (arduboy.pressed(DOWN_BUTTON))
  {
    cam.offset.y = -CAMERA_OFFSET;
  }
  else if (arduboy.pressed(UP_BUTTON))
  {
    cam.offset.y = CAMERA_OFFSET;
  }
  if (!kid.isClimbing) 
  {
    if (arduboy.pressed(LEFT_BUTTON)){
    //if (!(kid.againstWall&&(FACING_LEFT == kid.direction))){      
      mapTimer = TIMER_AMOUNT;
      /*cam.offset.x+=2;
      if (cam.offset.x > CAMERA_OFFSET)
        cam.offset.x = CAMERA_OFFSET;*/
      kid.direction = FACING_LEFT;
      if (!(kid.isFiring)){
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
    }
    else if (arduboy.pressed(RIGHT_BUTTON)){
      //if (!(kid.againstWall&&(FACING_RIGHT == kid.direction)))
      {
        //mapTimer = TIMER_AMOUNT;
        /*cam.offset.x-=2;
        if (cam.offset.x < -CAMERA_OFFSET)
          cam.offset.x = -CAMERA_OFFSET;*/
        kid.direction = FACING_RIGHT;
        if (!(kid.isFiring)){
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
    }
    //else
      //kid.againstWall = false;
  }
  //else 
    //cam.offset.x = (kid.direction ? CAMERA_OFFSET : -CAMERA_OFFSET);
 
  if (arduboy.justPressed(A_BUTTON))
  {
    //kid.isFlying = false;
    kid.fire();
    //kid.isFiring = true;
  }
  if (arduboy.pressed(A_BUTTON)&&(kid.shootingTimer>(SHOOT_TIMER_INIT-10)))
  {
    kid.isFiring = true;
    //kid.fire();
  }
  else
    kid.isFiring = false;
    
  if (arduboy.pressed(UP_BUTTON)&&arduboy.pressed(DOWN_BUTTON)){
    globalCounter=65;  
    gameState = STATE_GAME_PAUSE;
  }
  
  // Jump Button
  if (arduboy.justPressed(B_BUTTON))
  {
    bool letsJump = false;
    if (((kid.speed.y == 0 || kid.speed.y == -GRAVITY) && kid.isJumping == false && kid.isLanding == false)||kid.isClimbing)
    {
      //sound.tone(200, 100);
      kid.isWalking = false;      
      kid.jumpLetGo = false;

      if (kid.isClimbing){
        if (arduboy.pressed(DOWN_BUTTON)){
          kid.isClimbing=false;
          //kid.direction=!kid.direction; // not facing the wall when dropping it 
        }
        else if ((arduboy.pressed(RIGHT_BUTTON)&&(FACING_LEFT!=kid.direction)) // no jumping against the wall
              ||(arduboy.pressed(LEFT_BUTTON)&&(FACING_RIGHT!=kid.direction))){          
          //kid.direction=!kid.direction; // not facing the wall when dropping it 
          letsJump=true;          
          kid.speed.x = kid.direction? -MAX_XSPEED:MAX_XSPEED;
        }
        else if (!(arduboy.pressed(RIGHT_BUTTON)||arduboy.pressed(LEFT_BUTTON)))
          letsJump=true;        
      }
      else { //if (!kid.isClimbing){
        letsJump=true;
        if (arduboy.pressed(RIGHT_BUTTON)||arduboy.pressed(LEFT_BUTTON)){
          kid.speed.x = kid.direction? -MAX_XSPEED:MAX_XSPEED;
        }
      }
      if (letsJump&&kid.pos.y>-80){ // Y limitation for outdor lvl
        kid.isClimbing = false;
        kid.isJumping = true;
        kid.jumpTimer = PLAYER_JUMP_TIME - 4 +(jumpVelocity/10);
        kid.speed.y = jumpVelocity;
      }
    }
      
/*      if (arduboy.pressed(DOWN_BUTTON)&&(kid.isClimbing)){
        kid.isClimbing=false;
        kid.direction=!kid.direction; // not facing the wall when dropping it 
      }
      else {
        kid.jumpTimer = PLAYER_JUMP_TIME - 4 +(jumpVelocity/10);
        kid.speed.y = jumpVelocity;
      
      if (arduboy.pressed(RIGHT_BUTTON)){
        if (!(kid.isClimbing&&(FACING_RIGHT==kid.direction))){
          kid.speed.x = MAX_XSPEED;
        }
        //else
          //kid.againstWall=true;
      }
      else if (arduboy.pressed(LEFT_BUTTON)){
        if (!(kid.isClimbing&&(FACING_LEFT==kid.direction))){
          kid.speed.x = -MAX_XSPEED;
        }
       // else
          //kid.againstWall=true;
      }
    }
      else if (kid.isClimbing){
        kid.direction=!kid.direction; // not facing the wall when jumping out of it        
      }
      kid.isClimbing = false;
    }
*/
    
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
    if (kid.isJumping)
      kid.jumpLetGo = true;
  }
}

#endif
