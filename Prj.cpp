#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>

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
string filename = "highscore.txt";

int readHighScore(const string& filename) {
    ifstream file(filename);
    int highScore = 0;

    if (file.is_open()) {
        file >> highScore;
        file.close();
    }

    return highScore;
}

void saveHighScore(const string& filename, int newScore) {
    int currentHighScore = readHighScore(filename);

    if (newScore > currentHighScore) {
        ofstream file(filename);
        if (file.is_open()) {
            file << newScore;
            file.close();
        }
    }
}

void resetHighScoreFile(const string& filename) {
    ofstream file(filename, ios::out | ios::trunc);
    if (file.is_open()) {
        file << 0;
        file.close();
    } else {
        cerr << "Không thể reset file highscore." << endl;
    }
}

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
    SDL_Texture* home_texture = loadTexture("home.png", renderer);
    SDL_Texture* rocket_texture = loadTexture("rocket.png", renderer);
    SDL_Texture* sound_on_texture = loadTexture("sound_on.png", renderer);
    SDL_Texture* sound_off_texture = loadTexture("sound_off.png", renderer);
    SDL_Texture* sound;

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
    Mix_Music* music = Mix_LoadMUS("GameSound.mp3");
    Mix_Chunk* rocket_sound = Mix_LoadWAV("Rocket.wav");
    Mix_Chunk* collision = Mix_LoadWAV("Collision.wav");
    Mix_PlayMusic(music, -1);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(rocket_sound, 128);
    Mix_VolumeChunk(collision, 128);

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
    bool moveUp = false;
    bool moveDown = false;
    bool isPaused = false;
    bool scrolling_bg = true;

    int sound_check = 0;
    int flip = 0;
    int targetY = character.y;
    int point = 0;
    int mod;
    int ms;
    int rkt = -1;
    int col = -1;

    sound = sound_on_texture;
    SDL_Event event;

    while(running)
    {
        SDL_RenderClear(renderer);
        if (inGame == true)
        {
            if(scrolling_bg)
            scrollingg_background(backgroundX, BACKGROUND_SPEED, SCREEN_WIDTH);

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

        game_sound(sound_check, sound, sound_on_texture, sound_off_texture, renderer, soundd, col, rkt);

        if(inGame)
        {
            while (SDL_PollEvent(&event) && !lose_menu)
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_SPACE )
                    {
                        isPaused = !isPaused;
                        scrolling_bg = !scrolling_bg;
                    }
                }

                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
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
            }

            if(!gameOver && !isPaused)
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

                ShowBomb(bomb, bomb_texture, renderer, BOMB_SPEED, showBomb, bomb_Visible, point);

                if (SDL_GetTicks() >= nextBombflipTime && !bomb_flip_Visible)
                {
                    showBombflip = true;
                    bomb_flip_Visible = true;
                    bomb_flip.x = BOMB_START_X;
                    nextBombflipTime = SDL_GetTicks() + (rand() % 4000) + ms;
                }

                ShowBombFlip(bomb_flip, bomb_flip_texture, renderer, BOMB_SPEED, showBombflip, bomb_flip_Visible, point);

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
                    if(rkt == -1)
                        rkt = Mix_PlayChannel(-1, rocket_sound, 0);
                    rocket.x -= ROCKET_SPEED;
                    SDL_RenderCopy(renderer, rocket_texture, NULL, &rocket);
                    if (rocket.x + ROCKET_WIDTH < 0)
                    {
                        showRocket = false;
                        rocket_Visible = false;
                        rkt = -1;
                    }
                }

                if (checkCollision(character, bomb) || checkCollision(character, bomb_flip) || checkCollision(character, rocket))
                {
                    if (col == -1)
                    {
                        col = Mix_PlayChannel(-1, collision, 0);
                    }
                    rkt = 0;
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

        if (isPaused)
        {
            scrolling_bg = false;
            SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_RenderFillRect(renderer, &overlayRect);
            SDL_RenderFillRect(renderer, NULL);
            SDL_Texture* pauseTexture = renderText("Game Paused. Press SPACE to Resume", font, textColor, renderer);
            SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, 300, 100};
            SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
            SDL_DestroyTexture(pauseTexture);
        }

        while(gameOver && lose_menu)
        {
            int currentScore = point;
            SDL_RenderCopy(renderer, lose_texture, NULL, &lose);
            saveHighScore(filename, currentScore);
            int highScore = readHighScore(filename);

            string highest_score = "Highest point: " + to_string(highScore);
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
                    resetHighScoreFile("highscore.txt");
                }
                if(event.type == SDL_MOUSEBUTTONUP)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        if(mouseX >= 430 && mouseX <= 570 && mouseY >= 250 && mouseY <= 365)
                        {
                            start_game(gameOver, inGame, lose_menu, showBomb, showBombflip, showRocket, bomb_Visible, bomb_flip_Visible, rocket_Visible,
                              nextBombTime, nextBombflipTime, nextRocketTime, rocket, bomb, bomb_flip, flip, point, character, moveDown, moveUp, targetY);
                            rkt = -1;
                            col = -1;
                        }
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
                    resetHighScoreFile("highscore.txt");
                }
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        if(mouseX >= HOME_START_X && mouseX <= (HOME_START_X + HOME_WIDTH) && mouseY >= HOME_START_Y && mouseY <= (HOME_START_Y + HOME_HEIGHT))
                        {
                            resetHighScoreFile("highscore.txt");
                            game_quit(bomb_texture, bomb_flip_texture, sound_off_texture, sound_on_texture, lose_texture,
                                      character_texture, home_texture, sound, rocket_texture, background, renderer, window, music, rocket_sound, collision);
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

                            game_on(flip, character, point, inGame, lose_menu, moveDown, moveUp, showBomb,
                                    showBombflip, showRocket, bomb_Visible, bomb_flip_Visible, nextBombTime,nextBombflipTime);
                            rkt = -1;
                            col = -1;
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
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(delay);
    }
    resetHighScoreFile("highscore.txt");

    game_quit(bomb_texture, bomb_flip_texture, sound_off_texture, sound_on_texture, lose_texture,
              character_texture, home_texture, sound, rocket_texture, background, renderer, window, music, rocket_sound, collision);
    return 0;
}
