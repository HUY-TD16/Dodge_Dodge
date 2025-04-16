#pragma once

#include <SDL.h>
#include <vector>
#include "entities.h"
#include "player.h"
#include "fireball.h"
#include "global.h"
#include "lasers.h"
#include "explosions.h"
#include "skill.h"

class Collision {
public:
	Collision();
	bool checkCollision(Player& player, bool isInvincible,
		const std::vector<FireballIndex>& fireball_, FireBall& f,
		const std::vector<LaserIndex>& laser_, Lasers& l,
		const std::vector<ExplosionIndex>& explosion_, Explosion& e);

};