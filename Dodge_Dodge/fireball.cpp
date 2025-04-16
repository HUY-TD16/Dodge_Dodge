#include "fireball.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "player.h"
#include "global.h"
#include "entities.h"
#include <iostream>
#include "global.h"

extern SDL_Renderer* gRenderer;
extern Uint32 lastFrameTime;

FireBall::FireBall() {
	spriteClips[0] = { 0 , 0 , 64 , 64 };
	spriteClips[1] = { 64 , 0 , 64 , 64 };
	Fireball_Sprite.loadFromFile("Image/fireball_sprite.png");
	lastIncreaseTime = SDL_GetTicks();
}
void FireBall::reset(Uint32 currentTime) {
	fireball_.clear();
	fireballSpawn = 1;
	lastIncreaseTime = currentTime;
}
void FireBall::spawn(Uint32 currentTime, Player& player, Uint32& lastSpawmTime) {
	if (currentTime - lastSpawmTime >= spawnInterval_) {
		for (int i = 0; i < fireballSpawn; i++) {
			//Mix_PlayChannel(-1, musicFireball, 0);

			FireballIndex fireball{};
			int side = std::rand() % 4;
			switch (side) {
			case 0:
				fireball.fPosX = static_cast<float>(rand() % SCREEN_WIDTH);
				fireball.fPosY = -FIREBALL_SIZE;
				break;
			case 1:
				fireball.fPosX = static_cast<float>(rand() % SCREEN_WIDTH);
				fireball.fPosY = SCREEN_HEIGHT;
				break;
			case 2:
				fireball.fPosX = -FIREBALL_SIZE;
				fireball.fPosY = static_cast<float>(rand() % SCREEN_HEIGHT);
				break;
			case 3:
				fireball.fPosX = SCREEN_WIDTH;
				fireball.fPosY = static_cast<float>(rand() % SCREEN_HEIGHT);
				break;
			}
			//std::cout << "Spawning fireball at (" << fireball.fPosX << ", " << fireball.fPosY << ")\n";
			/*fireball.fPosX = std::rand() % SCREEN_WIDTH;
			fireball.fPosY = std::rand() % SCREEN_HEIGHT;
			fireball.fVelX = static_cast<int>((std::rand() % 2 == 0) ? FIREBALL_SPEED : -FIREBALL_SPEED);
			fireball.fVelY = static_cast<int>((std::rand() % 2 == 0) ? FIREBALL_SPEED : -FIREBALL_SPEED);*/
			float dx = player.getX() + PLAYER_SIZE / 2.0f - (fireball.fPosX + FIREBALL_SIZE / 2.0f);
			float dy = player.getY() + PLAYER_SIZE / 2.0f - (fireball.fPosY + FIREBALL_SIZE / 2.0f);
			float distance = std::sqrt(dx * dx + dy * dy);
			fireball.fVelX = (dx / distance) * FIREBALL_SPEED;
			fireball.fVelY = (dy / distance) * FIREBALL_SPEED;
			fireball_.push_back(fireball);

			lastSpawmTime = currentTime;
		}
	}
}

void  FireBall::update(float deltaTime, Uint32 currentTime) {

	for (FireballIndex& fireball : fireball_) {
		fireball.fPosX += fireball.fVelX * deltaTime;
		fireball.fPosY += fireball.fVelY * deltaTime;

	}
	/*fireball_.erase(
		std::remove_if(fireball_.begin(), fireball_.end(), [](const FireballIndex& o) {
			return o.fPosX < -FIREBALL_SIZE || o.fPosX > SCREEN_WIDTH || o.fPosY < -FIREBALL_SIZE || o.fPosY > SCREEN_HEIGHT;
			}), fireball_.end()
				);*/
	if (!fireball_.empty()) {
		fireball_.erase(
			std::remove_if(fireball_.begin(), fireball_.end(), [](const FireballIndex& o) {
				return o.fPosX < -FIREBALL_SIZE || o.fPosX > SCREEN_WIDTH || o.fPosY < -FIREBALL_SIZE || o.fPosY > SCREEN_HEIGHT;
				}),
			fireball_.end()
		);
	}
	if (currentTime - lastFrameTime >= frameDelay) {
		frame = (frame + 1) % 2; // Chuyển đổi giữa 2 frame
		lastFrameTime = currentTime;
	}

	if (currentTime - lastIncreaseTime >= increaseInterval) {
		if (fireballSpawn < maxFireballSpawn) {
			fireballSpawn++;
		}
		lastIncreaseTime = currentTime;
		std::cout << "Increase Fireball : FireballSpawn = " << fireballSpawn << std::endl;
	}

}


void FireBall::render(SDL_Renderer* gRenderer) {

	currentClip = spriteClips[frame];
	for (const auto& fireball : fireball_) {
		double angle = atan2(fireball.fVelY, fireball.fVelX) * (180.0 / M_PI);
		SDL_Rect fireballRect = { static_cast<int>(std::round(fireball.fPosX)) , static_cast<int>(std::round(fireball.fPosY)) , FIREBALL_SIZE , FIREBALL_SIZE };
		Fireball_Sprite.render(static_cast<int>(std::round(fireball.fPosX)), static_cast<int>(std::round(fireball.fPosY)), &currentClip, angle, nullptr, SDL_FLIP_NONE);
	}
}
