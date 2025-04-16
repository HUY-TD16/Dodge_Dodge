#pragma once
#include "global.h"
#include <iostream>

SDL_Renderer* gRenderer = nullptr;
SDL_Window* gWindow = nullptr;
TTF_Font* gFont = NULL;
Mix_Music* gMusicMenu;
Mix_Music* gMusicPlay = NULL;
Mix_Chunk* musicFireball = NULL;
Mix_Chunk* musicLaser = NULL;
Mix_Chunk* musicExplosion = NULL;
Mix_Chunk* musicClear = NULL;
Mix_Chunk* musicDash = NULL;
Mix_Chunk* musicInvinbility = NULL;
bool soundOn = true;

void logErrorAndExit(const char* msg, const char* error) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
	SDL_Quit();
}
bool init() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		logErrorAndExit("SDL_Init", SDL_GetError());
		return false;
	}
	gWindow = SDL_CreateWindow("Only One", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!gWindow) {
		logErrorAndExit("SDL_CreateWindow", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!gRenderer) {
		logErrorAndExit("Create REnder", SDL_GetError());
		return false;
	}
	//gFont = TTF_OpenFont("Font/lazy.ttf", 28);
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	gFont = TTF_OpenFont("Font/lazy.ttf", 28);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	int imgFlag = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlag) & imgFlag)) {
		logErrorAndExit("imgFlag", SDL_GetError());
		return false;
	}
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) == 0) {
		logErrorAndExit("SDL_mixer Init", Mix_GetError());
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2080) < 0) {
		logErrorAndExit("SDL mixer", Mix_GetError());
		return false;
	}
	return true;
}
void close() {
	Mix_FreeChunk(musicFireball);
	Mix_FreeChunk(musicExplosion);
	Mix_FreeChunk(musicLaser);
	Mix_FreeChunk(musicDash);
	Mix_FreeChunk(musicInvinbility);
	Mix_FreeChunk(musicClear);
	musicClear = NULL;
	musicDash = NULL;
	musicInvinbility = NULL;
	musicFireball = NULL;
	musicExplosion = NULL;
	musicLaser = NULL;

	Mix_FreeMusic(gMusicMenu);
	Mix_FreeMusic(gMusicPlay);
	gMusicPlay = NULL;
	gMusicMenu = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	TTF_CloseFont(gFont);
	gFont = NULL;
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
bool isButtonClick(int x, int y, SDL_Rect button) {
	return (x >= button.x && x <= button.x + button.w && y >= button.y && y <= button.y + button.h);
}

//void loadMedia(Animation& animation) {
//	animation.loadSpritesheet(gRenderer, "Image/run_right.png", Action::RUN, Direction::RIGHT, 8);
//	animation.loadSpritesheet(gRenderer, "Image/run_left.png", Action::RUN, Direction::LEFT, 8) ;
//	animation.loadSpritesheet(gRenderer, "Image/run_front.png", Action::RUN, Direction::FRONT, 8) ;
//	animation.loadSpritesheet(gRenderer, "Image/run_back.png", Action::RUN, Direction::BACK, 8) ;
//	animation.loadSpritesheet(gRenderer, "Image/idle_right.png", Action::IDLE, Direction::RIGHT, 4) ;
//	animation.loadSpritesheet(gRenderer, "Image/idle_left.png", Action::IDLE, Direction::LEFT, 4) ;
//	animation.loadSpritesheet(gRenderer, "Image/idle_front.png", Action::IDLE, Direction::FRONT, 4) ;
//	animation.loadSpritesheet(gRenderer, "Image/idle_back.png", Action::IDLE, Direction::BACK, 4);
//}

bool loadMedia() {
	//printf("DEBUG: Đã build lại!\n");
	bool success = true;
	gMusicMenu = Mix_LoadMUS("sound/menu.mp3");
	if (gMusicMenu == NULL) {
		logErrorAndExit("Failed MusicMenu", Mix_GetError());
		success = false;
	}
	gMusicPlay = Mix_LoadMUS("sound/play.mp3");
	if (gMusicPlay == NULL) {
		logErrorAndExit("Failed MusicPlay", Mix_GetError());
		success = false;
		//std::cerr << "jdisdjisdj\n";
	}
	/*musicFireball = Mix_LoadWAV("soundtest/scratch.wav");
	if (musicFireball == NULL) {
		logErrorAndExit("Failed musicFireball", Mix_GetError());
		success = false;
	}*/
	musicExplosion = Mix_LoadWAV("sound/explosion.wav");
	if (musicExplosion == NULL) {
		logErrorAndExit("Failed musicExplosion", Mix_GetError());
		success = false;
	}
	musicLaser = Mix_LoadWAV("sound/laser.mp3");
	if (musicLaser == NULL) {
		logErrorAndExit("Failed musicLaser", Mix_GetError());
		success = false;
	}
	musicDash = Mix_LoadWAV("sound/dash.mp3");
	if (musicDash == NULL) {
		logErrorAndExit("Failed musicDash", Mix_GetError());
		success = false;
	}
	musicInvinbility = Mix_LoadWAV("sound/invincibility.mp3");
	if (musicInvinbility == NULL) {
		logErrorAndExit("Failed musicInvinbility", Mix_GetError());
		success = false;
	}
	musicClear = Mix_LoadWAV("sound/clear.mp3");
	if (musicClear == NULL) {
		logErrorAndExit("Failed MusicMenu", Mix_GetError());
		success = false;
	}
	Mix_VolumeChunk(musicLaser, 24);
	Mix_VolumeChunk(musicExplosion, 40);
	Mix_VolumeMusic(40);
	return success;

}