#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 550;
const int BOMB_WIDTH = 40;
const int BOMB_HEIGHT = 40;
const int BOMB_START_X = 970;
const int BOMB_START_Y1 = 346;
const int BOMB_START_Y2 = 157;
const int CHARACTER_X = 350;
const int CHARACTER_Y1 = 327;
const int CHARACTER_WIDTH = 60;
const int CHARACTER_HEIGHT = 60;
extern float CHARACTER_SPEED;
extern float BOMB_SPEED;
const int LOSE_X = 300;
const int LOSE_Y = 152;
const int LOSE_WIDTH = 400;
const int LOSE_HEIGHT = 240;
const int SOUND_X = 23;
const int SOUND_Y = 62;
const int SOUND_WIDTH = 35;
const int SOUND_HEIGHT = 35;
const int ROCKET_HEIGHT = 30;
const int ROCKET_WIDTH = 60;
const int ROCKET_START_X = 950;
extern int ROCKET_START_Y;
extern float ROCKET_SPEED;
const int HOME_START_X = 7;
const int HOME_START_Y = 5;
const int HOME_WIDTH = 65;
const int HOME_HEIGHT = 65;

extern float delay;
extern int backgroundX;
extern float BACKGROUND_SPEED;
extern int rkt;
extern int col;

#endif
