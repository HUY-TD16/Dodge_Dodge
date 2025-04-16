#include "interface.h"
#include <iostream>
#include <vector>

Interface::Interface() {
	MenuDisplay.loadFromFile("Image/backgroundMenu.png");
	PauseDisplay.loadFromFile("Image/pause.png");
	BackgroundPlay.loadFromFile("Image/backgroundPlay.png");
	std::cerr << "load file\n";
}

void Interface::Timer(Uint32 currentTime) {
	tic = currentTime % 1000 / 10;
	second = currentTime / 1000 % 60;
	minutes = currentTime / 60000;
	recordText = std::to_string(minutes) + ":" + std::to_string(second) + ":" + std::to_string(tic);
	SDL_Color textColor = { 0 , 0 , 0 };
	RecordText.loadFromRenderText(recordText, textColor);
	RecordText.render(SCREEN_WIDTH - 100, 20);
}

void Interface::Menu(bool showTutorial, bool showHighScore, bool soundOn, std::vector<int> highScores) {
	SDL_Rect BackgroundRect = { 0,0, 800 , 600 };
	MenuDisplay.render(0, 0, NULL, &BackgroundRect);
	SDL_Color textColor = { 128,255, 255, 255 };
	gText.loadFromRenderText("START", textColor);
	gText.render(SCREEN_WIDTH / 2 - 50, 100, NULL, &StartTextRect);
	//SDL_RenderDrawRect(gRenderer, &StartTextRect);
	gText.loadFromRenderText("TuTorial", textColor);
	gText.render(SCREEN_WIDTH / 2 - 100, 200, NULL, &TutorialTextRect);
	gText.loadFromRenderText("HIgh ScoRe", textColor);
	gText.render(SCREEN_WIDTH / 2 - 150, 300, NULL, &HighscoreTextRect);
	if (soundOn) {
		gText.loadFromRenderText("Music : ON", textColor);
		gText.render(SCREEN_WIDTH / 2 - 60, 400, NULL, &MusicTextRect);
	}
	else {
		gText.loadFromRenderText("Music : OFF", textColor);
		gText.render(SCREEN_WIDTH / 2 - 60, 400, NULL, &MusicTextRect);
	}

	if (showTutorial) {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 200);
		SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderFillRect(gRenderer, &overlay);
		SDL_Color textColortmp = { 0 , 0 , 0 };

		gText.loadFromRenderText("Instructions:", textColortmp);
		gText.render(10, 10);
		gText.loadFromRenderText("Arrow Keys : Move", textColortmp);
		gText.render(10, 50);
		gText.loadFromRenderText("A : Invincibility cD 10s", textColortmp);
		gText.render(10, 100);
		gText.loadFromRenderText("S : Dash cD 5s", textColortmp);
		gText.render(10, 200);
		gText.loadFromRenderText("D : Clear Fireball cD 10s", textColortmp);
		gText.render(10, 300);
		gText.loadFromRenderText("Avoid obstacles, lasers, and explosions!", textColortmp);
		gText.render(10, 400);
		gText.loadFromRenderText("Click anywhere to close", textColortmp);
		gText.render(10, 500);

	}
	if (showHighScore) {
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 200);
		SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderFillRect(gRenderer, &overlay);

		SDL_Color textColortmp = { 0, 0, 0, 255 };

		gText.loadFromRenderText("High Scores:", textColortmp);
		gText.render(SCREEN_WIDTH / 2 - gText.getWidth() / 2, 100);

		for (size_t i = 0; i < highScores.size() && i < 3; ++i) {
			int tic = highScores[i] % 1000 / 10;
			int second = highScores[i] / 1000 % 60;
			int minutes = highScores[i] / 60000;
			std::string scoreText = std::to_string(i + 1) + ". " + std::to_string(minutes) + ":" + std::to_string(second) + ":" + std::to_string(tic);
			gText.loadFromRenderText(scoreText, textColortmp);
			gText.render(SCREEN_WIDTH / 2 - gText.getWidth() / 2, 130 + i * 30);
		}

		gText.loadFromRenderText("Click anywhere to close", textColortmp);
		gText.render(SCREEN_WIDTH / 2 - gText.getWidth() / 2, 220 + 3 * 30);
	}

}

