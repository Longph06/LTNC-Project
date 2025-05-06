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

void game_sound(int sound_check, SDL_Texture* &sound, SDL_Texture* &sound_on_texture, SDL_Texture* &sound_off_texture, SDL_Renderer* renderer, SDL_Rect soundd, int col, int rkt)
{
    if(sound_check % 2 == 0)
    {
        sound = sound_on_texture;
        SDL_RenderCopy(renderer, sound, NULL, &soundd);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
        Mix_Volume(-1, 100);
    }
    else
    {
        sound = sound_off_texture;
        SDL_RenderCopy(renderer, sound, NULL, &soundd);
        Mix_VolumeMusic(0);
        Mix_Volume(-1, 0);
    }
}

void game_property(int point, int &bomb_time, float &BOMB_SPEED, int &rocket_time, float &BACKGROUND_SPEED, float& delay, float &ROCKET_SPEED)
{
    if(point <= 2)
    {
        BOMB_SPEED = 9;
        bomb_time = 2000;
        rocket_time = 8000;
        CHARACTER_SPEED = 5;
        BACKGROUND_SPEED = 3;
        delay = 16;
        ROCKET_SPEED = 10;
    }
    else if(point <= 4)
    {
        BOMB_SPEED = 9.5;
        bomb_time = 1800;
        rocket_time = 7000;
        CHARACTER_SPEED = 5.2;
        BACKGROUND_SPEED = 3.4;
        delay = 15;
        ROCKET_SPEED = 10.5;
    }
    else if(point <= 6)
    {
        BOMB_SPEED = 10;
        bomb_time = 1600;
        rocket_time = 6500;
        CHARACTER_SPEED = 5.4;
        BACKGROUND_SPEED = 3.8;
        delay = 14;
        ROCKET_SPEED = 11;
    }
    else if(point <= 8)
    {
        BOMB_SPEED = 10.5;
        bomb_time = 1400;
        rocket_time = 6000;
        CHARACTER_SPEED = 5.6;
        BACKGROUND_SPEED = 4.2;
        delay = 13.5;
        ROCKET_SPEED = 11.5;
    }
    else if(point <= 10)
    {
        BOMB_SPEED = 11;
        bomb_time = 1200;
        rocket_time = 5500;
        CHARACTER_SPEED = 5.8;
        BACKGROUND_SPEED = 4.6;
        delay = 13;
        ROCKET_SPEED = 12;
    }
    else if(point <= 13)
    {
        BOMB_SPEED = 11.5;
        bomb_time = 1000;
        rocket_time = 5000;
        CHARACTER_SPEED = 6;
        BACKGROUND_SPEED = 5;
        delay = 12.5;
        ROCKET_SPEED = 12.5;
    }
    else if(point <= 16)
    {
        BOMB_SPEED = 12;
        bomb_time = 700;
        rocket_time = 4500;
        CHARACTER_SPEED = 6.3;
        BACKGROUND_SPEED = 5.4;
        delay = 12;
        ROCKET_SPEED = 13;
    }
    else
    {
        BOMB_SPEED = 13;
        bomb_time = 500;
        rocket_time = 4000;
        CHARACTER_SPEED = 6.7;
        BACKGROUND_SPEED = 6;
        delay = 13;
        ROCKET_SPEED = 14;
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

void game_quit(SDL_Texture* bomb_texture, SDL_Texture* bomb_flip_texture,
              SDL_Texture* sound_off_texture, SDL_Texture* sound_on_texture,
              SDL_Texture* lose_texture, SDL_Texture* character_texture,
              SDL_Texture* home_texture, SDL_Texture* sound,
              SDL_Texture* rocket_texture, SDL_Texture* background,
              SDL_Renderer* renderer, SDL_Window* window,
              Mix_Music* music, Mix_Chunk* rocket_sound, Mix_Chunk* collision)
{
    SDL_DestroyTexture(bomb_texture);
    SDL_DestroyTexture(bomb_flip_texture);
    SDL_DestroyTexture(sound_off_texture);
    SDL_DestroyTexture(sound_on_texture);
    SDL_DestroyTexture(lose_texture);
    SDL_DestroyTexture(character_texture);
    SDL_DestroyTexture(home_texture);
    SDL_DestroyTexture(sound);
    SDL_DestroyTexture(rocket_texture);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_FreeChunk(collision);
    Mix_FreeChunk(rocket_sound);
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_CloseAudio();
}

void ShowBomb(SDL_Rect &bomb, SDL_Texture* bomb_texture, SDL_Renderer* renderer,
             float BOMB_SPEED, bool &showBomb, bool &bomb_Visible, int &point)
{
    if (showBomb)
    {
        bomb.x -= BOMB_SPEED;
        SDL_RenderCopy(renderer, bomb_texture, NULL, &bomb);

        if (bomb.x + bomb.w < 0)
        {
            showBomb = false;
            bomb_Visible = false;
            point++;
        }
    }
}

void ShowBombFlip(SDL_Rect &bomb_flip, SDL_Texture* bomb_flip_texture, SDL_Renderer* renderer,
                  float BOMB_SPEED, bool &showBombflip, bool &bomb_flip_Visible, int &point)
{
    if (showBombflip)
    {
        bomb_flip.x -= BOMB_SPEED;
        SDL_RenderCopy(renderer, bomb_flip_texture, NULL, &bomb_flip);

        if (bomb_flip.x + bomb_flip.w < 0)
        {
            showBombflip = false;
            bomb_flip_Visible = false;
            point++;
        }
    }
}

void game_restart(bool &gameOver, bool &inGame, bool &lose_menu,
                 bool &showBomb, bool &showBombflip, bool &showRocket,
                 bool &bomb_Visible, bool &bomb_flip_Visible, bool &rocket_Visible,
                 Uint32 &nextBombTime, Uint32 &nextBombflipTime, Uint32 &nextRocketTime,
                 SDL_Rect &rocket, SDL_Rect &bomb, SDL_Rect &bomb_flip,
                 int &flip, int &point, SDL_Rect &character,
                 bool &moveDown, bool &moveUp, int &targetY)
{

    gameOver = false;
    inGame = true;
    lose_menu = false;
    showBomb = false;
    showBombflip = false;
    showRocket = false;
    bomb_Visible = false;
    bomb_flip_Visible = false;
    rocket_Visible = false;
    nextBombTime = SDL_GetTicks() + (rand() % 5000) + 1000;
    nextBombflipTime = SDL_GetTicks() + (rand() % 5000) + 1000;
    nextRocketTime = SDL_GetTicks() + (rand() % 10000) + 5000;
    rocket.x = ROCKET_START_X;
    bomb.x = BOMB_START_X;
    bomb_flip.x = BOMB_START_X;
    flip = 0;
    point = 0;
    character.y = CHARACTER_Y1;
    moveDown = false;
    moveUp = false;
    targetY = character.y;
}

void game_start(int &flip, SDL_Rect &character, int &point, bool &inGame, bool &lose_menu,
             bool &moveDown, bool &moveUp, bool &showBomb, bool &showBombflip,
             bool &showRocket, bool &bomb_Visible, bool &bomb_flip_Visible,
             Uint32 &nextBombTime, Uint32 &nextBombflipTime)
{
    flip = 0;
    character.y = 328;
    point = 0;
    inGame = true;
    lose_menu = false;
    moveDown = false;
    moveUp = false;

    showBomb = false;
    showBombflip = false;
    showRocket = false;
    bomb_Visible = false;
    bomb_flip_Visible = false;

    nextBombTime = SDL_GetTicks() + (rand() % 5000) + 1000;
    nextBombflipTime = SDL_GetTicks() + (rand() % 5000) + 1000;
}

void scrollingg_background(int &backgroundX, float BACKGROUND_SPEED, int SCREEN_WIDTH)
{
    backgroundX -= BACKGROUND_SPEED;
    if (backgroundX <= -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}
#endif
