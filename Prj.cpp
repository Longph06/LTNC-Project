#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include "Constants.h"
#include "GameLogic.h"
#include "Renderer.h"

using namespace std;

float CHARACTER_SPEED = 5;
float BOMB_SPEED = 9;
int ROCKET_START_Y = 0;
float ROCKET_SPEED = 10;
float delay = 16;
int backgroundX = 0;
float BACKGROUND_SPEED = 3;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    SDL_Window* window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* background = loadTexture("Interface.png", renderer);
    SDL_Texture* bomb_texture = loadTexture("bomb.png", renderer);
    SDL_Texture* bomb_flip_texture = loadTexture("bomb_flip.png", renderer);
    SDL_Texture* character_texture = loadTexture("Character.png", renderer);
    SDL_Texture* lose_texture = loadTexture("lose.png", renderer);
    SDL_Texture* sound_on_texture = loadTexture("sound_on.png", renderer);
    SDL_Texture* sound_off_texture = loadTexture("sound_off.png", renderer);
    SDL_Texture* rocket_texture = loadTexture("rocket.png", renderer);
    SDL_Texture* sound;
    SDL_Texture* home_texture = loadTexture("home.png", renderer);

    SDL_Rect bomb = {BOMB_START_X, BOMB_START_Y1, BOMB_WIDTH, BOMB_HEIGHT};
    SDL_Rect bomb_flip = {BOMB_START_X, BOMB_START_Y2, BOMB_WIDTH, BOMB_HEIGHT};
    SDL_Rect character = {CHARACTER_X, CHARACTER_Y1, CHARACTER_WIDTH, CHARACTER_HEIGHT};
    SDL_Rect lose = {LOSE_X, LOSE_Y, LOSE_WIDTH, LOSE_HEIGHT};
    SDL_Rect soundd = {SOUND_X, SOUND_Y, SOUND_WIDTH, SOUND_HEIGHT};
    SDL_Rect rocket = {ROCKET_START_X, ROCKET_START_Y, ROCKET_WIDTH, ROCKET_HEIGHT};
    SDL_Rect home = {HOME_START_X, HOME_START_Y, HOME_WIDTH, HOME_HEIGHT};

    if (TTF_Init() == -1)
    {
        cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        return -1;
    }
    TTF_Font* font = TTF_OpenFont("Aller.ttf", 24);
    if (!font)
    {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return -1;
    }
    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* music = Mix_LoadMUS("GameSound.wav");
    if (!music)
    {
        cout << "Không mở được nhạc: " << Mix_GetError() << endl;
        return -1;
    }
    Mix_PlayMusic(music, -1);

    Uint32 nextBombTime = SDL_GetTicks() + (rand() % 7000) + 2000;
    Uint32 nextBombflipTime = SDL_GetTicks() + (rand() % 7000) + 2000;
    Uint32 nextRocketTime = SDL_GetTicks() + (rand() % 10000) + 5000;

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

    int sound_check = 0;
    int flip = 0;
    bool moveUp = false;
    bool moveDown = false;
    int targetY = character.y;
    int point = 0;
    int mod;
    int ms;

    sound = sound_on_texture;
    SDL_Event event;

    while(running)
    {
        SDL_RenderClear(renderer);
        if (inGame == true)
        {
            backgroundX -= BACKGROUND_SPEED;
            if (backgroundX <= -SCREEN_WIDTH)
            {
                backgroundX = 0;
            }
            SDL_Rect bgRect1 = {backgroundX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Rect bgRect2 = {backgroundX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, background, NULL, &bgRect1);
            SDL_RenderCopy(renderer, background, NULL, &bgRect2);

            string pointsText = "Point: " + to_string(point);
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
            SDL_Texture* pointsTexture = renderText(pointsText, font, textColor, renderer);
            if (pointsTexture)
            {
                int texW = 0;
                int texH = 0;
                SDL_QueryTexture(pointsTexture, NULL, NULL, &texW, &texH);
                SDL_Rect dstRect = {SCREEN_WIDTH - texW - 30, 20, texW, texH};
                SDL_RenderCopy(renderer, pointsTexture, NULL, &dstRect);
                SDL_DestroyTexture(pointsTexture);
            }
        }
        else
        {
            SDL_RenderCopy(renderer, background, NULL, NULL);
        }
        SDL_RenderCopy(renderer, home_texture, NULL, &home);
        SDL_RenderCopy(renderer, sound, NULL, &soundd);

        game_sound(sound_check, sound, sound_on_texture, sound_off_texture, renderer, soundd);

        if(inGame)
        {
            while (SDL_PollEvent(&event) && !lose_menu)
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if(mouseX >= SOUND_X && mouseX <= (SOUND_X + SOUND_WIDTH) && mouseY >= SOUND_Y && mouseY <= (SOUND_Y + SOUND_WIDTH))
                    {
                        sound_check++;
                    }

                    if (mouseX >= (SOUND_WIDTH + SOUND_X) && mouseX <= 1000 && mouseY >= 0 && mouseY <= 550)
                    {
                        check_movement(flip, targetY, moveDown, moveUp);

                    }
                    if(mouseX >= HOME_START_X && mouseX <= (HOME_START_X + HOME_WIDTH) && mouseY >= HOME_START_Y && mouseY <= (HOME_START_Y + HOME_HEIGHT))
                    {
                        SDL_DestroyTexture(background);
                        background = loadTexture("Interface.png", renderer);
                        inGame = false;
                    }
                }
            }

            if(!gameOver)
            {
                if (moveUp && character.y > targetY)
                {
                    character.y -= CHARACTER_SPEED;
                    if (character.y <= targetY)
                    {
                        character.y = targetY;
                        moveUp = false;
                    }
                }
                else if (moveDown && character.y < targetY)
                {
                    character.y += CHARACTER_SPEED;
                    if (character.y >= targetY)
                    {
                        character.y = targetY;
                        moveDown = false;
                    }
                }
                game_property(point, ms, BOMB_SPEED, mod, BACKGROUND_SPEED, delay, ROCKET_SPEED);

                if (SDL_GetTicks() >= nextBombTime && !bomb_Visible)
                {
                    showBomb = true;
                    bomb_Visible = true;
                    bomb.x = BOMB_START_X;
                    nextBombTime = SDL_GetTicks() + (rand() % 4000) + ms;
                }

                if (showBomb)
                {
                    bomb.x -= BOMB_SPEED;
                    SDL_RenderCopy(renderer, bomb_texture, NULL, &bomb);
                    if (bomb.x + BOMB_WIDTH < 0)
                    {
                        showBomb = false;
                        bomb_Visible = false;
                        point++;
                        std::cout << point << std::endl;
                    }
                }

                if (SDL_GetTicks() >= nextBombflipTime && !bomb_flip_Visible)
                {
                    showBombflip = true;
                    bomb_flip_Visible = true;
                    bomb_flip.x = BOMB_START_X;
                    nextBombflipTime = SDL_GetTicks() + (rand() % 4000) + ms;
                }

                if (showBombflip)
                {
                    bomb_flip.x -= BOMB_SPEED;
                    SDL_RenderCopy(renderer, bomb_flip_texture, NULL, &bomb_flip);
                    if (bomb_flip.x + BOMB_WIDTH < 0)
                    {
                        showBombflip = false;
                        point++;
                        bomb_flip_Visible = false;
                    }
                }

                if (SDL_GetTicks() >= nextRocketTime && !rocket_Visible && Valid_Rocket(bomb, bomb_flip) )
                {
                    showRocket = true;
                    rocket_Visible = true;
                    rocket.x = ROCKET_START_X;
                    rocket.y = (220 + rand() % 50);
                    nextRocketTime = SDL_GetTicks() + (rand() % 8000) + mod;
                }

                if (showRocket)
                {
                    rocket.x -= ROCKET_SPEED;
                    SDL_RenderCopy(renderer, rocket_texture, NULL, &rocket);
                    if (rocket.x + ROCKET_WIDTH < 0)
                    {
                        showRocket = false;
                        rocket_Visible = false;
                    }
                }

                if (checkCollision(character, bomb) || checkCollision(character, bomb_flip) || checkCollision(character, rocket))
                {
                    gameOver = true;
                    lose_menu = true;
                }
            }

            if(flip % 2 != 0)
            {
                SDL_RenderCopyEx(renderer, character_texture, NULL, &character, 0, NULL, SDL_FLIP_VERTICAL);
            }
            else
            {
                SDL_RenderCopy(renderer, character_texture, NULL, &character);
            }
        }

        while(gameOver && lose_menu)
        {
            SDL_RenderCopy(renderer, lose_texture, NULL, &lose);
            string highest_score = "Highest point: " + to_string(point);
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
            SDL_Texture* highestpointsTexture = renderText(highest_score, font, textColor, renderer);
            if (highestpointsTexture)
            {
                int texxW = 0;
                int texxH = 0;
                SDL_QueryTexture(highestpointsTexture, NULL, NULL, &texxW, &texxH);
                SDL_Rect hstRect = {SCREEN_WIDTH - texxW - 413, 250, texxW, texxH};
                SDL_RenderCopy(renderer, highestpointsTexture, NULL, &hstRect);
                SDL_DestroyTexture(highestpointsTexture);
            }
            SDL_RenderPresent(renderer);

            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    running = false;
                    gameOver = false;
                }
                if(event.type == SDL_MOUSEBUTTONUP)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if(mouseX >= 430 && mouseX <= 570 && mouseY >= 250 && mouseY <= 365)
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
                }
            }
        }
        SDL_RenderPresent(renderer);

            if(inGame == false)
            {
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        running = false;
                    }
                    if (event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        if(mouseX >= HOME_START_X && mouseX <= (HOME_START_X + HOME_WIDTH) && mouseY >= HOME_START_Y && mouseY <= (HOME_START_Y + HOME_HEIGHT))
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
                            return 0;
                        }

                        if(mouseX >= SOUND_X && mouseX <= (SOUND_X + SOUND_WIDTH) && mouseY >= SOUND_Y && mouseY <= (SOUND_Y + SOUND_WIDTH))
                        {
                            sound_check++;
                        }

                        if (mouseX >= 630 && mouseX <= 960 && mouseY >= 420 && mouseY <= 510)
                        {
                            SDL_DestroyTexture(background);
                            background = loadTexture("Instruction.png", renderer);
                            instruction = true;
                        }

                        if (mouseX >= 50 && mouseX <= 400 && mouseY >= 280 && mouseY <= 510 && instruction == false)
                        {
                            SDL_DestroyTexture(background);
                            background = loadTexture("Ingame.png", renderer);
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

                        if (mouseX >= 0 && mouseX <= 174 && mouseY >= 490 && mouseY <= 550)
                        {
                            SDL_DestroyTexture(background);
                            background = loadTexture("Interface.png", renderer);
                            instruction = false;
                        }
                    }
                }
            }
        SDL_RenderPresent(renderer);
        SDL_Delay(delay);
    }

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
    return 0;
}