void Interface::ProgressBars(Uint32 currentTime, Uint32 lastInvincibilityTime, Uint32 lastDashTime, Uint32 lastClearTime) {
	const int boxSize = 40;
	const int borderWidth = 4;
	int xPos = 10;
	const int yPos = SCREEN_HEIGHT - 50;
	const int spacing = 10;
	struct SkillTime {
		Uint32 lastTime;
		Uint32 coolDown;
		std::string key;
		SDL_Color color;
	};
	std::vector<SkillTime> skillt = {
		{lastInvincibilityTime , 10000 , "A" , {0 , 255 , 255 , 255}},
		{lastDashTime , 5000 , "S" ,{255 , 255 , 0 , 255}},
		{lastClearTime , 10000, "D" , {255 , 0 , 255 , 255}}
	};
	for (auto& skill : skillt) {
		float progress;
		if (currentTime < skill.lastTime + skill.coolDown) {
			progress = static_cast<float>(currentTime - skill.lastTime) / skill.coolDown;
		}
		else {
			progress = 1.0f;
		}

		if (progress >= 1.0f) {
			SDL_SetRenderDrawColor(gRenderer, skill.color.r, skill.color.g, skill.color.b, 255);
		}
		else {
			SDL_SetRenderDrawColor(gRenderer, skill.color.r / 2, skill.color.g / 2, skill.color.b / 2, 195);
		}
		SDL_Rect box = { xPos , yPos , boxSize , boxSize };
		SDL_RenderDrawRect(gRenderer, &box);

		if (progress < 1.0f) {
			int borderHeight = static_cast<int>(boxSize * (1.0f - progress));
			SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 195);
			SDL_Rect border = { xPos , yPos, boxSize  , borderHeight };
			SDL_RenderFillRect(gRenderer, &border);
		}
		SDL_Color textColor = { skill.color.r, skill.color.g, skill.color.b, 255 };
		SkillDisplay.loadFromRenderText(skill.key, textColor);
		SkillDisplay.render(xPos, yPos, NULL, &box);
		xPos += boxSize + spacing;
	}
}

void Interface::GameOver(Uint32 finalTime, bool isNewHighScore) {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 10);
	SDL_Rect overlay = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &overlay);

	SDL_Color textColor = { 255 , 255 , 255 , 255 };
	tic = finalTime % 1000 / 10;
	second = finalTime / 1000 % 60;
	minutes = finalTime / 60000;
	std::string scoreText = "Score: " + std::to_string(minutes) + ":" + std::to_string(second) + ":" + std::to_string(tic);
	gText.loadFromRenderText(scoreText, textColor);
	SDL_Rect scoreRect = { SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 50 , 240 , 100 };
	gText.render(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 50, NULL, &scoreRect);
	if (isNewHighScore) {
		SDL_Rect newscoreRect = { SCREEN_WIDTH / 2 - 130 , SCREEN_HEIGHT / 2 + 50  , 260 , 100 };
		gText.loadFromRenderText("NEw ScORe!!!", textColor);
		gText.render(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 + 50, NULL, &newscoreRect);
	}

	//SDL_RenderDrawRect(gRenderer, &replayRect);
	gText.loadFromRenderText("Replay", textColor);
	gText.render(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 150, NULL, &replayRect);

	//SDL_RenderDrawRect(gRenderer, &menuRect);
	gText.loadFromRenderText("Menu", textColor);
	gText.render(SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 + 150, NULL, &menuRect);


}

void Interface::Pause() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 10);
	SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &overlay);

	SDL_Color textColor = { 255 , 255 , 255 , 255 };
	gText.loadFromRenderText("Continute", textColor);
	gText.render(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 100, NULL, &continuteRect);

	gText.loadFromRenderText("Resume", textColor);
	gText.render(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, NULL, &resumeRect);

	gText.loadFromRenderText("Back Menu", textColor);
	gText.render(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 + 100, NULL, &backmenuRect);
}

void Interface::PlayDisplay() {
	SDL_Rect bgPlay = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	BackgroundPlay.render(0, 0, NULL, &bgPlay);
	PauseDisplay.render(10, 10, NULL, &PauseRect);
}

//void Interface::Playing(Uint32 currenTime ,Player player,  Animation animation, FireBall fireball, Lasers laser, Explosion explosion) {
//	animation.render(gRenderer, player.getX() - 40 * 1.5, player.getY() - 32 * 1.5);
//	laser.render(gRenderer);
//	explosion.render(gRenderer);
//	fireball.render(gRenderer);
//}