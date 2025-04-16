#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "global.h"
#include "player.h"
#include <vector>
#include <string>
#include <map>
#include <set>

enum class Action {
	IDLE,
	RUN
};
enum class Direction {
	FRONT,
	BACK,
	RIGHT,
	LEFT
};
class Animation {
public:
	Animation();
	~Animation();

	bool loadSpritesheet(SDL_Renderer* gRenderer, const std::string path, Action action, Direction direction, int frameCount);
	void update();
	void render(SDL_Renderer* gRenderer, int x, int y);
	void setAction(Action action);
	void setDirection(Direction direction);
	void handleInput(SDL_Event& e);
	void updateAnimationState();
	void reset();
private:
	struct FrameInfo {
		SDL_Texture* gTexture;
		int frameCount;
	};
	std::map<std::pair<Action, Direction>, FrameInfo> animationFrames;
	Action currentAction;
	Direction currentDirection;

	int currentFrame;
	int frameWidth;
	int frameHeight;
	Uint32 lastUpdateTime;
	int frameDelay;

	std::set<SDL_Keycode> keyState;

};