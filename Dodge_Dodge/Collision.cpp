#include "collision.h"
#include <iostream>
extern SDL_Renderer* gRenderer;
Collision::Collision() {}
bool Collision::checkCollision(Player& player, bool isInvincible,
	const std::vector<FireballIndex>& fireball_, FireBall& f,
	const std::vector<LaserIndex>& laser_, Lasers& l,
	const std::vector<ExplosionIndex>& explosion_, Explosion& e
) {
	if (!isInvincible) {

		SDL_Rect playerRect = { player.getX() - 10 , player.getY() - 10 , PLAYER_SIZE , PLAYER_SIZE };
		/*SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderFillRect(gRenderer, &playerRect);*/
		for (const auto& fireball : fireball_) {
			SDL_Rect fireballRect = {
				static_cast<int>(fireball.fPosX + FIREBALL_SIZE),
				static_cast<int>(fireball.fPosY + FIREBALL_SIZE),
				FIREBALL_SIZE,
				FIREBALL_SIZE
			};
			if (SDL_HasIntersection(&playerRect, &fireballRect)) {
				std::cout << "Collision with fireball - GameOver\n";
				return true;
			}
			/*SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
			SDL_RenderFillRect(gRenderer, &fireballRect);*/

		}

		for (const auto& laser : laser_) {
			if (laser.isActive) {
				// Xác định hướng của laser
				float dx = laser.lVelX;
				float dy = laser.lVelY;
				float length = std::sqrt(dx * dx + dy * dy);

				// Đảm bảo laser kéo dài xuyên màn hình
				float scaleFactor = std::max(SCREEN_WIDTH, SCREEN_HEIGHT) * 2 / length;
				float endX = laser.lPosX + dx * scaleFactor;
				float endY = laser.lPosY + dy * scaleFactor;

				// Tính toán độ dày của laser
				float offsetX = (dy / length) * (PLAYER_SIZE / 2.0f);
				float offsetY = -(dx / length) * (PLAYER_SIZE / 2.0f);

				// Kiểm tra va chạm theo từng đoạn nhỏ của laser với độ dày đầy đủ
				for (float t = 0; t <= 1; t += 0.005f) { // Kiểm tra dày hơn để không bị sót
					for (int i = -PLAYER_SIZE / 2; i <= PLAYER_SIZE / 2; i++) {
						int x = static_cast<int>(laser.lPosX + t * (endX - laser.lPosX) + offsetX * (i / (PLAYER_SIZE / 2.0f)));
						int y = static_cast<int>(laser.lPosY + t * (endY - laser.lPosY) + offsetY * (i / (PLAYER_SIZE / 2.0f)));

						SDL_Rect laserSegment = { x, y, 2, 2 }; // Kiểm tra từng điểm nhỏ thay vì hình vuông lớn

						if (SDL_HasIntersection(&playerRect, &laserSegment)) {
							std::cout << "Collision with laser - GameOver\n";
							return true;
						}
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
						SDL_RenderFillRect(gRenderer, &laserSegment);
					}
				}
			}
		}
		for (const auto& explosion : explosion_) {
			if (explosion.isActive) {
				SDL_Rect explosionRect = { static_cast<int>(std::round(explosion.ePosX)) , static_cast<int>(std::round(explosion.ePosY)) , PLAYER_SIZE * 10 , PLAYER_SIZE * 10 };
				if (SDL_HasIntersection(&playerRect, &explosionRect)) {
					std::cout << "Collision with explosion - Game Over\n";
					return true;
				}
			}
		}
	}
	/*else {
		SDL_Rect AreaInvincibility = { player.getX() - 10 , player.getY() - 10 , PLAYER_SIZE * 2 , PLAYER_SIZE * 2 };
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 200, 150);
		SDL_RenderFillRect(gRenderer, &AreaInvincibility);
	}*/
	return false;
}