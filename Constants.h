#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 550;
const int BOMB_WIDTH = 40;
const int BOMB_HEIGHT = 40;
const int BOMB_START_X = 970;
const int BOMB_START_Y1 = 346;
const int BOMB_START_Y2 = 157;
extern float BOMB_SPEED;
const int CHARACTER_Y1 = 327;
const int CHARACTER_X = 350;
const int CHARACTER_WIDTH = 60;
const int CHARACTER_HEIGHT = 60;
extern float CHARACTER_SPEED;
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

bool showBomb = false;
bool showBombflip = false;
bool showRocket = false;
bool running = true;
bool inGame = false;
bool gameOver = false;
bool lose_menu = false;
bool instruction = false;
bool bomb_Visible = false;
bool bomb_flip_Visible = false;
bool rocket_Visible = false;
bool moveUp = false;
bool moveDown = false;
bool isPaused = false;
bool scrolling_bg = true;

float CHARACTER_SPEED = 5;
float BOMB_SPEED = 9;
float BACKGROUND_SPEED = 3;
float ROCKET_SPEED = 10;
float delay = 16;

int backgroundX = 0;
int ROCKET_START_Y = 0;
int sound_check = 0;
int flip = 0;
int point = 0;
int rocket_time;
int bomb_time;
int rkt = -1;
int col = -1;

Uint32 nextBombTime = SDL_GetTicks() + (rand() % 7000) + 2000;
Uint32 nextBombflipTime = SDL_GetTicks() + (rand() % 7000) + 2000;
Uint32 nextRocketTime = SDL_GetTicks() + (rand() % 10000) + 5000;

#endif
