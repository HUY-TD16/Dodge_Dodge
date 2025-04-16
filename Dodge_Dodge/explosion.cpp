#include "explosions.h"
#include <iostream>

Explosion::Explosion() {
	spriteClips[0] = { 0 , 0 , 64 , 64 };
	spriteClips[1] = { 64 , 0 , 64 , 64 };
	spriteClips[2] = { 64 * 2 , 0 , 64 , 64 };
	spriteClips[3] = { 64 * 3 , 0 , 64 , 64 };
	spriteClips[4] = { 64 * 4 , 0 , 64 , 64 };
	spriteClips[5] = { 64 * 5 , 0 , 64 , 64 };
	spriteClips[6] = { 64 * 6 , 0 , 64 , 64 };
	spriteClips[7] = { 64 * 7 , 0 , 64 , 64 };
	spriteClips[8] = { 64 * 8 , 0 , 64 , 64 };
	spriteClips[9] = { 65 * 9 , 0 , 64 , 64 };
	Explosion_Sprite.loadFromFile("Image/explosion.png");
}

void Explosion::spawn(Uint32 currentTime, Uint32& lastExplosionTime, Uint32 gameStartTime) {
	if (currentTime - gameStartTime >= explosionStartDelay && currentTime - lastExplosionTime >= explosionInterval) {
		for (int i = 0; i < explosionPerSpwan; i++) {
			ExplosionIndex explosion;
			explosion.ePosX = static_cast<float>(std::rand() % (SCREEN_WIDTH - explosionRadius));
			explosion.ePosY = static_cast<float>(std::rand() % (SCREEN_HEIGHT - explosionRadius));
			explosion.spawnTime = currentTime;
			explosion.isActive = false;
			explosion_.push_back(explosion);

			lastExplosionTime = currentTime;
			std::cout << "Explosion spawned at: " << explosion_.back().ePosX << " " << explosion_.back().ePosY << std::endl;
		}
	}
}
void Explosion::reset(Uint32 currentTime) {
	explosion_.clear();
	explosionPerSpwan = 1;
	lastIncreaseTime = currentTime + explosionStartDelay;
	frame = 0;
	lastFrameTime = currentTime;
}
void Explosion::update(Uint32 currentTime) {
	for (ExplosionIndex& explosion : explosion_) {
		if (!explosion.isActive && currentTime - explosion.spawnTime >= explosionWarningTime) {
			explosion.isActive = true;
		}
	}

	/*explosion_.erase(
		std::remove_if(explosion_.begin(), explosion_.end(), [currentTime , this](const ExplosionIndex& e) {
			return e.isActive && currentTime - e.spawnTime >= (explosionWarningTime + explosionActiveTime);
			}),
		explosion_.end()
	);*/
	explosion_.erase(
		std::remove_if(explosion_.begin(), explosion_.end(),
			[currentTime, this](const ExplosionIndex& e) {
				// Kiểm tra nếu explosion đã tồn tại lâu hơn thời gian cảnh báo + thời gian hoạt động
				return e.isActive && (currentTime - e.spawnTime >= (explosionWarningTime + explosionActiveTime));
			}
		),
		explosion_.end()
	);

	if (currentTime - lastFrameTime >= frameDelay) {
		frame = (frame + 1) % 10;
		lastFrameTime = currentTime;
	}

	if ((currentTime > lastIncreaseTime) && (currentTime - lastIncreaseTime >= increaseInterval)) {
		if (explosionPerSpwan < maxExplosionsPerSpawn) {
			explosionPerSpwan++;

		}
		lastIncreaseTime = currentTime;
		std::cout << "explosionPreSpawn =" << explosionPerSpwan << std::endl;
	}

}

void Explosion::render(SDL_Renderer* renderer) {
	currentClip = spriteClips[frame];
	for (const auto& explosion : explosion_) {
		SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
		SDL_Rect explosionRect = { static_cast<int>(explosion.ePosX), static_cast<int>(explosion.ePosY),
								   explosionRadius, explosionRadius };
		if (explosion.isActive) {
			if (soundOn) {
				Mix_PlayChannel(-1, musicExplosion, 0);
			}
			Explosion_Sprite.render(static_cast<int>(std::round(explosion.ePosX)), static_cast<int>(std::round(explosion.ePosY)), &currentClip, &explosionRect);
		}
		else {
			SDL_RenderDrawRect(renderer, &explosionRect);
		}
	}
}