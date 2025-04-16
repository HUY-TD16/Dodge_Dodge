#pragma once

#include <SDL.h>
#include <vector>
#include "entities.h"
#include "global.h"
#include "player.h"
#include <map>
#include <set>

enum class SkillInfo {
	INVINCIBILITY,
	DASH,
	CLEAR,
	DEFAULT
};
class Skills {
public:
	Skills();
	void reset(Uint32 currentTime);
	void handleInput(const Uint8* key, Uint32 currentTime, SDL_Event& event, Player& player, std::vector<ClearEffect>& clearEffect, std::vector<FireballIndex>& fireball);
	void update(Uint32 currentTime /*, std::vector<ClearEffect>& clearEffect,
		std::vector<FireballIndex>& fireball*/);
	bool res() { return isInvincible; }
	Uint32 getLastInvicibility() { return lastInvincibilityTime; }
	Uint32 getLastDashTime() { return lastDashTime; }
	Uint32 getLastClear() { return lastClearTime; }

	void loadMediaSkill();
	void render(int x, int y);
	void setSkill(SkillInfo skilll, Uint32 currentTime);
private:
	Uint32 lastInvincibilityTime;
	Uint32 invincibilityEndTime;
	const Uint32 invincibilityDuration = 2000;
	bool isInvincible;
	Uint32 lastDashTime;
	Uint32 lastClearTime;
	/*const Uint32 clearDuration = 500;*/
	const float clearRadius = PLAYER_SIZE * 5.0f;

	struct FrameInfo {
		LTexture gSkill;
		int frameCount;
		int frameWidth;
		int frameHeight;
		Uint32 timeSkill;
		int center;
	};
	std::map<SkillInfo, FrameInfo> animationFrame;
	SkillInfo currentSkill;

	int currentFrame;
	Uint32 lastUpdateTime;
	int frameDelay;
	Uint32 lastTimeSkill;


};