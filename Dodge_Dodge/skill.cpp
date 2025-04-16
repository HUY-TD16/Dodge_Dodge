#include "skill.h"
#include <cmath>
#include <iostream>

Skills::Skills() {
	lastInvincibilityTime = SDL_GetTicks();
	invincibilityEndTime = SDL_GetTicks();
	isInvincible = false;
	lastDashTime = SDL_GetTicks();
	lastClearTime = SDL_GetTicks();

	currentSkill = SkillInfo::DEFAULT;
	currentFrame = 0;
	lastUpdateTime = 0;
	frameDelay = 10;
	lastTimeSkill = 0;
}
void Skills::reset(Uint32 currentTime) {
	lastInvincibilityTime = currentTime;
	invincibilityEndTime = currentTime;
	lastDashTime = currentTime;
	lastClearTime = currentTime;
	isInvincible = false;

	currentSkill = SkillInfo::DEFAULT;
	currentFrame = 0;
	lastUpdateTime = 0;
	frameDelay = 10;
}

void Skills::loadMediaSkill() {
	animationFrame[SkillInfo::INVINCIBILITY].gSkill.loadFromFile("Image/invincibility.png");
	animationFrame[SkillInfo::INVINCIBILITY].frameCount = 10;
	animationFrame[SkillInfo::INVINCIBILITY].frameWidth = PLAYER_SIZE * 4;
	animationFrame[SkillInfo::INVINCIBILITY].frameHeight = PLAYER_SIZE * 4;
	animationFrame[SkillInfo::INVINCIBILITY].timeSkill = 2000;
	animationFrame[SkillInfo::INVINCIBILITY].center = PLAYER_SIZE / 2 - PLAYER_SIZE * 2;

	animationFrame[SkillInfo::DASH].gSkill.loadFromFile("Image/dash.png");
	animationFrame[SkillInfo::DASH].frameCount = 11;
	animationFrame[SkillInfo::DASH].frameWidth = PLAYER_SIZE * 4;
	animationFrame[SkillInfo::DASH].frameHeight = PLAYER_SIZE * 4;
	animationFrame[SkillInfo::DASH].timeSkill = 300;
	animationFrame[SkillInfo::DASH].center = PLAYER_SIZE / 2 - PLAYER_SIZE * 2;

	animationFrame[SkillInfo::CLEAR].gSkill.loadFromFile("Image/clearskill.png");
	animationFrame[SkillInfo::CLEAR].frameCount = 9;
	animationFrame[SkillInfo::CLEAR].frameWidth = PLAYER_SIZE * 5;
	animationFrame[SkillInfo::CLEAR].frameHeight = PLAYER_SIZE * 5;
	animationFrame[SkillInfo::CLEAR].timeSkill = 100;
	animationFrame[SkillInfo::CLEAR].center = PLAYER_SIZE / 2 - PLAYER_SIZE / 2 * 5;

	animationFrame[SkillInfo::DEFAULT].gSkill.loadFromFile("Image/clearskill.png");
	animationFrame[SkillInfo::DEFAULT].frameCount = 1;
	animationFrame[SkillInfo::DEFAULT].frameWidth = 0;
	animationFrame[SkillInfo::DEFAULT].frameHeight = 0;
	animationFrame[SkillInfo::DEFAULT].timeSkill = 0;



}

