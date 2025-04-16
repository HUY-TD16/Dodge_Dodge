#include "animation.h"
#include <SDL_image.h>
#include <iostream>
#include "global.h"

extern SDL_Renderer* gRenderer;

Animation::Animation() : currentAction(Action::IDLE),
currentDirection(Direction::FRONT),
currentFrame(0),
lastUpdateTime(0),
frameDelay(100) {
}
Animation::~Animation() {
	for (auto& x : animationFrames) {
		SDL_DestroyTexture(x.second.gTexture);
	}
}
bool Animation::loadSpritesheet(SDL_Renderer* renderer, const std::string path, Action action, Direction direction, int frameCount) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cerr << "Unable to load image " << path << ". SDL_image Error: " << IMG_GetError() << '\n';
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture) {
		std::cerr << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << '\n';
		return false;
	}

	int textureWidth, textureHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	frameWidth = textureWidth / frameCount;
	frameHeight = textureHeight;

	animationFrames[{action, direction}] = { texture, frameCount };

	return true;
}
void Animation::update() {
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > lastUpdateTime + frameDelay) {
		currentFrame = (currentFrame + 1) % animationFrames[{currentAction, currentDirection}].frameCount;
		lastUpdateTime = currentTime;
	}

}
void Animation::render(SDL_Renderer* gRenderer, int x, int y) {
	SDL_Rect srcRect = { currentFrame * frameWidth , 0 , frameWidth , frameHeight };
	SDL_Rect destRect = { x , y , frameWidth * 1.5 , frameHeight * 1.5 };
	SDL_Texture* texture = animationFrames[{currentAction, currentDirection}].gTexture;
	SDL_RenderCopy(gRenderer, texture, &srcRect, &destRect);
	//std::cout << x <<  " " << y << std::endl;

}
void Animation::setAction(Action action)
{
	if (currentAction != action) {
		currentAction = action;
		currentFrame = 0;
		lastUpdateTime = SDL_GetTicks();
	}
}

void Animation::setDirection(Direction direction) {
	if (currentDirection != direction) {
		currentDirection = direction;
		currentFrame = 0;
		lastUpdateTime = SDL_GetTicks();
	}
}
//void Animation::handleInput(SDL_Event& e) {
//	if (e.type == SDL_KEYDOWN  && e.key.repeat == 0) {
//		switch (e.key.keysym.sym) {
//		case SDLK_LEFT: setAction(Action::RUN); setDirection(Direction::LEFT); break;
//		case SDLK_RIGHT: setAction(Action::RUN); setDirection(Direction::RIGHT); break;
//		case SDLK_UP: setAction(Action::RUN); setDirection(Direction::BACK); break;
//		case SDLK_DOWN: setAction(Action::RUN); setDirection(Direction::FRONT); break;
//		}
//	}
//	else if (e.type == SDL_KEYUP && e.key.repeat ==0 ) {
//		switch (e.key.keysym.sym) {
//		case SDLK_LEFT: setAction(Action::IDLE); setDirection(Direction::LEFT); break;
//		case SDLK_RIGHT: setAction(Action::IDLE); setDirection(Direction::RIGHT); break;
//		case SDLK_UP: setAction(Action::IDLE); setDirection(Direction::BACK); break;
//		case SDLK_DOWN: setAction(Action::IDLE); setDirection(Direction::FRONT); break;
//		}
//	}
//}
void Animation::handleInput(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		keyState.insert(e.key.keysym.sym); // Lưu phím đang được nhấn
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		keyState.erase(e.key.keysym.sym); // Xóa phím khi người dùng nhả ra
	}
	updateAnimationState(); // Cập nhật animation dựa vào phím đang giữ
}

void Animation::updateAnimationState() {
	if (keyState.empty()) {
		setAction(Action::IDLE);
		return;
	}

	if (keyState.count(SDLK_LEFT)) { setAction(Action::RUN); setDirection(Direction::LEFT); }
	else if (keyState.count(SDLK_RIGHT)) { setAction(Action::RUN); setDirection(Direction::RIGHT); }
	else if (keyState.count(SDLK_UP)) { setAction(Action::RUN); setDirection(Direction::BACK); }
	else if (keyState.count(SDLK_DOWN)) { setAction(Action::RUN); setDirection(Direction::FRONT); }
}

void Animation::reset() {
	currentAction = Action::IDLE;
	currentDirection = Direction::FRONT;
	currentFrame = 0;
	lastUpdateTime = SDL_GetTicks();
	keyState.clear();
}