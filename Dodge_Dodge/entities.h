#pragma once

#include <SDL.h>
#include <stdlib.h>

struct FireballIndex {
	float fPosX, fPosY;
	float fVelX, fVelY;
};

struct LaserIndex {
	float lPosX, lPosY;
	float lVelX, lVelY;
	Uint32 spawnTime;
	bool isActive;
};

struct ExplosionIndex {
	float ePosX, ePosY;
	Uint32 spawnTime;
	bool isActive;
};

struct ClearEffect {
	float x, y;
	Uint32 SpawnTime;
};

