#pragma once
#include <vector>
#include "entities.h"
#include "global.h"
#include "player.h"
#include <SDL.h>

class Lasers {
public:
	Lasers();
	void reset(Uint32 currentTime);
	void spawn(Uint32 currentTime, Player& player, Uint32& lastLaserTime, Uint32 gameStartTime);
	void update(Uint32 currentTime);
	std::vector<LaserIndex>& getLaser() { return laser_; }
	void render(SDL_Renderer* gRenderer);
private:
	std::vector<LaserIndex> laser_;
	int laserPerSpawn = 10;
	const Uint32 laserInterval = 3000;
	const Uint32 laserWarningTime = 1000;
	const Uint32 laserActiveTime = 1000;
	const int maxLasersPerSpawn = 5;
	const int laserStartDelay = 10000;
	Uint32 lastIncreaseTime = 10000;

};