void Skills::handleInput(const Uint8* key, Uint32 currentTime, SDL_Event& event, Player& player, std::vector<ClearEffect>& clearEffect, std::vector<FireballIndex>& fireball) {
	if (key[SDL_SCANCODE_A] && (currentTime - lastInvincibilityTime >= invincibilityCoolDown) && !isInvincible) {
		if (soundOn) {
			Mix_PlayChannel(-1, musicInvinbility, 0);
		}
		isInvincible = true;
		lastInvincibilityTime = currentTime;
		invincibilityEndTime = currentTime + invincibilityDuration;

		setSkill(SkillInfo::INVINCIBILITY, currentTime);

		std::cout << "Invincibility activated\n";
	}
	else if (key[SDL_SCANCODE_S] && (currentTime - lastDashTime >= dashCoolDown)) {

		if (soundOn) Mix_PlayChannel(-1, musicDash, 0);
		int dx = player.VelX();
		int dy = player.VelY();

		if (dx != 0 || dy != 0) {
			float magnitude = std::sqrt(dx * dx + dy * dy);
			float dashDX = (dx / magnitude) * DASHDISTANCE;
			float dashDY = (dy / magnitude) * DASHDISTANCE;
			player.dash(static_cast<int>(std::round(dashDX)), static_cast<int>(std::round(dashDY)));
			lastDashTime = currentTime;
			std::cout << "Dash Activate\n";
			setSkill(SkillInfo::DASH, currentTime);
		}
	}
	else if (key[SDL_SCANCODE_D] && currentTime - lastClearTime >= clearCoolDown) {
		if (soundOn) Mix_PlayChannel(-1, musicClear, 0);
		float playerCenterX = player.getX() + PLAYER_SIZE / 2.0f;
		float playerCenterY = player.getY() + PLAYER_SIZE / 2.0f;

		ClearEffect effect;
		effect.x = playerCenterX - clearRadius / 2.0f;
		effect.y = playerCenterY - clearRadius / 2.0f;
		effect.SpawnTime = currentTime;
		clearEffect.push_back(effect);
		auto it = std::remove_if(
			fireball.begin(), fireball.end(), [&](const FireballIndex& f) {
				float dist = std::sqrt(
					std::pow(f.fPosX + FIREBALL_SIZE / 2.0f - playerCenterX, 2) +
					std::pow(f.fPosY + FIREBALL_SIZE / 2.0f - playerCenterY, 2)
				);
				return dist <= clearRadius;
			}
		);
		// Chỉ xóa nếu có ít nhất 1 phần tử cần xóa
		if (it != fireball.end()) {
			fireball.erase(it, fireball.end());
		}
		lastClearTime = currentTime;
		setSkill(SkillInfo::CLEAR, currentTime);
		std::cout << "Clear activate\n";

	}
	else {
		setSkill(SkillInfo::DEFAULT, currentTime);
	}

}

void Skills::update(Uint32 currentTime /*, std::vector<ClearEffect>& clearEffect,
	std::vector<FireballIndex>& fireball*/) {
	if (isInvincible && currentTime >= invincibilityEndTime) {
		isInvincible = false;
		std::cout << "Invincibility ended\n";
	}
	/*for (auto& clear : clearEffect) {
		if (currentTime - clear.SpawnTime <= clearDuration) {
			fireball.erase(
				std::remove_if(fireball.begin(), fireball.end(), [&](const FireballIndex& f) {
					float dist = std::sqrt(
						std::pow(f.fPosX + FIREBALL_SIZE / 2.0f - clear.x + clearRadius/2.0f, 2) +
						std::pow(f.fPosY + FIREBALL_SIZE / 2.0f - clear.y + clearRadius/2.0f, 2)
					);
					return dist <= clearRadius;
					}),
				fireball.end()
			);
		}
	}
	clearEffect.erase(
		std::remove_if(clearEffect.begin(), clearEffect.end(), [currentTime](const ClearEffect& e) {
			return currentTime - e.SpawnTime >= 500;
			}),
		clearEffect.end()
	);*/
	if (currentTime > lastUpdateTime + frameDelay) {
		currentFrame = (currentFrame + 1) % animationFrame[currentSkill].frameCount;
		lastUpdateTime = currentTime;
	}
}

void Skills::setSkill(SkillInfo skilll, Uint32 currentTime) {
	if (currentTime >= lastTimeSkill + animationFrame[currentSkill].timeSkill && currentSkill != skilll) {
		currentSkill = skilll;
		currentFrame = 0;
		lastTimeSkill = currentTime;
	}
	//std::cerr << static_cast<int>(currentSkill) << std::endl;


}

void Skills::render(int x, int y) {
	if (currentSkill != SkillInfo::DEFAULT) {

		SDL_Rect srcRect = { currentFrame * 64 , 0 , 64 , 64 };
		SDL_Rect destRect = { x + animationFrame[currentSkill].center , y + animationFrame[currentSkill].center , animationFrame[currentSkill].frameWidth , animationFrame[currentSkill].frameHeight };
		animationFrame[currentSkill].gSkill.render(x, y, &srcRect, &destRect);

	}
}

