#pragma once

#include <vector>
#include "entities.h"
#include "global.h"
#include "player.h"

class Explosion {
public:
	Explosion();
	void reset(Uint32 currentTime);
	void spawn(Uint32 currentTime, Uint32& lastExplosionTime, Uint32 gameStartTime);
	void update(Uint32 currentTime);
	std::vector<ExplosionIndex>& getExplosion() { return explosion_; }
	void render(SDL_Renderer* gRenderer);

private:
	std::vector<ExplosionIndex> explosion_;
	int explosionPerSpwan = 8;
	const Uint32 explosionInterval = 3000;
	const Uint32 explosionWarningTime = 1000;
	const Uint32 explosionActiveTime = 1000;
	const int explosionRadius = PLAYER_SIZE * 10;
	const int maxExplosionsPerSpawn = 3;
	const Uint32 explosionStartDelay = 20000;
	Uint32 lastIncreaseTime = 20000;
	LTexture Explosion_Sprite;
	SDL_Rect spriteClips[10];
	int frame = 0;
	SDL_Rect currentClip;
	Uint32 lastFrameTime = 15000;

};