#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Constants.h"

void check_movement(int &flip, int &targetY, bool &moveDown, bool &moveUp)
{
    flip++;
    if (flip % 2 != 0)
    {
        targetY = 157;
        moveUp = true;
        moveDown = false;
    }
    else
    {
        targetY = 327;
        moveUp = false;
        moveDown = true;
    }
}

void game_sound(int sound_check, SDL_Texture* &sound, SDL_Texture* &sound_on_texture, SDL_Texture* &sound_off_texture, SDL_Renderer* renderer, SDL_Rect soundd)
{
    if(sound_check % 2 == 0)
    {
        sound = sound_on_texture;
        SDL_RenderCopy(renderer, sound, NULL, &soundd);
        Mix_ResumeMusic();
    }
    else
    {
        sound = sound_off_texture;
        SDL_RenderCopy(renderer, sound, NULL, &soundd);
        Mix_PauseMusic();
    }
}

void game_property(int point, int &ms, float &BOMB_SPEED, int &mod, float &BACKGROUND_SPEED, float& delay, float &ROCKET_SPEED)
{
    if(point <= 3)
    {
        BOMB_SPEED = 9;
        ms = 2000;
        mod = 8000;
        CHARACTER_SPEED = 5;
        BACKGROUND_SPEED = 3;
        delay = 16;
        ROCKET_SPEED = 10;
    }
    else if(point <= 6)
    {
        BOMB_SPEED = 9.5;
        ms = 1800;
        mod = 7000;
        CHARACTER_SPEED = 5.2;
        BACKGROUND_SPEED = 3.4;
        delay = 15;
        ROCKET_SPEED = 10.5;
    }
    else if(point <= 9)
    {
        BOMB_SPEED = 10;
        ms = 1600;
        mod = 6500;
        CHARACTER_SPEED = 5.4;
        BACKGROUND_SPEED = 3.8;
        delay = 14;
        ROCKET_SPEED = 11;
    }
    else if(point <= 12)
    {
        BOMB_SPEED = 10.5;
        ms = 1400;
        mod = 6000;
        CHARACTER_SPEED = 5.6;
        BACKGROUND_SPEED = 4.2;
        delay = 13.5;
        ROCKET_SPEED = 11.5;
    }
    else if(point <= 15)
    {
        BOMB_SPEED = 11;
        ms = 1200;
        mod = 5500;
        CHARACTER_SPEED = 5.8;
        BACKGROUND_SPEED = 4.6;
        delay = 13;
        ROCKET_SPEED = 12;
    }
    else if(point <= 18)
    {
        BOMB_SPEED = 11.5;
        ms = 1000;
        mod = 5000;
        CHARACTER_SPEED = 6;
        BACKGROUND_SPEED = 5;
        delay = 12.5;
        ROCKET_SPEED = 12.5;
    }
    else
    {
        BOMB_SPEED = 12;
        ms = 700;
        mod = 4500;
        CHARACTER_SPEED = 6.3;
        BACKGROUND_SPEED = 5.4;
        delay = 12;
        ROCKET_SPEED = 13;
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    if(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
    {
        return false;
    }
    return true;
}

bool Valid_Rocket(SDL_Rect a, SDL_Rect b)
{
    if((a.x * 4 <= SCREEN_WIDTH) || (b.x * 4 <= SCREEN_WIDTH))
        return true;
    return false;
}

#endif // GAME_LOGIC_H
