#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "global.h"
#include "Ltexture.h"

class Player {

private:
	int mPosX, mPosY;
	int mVelX, mVelY;
public:
	static const int PLAYER_SIZE = 20;
	static const int PLAYER_WIDTH = 80;
	static const int PLAYER_HEIGHT = 64;
	Player();
	void handleEvent(SDL_Event& e);
	void move();
	int getX();
	int getY();
	int VelX() { return mVelX; }
	int VelY() { return mVelY; }
	void dash(int dashDX, int dashDY);
};