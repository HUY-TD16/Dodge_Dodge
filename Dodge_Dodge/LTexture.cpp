#include "LTexture.h"
#include <string>
#include <iostream>

extern SDL_Renderer* gRenderer;

LTexture::LTexture() : mTexture(nullptr), mWidth(0), mHeight(0) {}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(std::string path) {
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (!mTexture) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return mTexture != nullptr;
}
bool LTexture::loadFromRenderText(std::string text, SDL_Color color) {
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error:" << TTF_GetError() << std::endl;
    }
    else {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    return mTexture != NULL;
}

void LTexture::free() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y) {
    SDL_Rect renderQuad = { x , y , mWidth , mHeight };
    SDL_RenderCopy(gRenderer, mTexture, nullptr, &renderQuad);

}
void LTexture::render(int x, int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = { x , y , mWidth , mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}
void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = { x , y , mWidth , mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}
void LTexture::render(int x, int y, SDL_Rect* clip, SDL_Rect* size) {
    SDL_RenderCopy(gRenderer, mTexture, clip, size);
}
int LTexture::getWidth() { return mWidth; }
int LTexture::getHeight() { return mHeight; }
