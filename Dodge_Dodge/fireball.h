#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "entities.h"
#include "global.h"
#include "LTexture.h"
#include "player.h"
#include <vector>


class FireBall {
private:
	std::vector<FireballIndex> fireball_;
	int fireballSpawn = 1;
	int maxFireballSpawn = 5;
	const Uint32 spawnInterval_ = 1200;
	LTexture Fireball_Sprite;
	SDL_Rect spriteClips[2];
	int frame = 0;
	SDL_Rect currentClip;
	Uint32 lastFrameTime;
	Uint32 lastIncreaseTime;

public:
	double scaleX = FIREBALL_SIZE / 64.0;
	double scaleY = FIREBALL_SIZE / 64.0;
	int hitboxWidth = static_cast<int>(54 * scaleX);
	int hitboxHeight = static_cast<int>(26 * scaleX);

	FireBall();
	void reset(Uint32 currentTime);
	void spawn(Uint32 currentTime, Player& player, Uint32& lastSpawnTime);
	void update(float deltaTime, Uint32 curentTime);
	std::vector<FireballIndex>& getFireball() { return fireball_; }
	void render(SDL_Renderer* gRenderer);
};