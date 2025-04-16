#include "lasers.h"
#include <cmath>
#include <iostream>

extern SDL_Renderer* gRenderer;

Lasers::Lasers() {}

void Lasers::spawn(Uint32 currentTime, Player& player, Uint32& lastLaserTime, Uint32 gameStartTime) {
	if (currentTime - gameStartTime >= laserStartDelay && currentTime - lastLaserTime >= laserInterval) {
		for (int i = 0; i < laserPerSpawn; i++) {
			LaserIndex laser;
			int corner = std::rand() % 4;
			switch (corner) {
			case 0:
				laser.lPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
				laser.lPosY = 0;
				break;
			case 1:
				laser.lPosX = static_cast<float>(std::rand() % SCREEN_WIDTH);
				laser.lPosY = SCREEN_HEIGHT;
				break;
			case 2:
				laser.lPosX = SCREEN_WIDTH - PLAYER_SIZE;
				laser.lPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);
				break;
			case 3:
				laser.lPosX = 0;
				laser.lPosY = static_cast<float>(std::rand() % SCREEN_HEIGHT);
				break;
			}
			float dx = player.getX() + PLAYER_SIZE / 2.0f - laser.lPosX;
			float dy = player.getY() + PLAYER_SIZE / 2.0f - laser.lPosY;
			float distance = std::sqrt(dx * dx + dy * dy);
			laser.lVelX = (dx / distance) * SCREEN_WIDTH;
			laser.lVelY = (dy / distance) * SCREEN_HEIGHT;
			laser.spawnTime = currentTime;
			laser.isActive = false;
			laser_.push_back(laser);
			lastLaserTime = currentTime;
			std::cout << "Laser spwaned\n";
		}
	}
}

void Lasers::reset(Uint32 currentTime) {
	laser_.clear();
	laserPerSpawn = 1;
	lastIncreaseTime = currentTime + laserStartDelay;
}
void Lasers::update(Uint32 currentTime) {
	for (auto& laser : laser_) {
		if (!laser.isActive && currentTime - laser.spawnTime >= laserWarningTime) {
			laser.isActive = true;
		}
	}
	laser_.erase(
		std::remove_if(laser_.begin(), laser_.end(),
			[currentTime, this](const LaserIndex& l) {
				// Kiểm tra nếu laser đã tồn tại lâu hơn thời gian cảnh báo + thời gian hoạt động
				return l.isActive && (currentTime - l.spawnTime >= (laserWarningTime + laserActiveTime));
			}
		),
		laser_.end()
	);

	if ((currentTime > lastIncreaseTime) && (currentTime - lastIncreaseTime >= increaseInterval)) {
		std::cout << " curentTime = " << currentTime << std::endl;
		std::cout << "LastIncreasseTime : " << lastIncreaseTime << std::endl;
		std::cout << currentTime - lastIncreaseTime << std::endl;
		if (laserPerSpawn <= maxLasersPerSpawn) laserPerSpawn++;
		lastIncreaseTime = currentTime;
		std::cout << "lasePerSpawn = " << laserPerSpawn << std::endl;
	}
}

void Lasers::render(SDL_Renderer* gRenderer) {
	for (auto& laser : laser_) {
		if (laser.isActive) {
			if (soundOn) {
				Mix_PlayChannel(-1, musicLaser, 0);
			}
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255); // Màu vàng
		}
		else {
			SDL_SetRenderDrawColor(gRenderer, 255, 165, 0, 255); // Màu cam cảnh báo
		}

		// Hướng tia laser
		float dx = laser.lVelX;
		float dy = laser.lVelY;
		float length = std::sqrt(dx * dx + dy * dy);

		// Kéo dài tia laser xuyên màn hình
		float scaleFactor = std::max(SCREEN_WIDTH, SCREEN_HEIGHT) * 2 / length;
		float endX = laser.lPosX + dx * scaleFactor;
		float endY = laser.lPosY + dy * scaleFactor;

		// Tính độ dày tia laser
		float offsetX = (dy / length) * (PLAYER_SIZE / 2.0f);
		float offsetY = -(dx / length) * (PLAYER_SIZE / 2.0f);

		for (int i = -PLAYER_SIZE / 2; i <= PLAYER_SIZE / 2; i++) {
			int startX = static_cast<int>(laser.lPosX + offsetX * (i / (PLAYER_SIZE / 2.0f)));
			int startY = static_cast<int>(laser.lPosY + offsetY * (i / (PLAYER_SIZE / 2.0f)));
			int endX2 = static_cast<int>(endX + offsetX * (i / (PLAYER_SIZE / 2.0f)));
			int endY2 = static_cast<int>(endY + offsetY * (i / (PLAYER_SIZE / 2.0f)));

			SDL_RenderDrawLine(gRenderer, startX, startY, endX2, endY2);
		}
	}
}


//void Lasers::render(SDL_Renderer* gRenderer) {
//	for (auto& laser : laser_) {
//		if (laser.isActive){
//			SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
//		}
//		else {
//			SDL_SetRenderDrawColor(gRenderer, 255, 165, 0, 255);
//		}
//		float endX = laser.lPosX + laser.lVelX ;
//		float endY = laser.lPosY + laser.lVelY ;
//		float dx = endX - laser.lPosX;
//		float dy = endY - laser.lPosY;
//		float length = std::sqrt(dx * dx +  dy * dy);
//		float offsetX = (dy / length) * (PLAYER_SIZE / 2.0f);
//		float offsetY = -(dx / length) * (PLAYER_SIZE / 2.0f);
//		for (int i = -PLAYER_SIZE / 2; i <= PLAYER_SIZE / 2; i ++) {
//			int startX = static_cast<int>(laser.lPosX + PLAYER_SIZE / 2 + offsetX * (i / (PLAYER_SIZE / 2.0f)));
//			int startY = static_cast<int>(laser.lPosY + PLAYER_SIZE / 2 + offsetY * (i / (PLAYER_SIZE / 2.0f)));
//			int endX2 = static_cast<int>(endX + PLAYER_SIZE / 2 + offsetX * (i / (PLAYER_SIZE / 2.0f)));
//			int endY2 = static_cast<int>(endY + PLAYER_SIZE / 2 + offsetY * (i / (PLAYER_SIZE / 2.0f)));
//			SDL_RenderDrawLine(gRenderer, startX, startY, endX2, endY2);
//		}
//	}
//}

