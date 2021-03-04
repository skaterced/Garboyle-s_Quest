#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
//#include <ArduboyTones.h>
#include "vec2.h"
#include "bitmaps.h"

#define CHEAT_ON //cost 100 progmem bytes

// EEPROM - change this address offset from the arduboy starting address if desired
#define OFFSET_GQ_START              (EEPROM_STORAGE_SPACE_START + 336)
#define OFFSET_DIFFICULTY            (OFFSET_GQ_START + sizeof(byte))
#define OFFSET_LEVEL                 (OFFSET_DIFFICULTY + sizeof(bool))
#define OFFSET_DEATH                 (OFFSET_LEVEL + sizeof(uint16_t))

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_DIFF              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_GAMEOVER          5

//define game states (on main menu)
#define STATE_GAME_NEXT_LEVEL        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_PAUSE             8
#define STATE_GAME_LVLUP             9
#define STATE_GAME_OVER              10
#define STATE_GAME_PLAYLOAD          11
#define STATE_GAME_PLAYNEW           12

#define TILE_BG                      11

#define FACING_RIGHT                 0
#define FACING_LEFT                  1

#define LEVEL_TO_START_WITH          1
#define TOTAL_LEVELS                 39
#define TOTAL_COINS                  TOTAL_LEVELS * 6

#define MAX_PER_TYPE                 6                    // total instances per enemy type

#define LEVEL_WIDTH                  384                  // 24 * 16
#define LEVEL_HEIGHT                 384                  // 24 * 16
#define LEVEL_CELLSIZE               16
#define LEVEL_WIDTH_CELLS            level_width_cell //24
#define LEVEL_HEIGHT_CELLS           level_height_cell //24
#define LEVEL_CELL_BYTES             (LEVEL_WIDTH_CELLS * LEVEL_HEIGHT_CELLS) >> 3
#define LEVEL_ARRAY_SIZE             level_width_cell*level_height_cell//576

#define MAX_DOORS                    7
//lvl ups
#define UPGRADE_JUMP   0x01
#define UPGRADE_FIRE   0x02
#define UPGRADE_WINGS  0x04
#define UPGRADE_ARMOR  0x08

#define IMUNE_TIME 80
#define PLAYER_JUMP_TIME             11
//#define PLAYER_JUMP_VELOCITY         30

Arduboy2Base arduboy;
Sprites sprites;
//ArduboyTones sound(arduboy.audio.enabled);

// This is a replacement for struct Rect in the Arduboy2 library.
// It defines height as an int instead of a uint8_t to allow a higher rectangle.
class HighRect
{
  public:
    int x;
    int y;
    uint16_t width;
    uint16_t height;
    void draw(int offX, int offY){
      arduboy.fillRect(x-offX,y-offY,width,height);
    }
};

class Door
{
  public:
    vec2 pos;
    uint8_t destination;
    Door(){
      pos.x=99;
      pos.y=99;
      destination=99;
    }
};

uint8_t level_width_cell = 24;
uint8_t level_height_cell = 24;

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
//byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
uint8_t globalCounter = 0;
byte level;
bool difficulty = 0; // 0 easy, 1 hard
bool finalBossBeaten = false;

Door levelExits [MAX_DOORS];
uint8_t wichEntrance = 0;
vec2 startPos = vec2(12,12);
byte mapTimer = 10; // need?
uint8_t lvlSettings=0;
bool indorLevel=false;
//uint8_t bossesAlive = 0xFF;
bool bossRoom =false;

uint8_t wingLvl=1;
uint8_t jumpVelocity=30;
uint8_t firePower =5;
uint8_t heartsMax = 3; // (armor)

uint8_t deathToll = 0;

void loadSetEEPROM()
{
  if (EEPROM.read(OFFSET_GQ_START) != GAME_ID) //&& (EEPROM.read(OFFSET_GQ_END) != GAME_ID))
  {
    EEPROM.put(OFFSET_GQ_START, (byte)GAME_ID); // game id
    EEPROM.put(OFFSET_DIFFICULTY, difficulty);
    EEPROM.put(OFFSET_LEVEL, (uint16_t)0);
    EEPROM.put(OFFSET_DEATH, (uint8_t)0);
    /*
    EEPROM.put(OFFSET_COINS, (byte)0); // coins current run
    EEPROM.put(OFFSET_COINSHS, (byte)0); // coins highscore run
    EEPROM.put(OFFSET_SCORE, (unsigned long)0); // clear score
    EEPROM.put(OFFSET_HSCORE, (unsigned long)0); // clear high score
    EEPROM.put(OFFSET_GQ_END, (byte)GAME_ID); // game id*/
  }
}

// This is a replacement for the collide() function in the Arduboy2 library.
// It uses struct HighRect instead of the struct Rect in the library.
bool collide(HighRect rect1, HighRect rect2)
{
  return !( rect2.x                        >=  (int)(rect1.x + rect1.width)   ||
            rect2.x + (int)(rect2.width)   <=  rect1.x                        ||
            rect2.y                        >=  (int)(rect1.y + rect1.height)  ||
            rect2.y + (int)(rect2.height)  <=  rect1.y);
}

#endif
