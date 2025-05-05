#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
using namespace std;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == nullptr)
    {
        cout << "Failed to load image! SDL_image Error: " << IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

SDL_Texture* renderText(const string &message, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if(!surface)
    {
        cerr << "Failed to create surface: " << TTF_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if(!texture)
    {
        cerr << "Failed to create texture: " << SDL_GetError() << endl;
        return nullptr;
    }
    return texture;
}

#endif
