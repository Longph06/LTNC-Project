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

void game_quit(SDL_Texture* bomb_texture, SDL_Texture* bomb_flip_texture,
              SDL_Texture* sound_off_texture, SDL_Texture* sound_on_texture,
              SDL_Texture* lose_texture, SDL_Texture* character_texture,
              SDL_Texture* home_texture, SDL_Texture* sound,
              SDL_Texture* rocket_texture, SDL_Texture* background,
              SDL_Renderer* renderer, SDL_Window* window,
              Mix_Music* music)
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
                            IMG_Quit();
                            SDL_Quit();
                            TTF_Quit();
                            Mix_FreeMusic(music);
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

void start_game(bool &gameOver, bool &inGame, bool &lose_menu,
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

void game_on(int &flip, SDL_Rect &character, int &point, bool &inGame, bool &lose_menu,
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
#endif // GAME_LOGIC_H
