#pragma once

#include <SDL_image.h>
#include <SDL.h>
#include "LTexture.h"
#include <iostream>
#include <string>
#include <vector>

extern SDL_Renderer* gRenderer;

class Interface {
public:
	Interface();
	void GameOver(Uint32 finalTime, bool isNewHighScore);
	void Pause();
	void PlayDisplay();
	void Menu(bool showTutorial, bool showHighScore, bool soundOn, std::vector<int> highScores);
	void Timer(Uint32 currentTime);
	void ProgressBars(Uint32 currentTime, Uint32 lastInvincibilityTime, Uint32 lastDashTime, Uint32 lastClearTime);
	// Menu button
	SDL_Rect StartTextRect = { SCREEN_WIDTH / 2 - 80 , 100 , 160 , 100 };
	SDL_Rect TutorialTextRect = { SCREEN_WIDTH / 2 - 100, 200 , 200 , 100 };
	SDL_Rect HighscoreTextRect = { SCREEN_WIDTH / 2 - 150 , 300 , 300 , 100 };
	SDL_Rect MusicTextRect = { SCREEN_WIDTH / 2 - 100 , 400 , 200 , 100 };

	// Playing button
	SDL_Rect PauseRect = { 10 , 10 , 20 , 20 };
	// Pause button
	SDL_Rect continuteRect = { SCREEN_WIDTH / 2 - 80 , SCREEN_HEIGHT / 2 - 100 , 160 , 100 };
	SDL_Rect resumeRect = { SCREEN_WIDTH / 2 - 60 , SCREEN_HEIGHT / 2  , 120 , 100 };
	SDL_Rect backmenuRect = { SCREEN_WIDTH / 2 - 70 , SCREEN_HEIGHT / 2 + 100 , 140 , 100 };
	// GameOver button
	SDL_Rect replayRect = { SCREEN_WIDTH / 2 - 150 , SCREEN_HEIGHT / 2 + 150 , 100 , 100 };
	SDL_Rect menuRect = { SCREEN_WIDTH / 2 + 50 , SCREEN_HEIGHT / 2 + 150 , 100 , 100 };
private:
	LTexture RecordText;
	int tic, second, minutes;
	std::string recordText;
	LTexture BackgroundPlay;
	LTexture MenuDisplay;
	LTexture gText;
	LTexture PauseDisplay;
	LTexture SkillDisplay;

};