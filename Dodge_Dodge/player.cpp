#include <SDL.h>
#include <SDL_image.h>
#include "player.h"
#include <iostream>

extern SDL_Renderer* gRenderer;
Player::Player() {
	mPosX = SCREEN_WIDTH / 2 - PLAYER_SIZE / 2;
	mPosY = SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2;
	mVelX = 0;
	mVelY = 0;
}
void Player::handleEvent(SDL_Event& e) {
	//std::cerr << "fhihifu\n";
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {

		switch (e.key.keysym.sym) {
		case SDLK_UP: mVelY -= PLAYER_VEL; break;
		case SDLK_DOWN: mVelY += PLAYER_VEL; break;
		case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
		case SDLK_RIGHT: mVelX += PLAYER_VEL; break;

		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {

		switch (e.key.keysym.sym) {
		case SDLK_UP: mVelY += PLAYER_VEL; break;
		case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
		case SDLK_LEFT: mVelX += PLAYER_VEL; break;
		case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;

		}
	}
}
void Player::move() {
	mPosX += mVelX;
	if ((mPosX < 0) || (mPosX > SCREEN_WIDTH)) {
		mPosX -= mVelX;
	}
	mPosY += mVelY;

	//If the dot went too far up or down
	if ((mPosY < 0) || (mPosY > SCREEN_HEIGHT))
	{
		//Move back
		mPosY -= mVelY;
	}
}
int Player::getX() { return mPosX; }
int Player::getY() { return mPosY; }
void Player::dash(int dashDX, int dashDY) {
	mPosX += dashDX;
	mPosY += dashDY;
}