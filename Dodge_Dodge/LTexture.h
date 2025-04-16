#pragma once
#include "global.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class LTexture
{
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(std::string path);
    bool loadFromRenderText(std::string text, SDL_Color color);
    void free();
    void render(int x, int y);
    void render(int x, int y, SDL_Rect* clip);
    void render(int x, int y, SDL_Rect* clip, SDL_Rect* size);
    void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
    int getWidth();
    int getHeight();
private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};
