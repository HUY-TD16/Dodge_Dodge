#include "game.h"
#include <iostream>
#include <algorithm>
#include <fstream>

Game::Game() {
    gameStartTime = SDL_GetTicks();
    lastSpawnTime = SDL_GetTicks();
    lastFrameTime = SDL_GetTicks();
    lastLaserTime = SDL_GetTicks();
    lastExplosionTime = SDL_GetTicks();
    finalTime = 0;
    state = State::MENU;
    isNewHighScore = false;
    showTutorial = false;
    showHighScore = false;
    soundOn = true;
    loadHighScores();
    skill.loadMediaSkill();
}

Game::~Game() {
    saveHighScores();
    close();
}

void Game::handleInput(SDL_Event& e, Uint32 currentTime) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        switch (state) {
        case State::MENU:
            if (isButtonClick(mouseX, mouseY, interface.StartTextRect)) {
                state = State::PLAYING;
                Mix_HaltMusic();
                gameStartTime = SDL_GetTicks();
                showTutorial = false;
                showHighScore = false;
                resetGame(currentTime);
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";

            }
            else if (isButtonClick(mouseX, mouseY, interface.HighscoreTextRect)) {
                showTutorial = false;
                showHighScore = !showHighScore;
                std::cout << "Switched to state: HighScore" << "\n";


            }
            else if (isButtonClick(mouseX, mouseY, interface.TutorialTextRect)) {
                showTutorial = !showTutorial;
                showHighScore = false;
                std::cout << "Switched to state: Tutorial \n";

            }
            else if (isButtonClick(mouseX, mouseY, interface.MusicTextRect)) {
                soundOn = !soundOn;
                std::cout << "Switched to state: Music\n";
            }
            else if (showTutorial || showHighScore) {
                showHighScore = false;
                showTutorial = false;
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";

            }
            break;

        case State::PLAYING:
            //std::cerr << "húhifiuhfd\n";
            player.handleEvent(e);
            animation.handleInput(e);
            skill.handleInput(SDL_GetKeyboardState(NULL), SDL_GetTicks(), e, player, clearEffect, fireball.getFireball());
            if (isButtonClick(mouseX, mouseY, interface.PauseRect)) {
                state = State::PAUSED;
                Mix_HaltMusic();
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";

            }
            break;
        case State::PAUSED:
            if (isButtonClick(mouseX, mouseY, interface.backmenuRect)) {
                state = State::MENU;
                Mix_HaltMusic();
                resetGame(currentTime);
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";
            }
            else if (isButtonClick(mouseX, mouseY, interface.resumeRect)) {
                state = State::PLAYING;
                Mix_HaltMusic();
                resetGame(currentTime);
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";
            }
            else if (isButtonClick(mouseX, mouseY, interface.continuteRect)) {
                state = State::PLAYING;
                Mix_HaltMusic();
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";

            }
            break;
        case State::GAME_OVER:
            if (isButtonClick(mouseX, mouseY, interface.menuRect)) {
                state = State::MENU;
                Mix_HaltMusic();
                isNewHighScore = false;
                resetGame(currentTime);
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";
            }
            else if (isButtonClick(mouseX, mouseY, interface.replayRect)) {
                state = State::PLAYING;
                Mix_HaltMusic();
                gameStartTime = SDL_GetTicks();
                isNewHighScore = false;
                resetGame(currentTime);
                std::cout << "Switched to state: " << static_cast<int>(state) << "\n";

            }
            break;
        }

    }
    if (state == State::PLAYING) {
        //std::cerr << "húhifiuhfd\n";
        player.handleEvent(e);
        animation.handleInput(e);
        skill.handleInput(SDL_GetKeyboardState(NULL), SDL_GetTicks(), e, player, clearEffect, fireball.getFireball());
    }
}

void Game::update(float deltaTime, Uint32 currentTime) {
    /*SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);*/
    player.move();
    animation.update();
    skill.update(currentTime/* , clearEffect , fireball.getFireball()*/);
    fireball.spawn(currentTime, player, lastSpawnTime);
    fireball.update(deltaTime, currentTime);
    laser.spawn(currentTime, player, lastLaserTime, gameStartTime);
    laser.update(currentTime);
    //std::cerr << currentTime << " " << gameStartTime << std::endl;
    explosion.spawn(currentTime, lastExplosionTime, gameStartTime);
    explosion.update(currentTime);
    clearEffect.erase(
        std::remove_if(clearEffect.begin(), clearEffect.end(), [currentTime](const ClearEffect& e) {
            return currentTime - e.SpawnTime >= 1000;
            }),
        clearEffect.end()
    );
    /*if (collisision.checkCollision(player, skill.res(),
        fireball.getFireball(), fireball,
        laser.getLaser(), laser,
        explosion.getExplosion(), explosion)) {

        state = State::GAME_OVER;
        finalTime = currentTime - gameStartTime;
        highScore.push_back(finalTime);
        std::sort(highScore.begin(), highScore.end(), std::greater<int>());
        isNewHighScore = (highScore.size() == 1 || finalTime > highScore[1]);
        if (highScore.size() > 3) highScore.resize(3);
    }*/
    lastFrameTime = currentTime;

}

void Game::loadHighScores() {
    std::ifstream file("highScore.txt");
    if (file.is_open()) {
        int score;
        while (file >> score) {
            highScore.push_back(score);
        }
        file.close();
        std::sort(highScore.begin(), highScore.end(), std::greater<int>());
        if (highScore.size() > 3) highScore.resize(3);
    }
}

void Game::saveHighScores() {
    std::ofstream file("highScore.txt");
    if (file.is_open()) {
        for (int score : highScore) {
            file << score << "\n";
        }
        file.close();
    }
}

void Game::resetGame(Uint32 currentTime) {
    player = Player();
    animation.reset();
    skill.reset(currentTime);
    fireball.reset(currentTime);
    laser.reset(currentTime);
    explosion.reset(currentTime);
    clearEffect.clear();
    gameStartTime = currentTime;
    lastSpawnTime = currentTime;
    lastFrameTime = currentTime;
    lastLaserTime = currentTime;
    lastExplosionTime = currentTime;
    finalTime = 0;
}

void Game::render(Uint32 currentTime) {
    //std::cerr << "kkk\n";
    switch (state) {
    case State::MENU:
        interface.Menu(showTutorial, showHighScore, soundOn, highScore);
        if (soundOn) {
            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(gMusicMenu, -1);
            }
        }
        else {
            Mix_HaltMusic();
        }
        //std::cerr << "menu\n";
        break;
    case State::PLAYING:
        //Mix_HaltMusic();
        if (soundOn) {
            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(gMusicPlay, -1);
            }
            /*else {
                if (Mix_PausedMusic() == 1) {
                    Mix_ResumeMusic();
                }
                else {
                    Mix_PauseMusic();
                }
            }*/
        }
        else {
            Mix_HaltMusic();
        }
        // std::cout << "Playein\n";
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        interface.PlayDisplay();
        animation.render(gRenderer, player.getX() - 40 * 1.5, player.getY() - 32 * 1.5);
        fireball.render(gRenderer); // game time 
        laser.render(gRenderer); // game time
        explosion.render(gRenderer); // game time 
        interface.ProgressBars(currentTime, skill.getLastInvicibility(), skill.getLastDashTime(), skill.getLastClear()); // game time 
        interface.Timer(currentTime - gameStartTime); // game time
        skill.render(player.getX() - PLAYER_SIZE / 2, player.getY() - PLAYER_SIZE / 2);
        if (collisision.checkCollision(player, skill.res(),
            fireball.getFireball(), fireball,
            laser.getLaser(), laser,
            explosion.getExplosion(), explosion)) {

            state = State::GAME_OVER;
            Mix_HaltMusic();
            finalTime = currentTime - gameStartTime;
            highScore.push_back(finalTime);
            std::sort(highScore.begin(), highScore.end(), std::greater<int>());
            isNewHighScore = (highScore.size() == 1 || finalTime > highScore[1]);
            if (highScore.size() > 3) highScore.resize(3);

        }
        break;
    case State::GAME_OVER:
        //std::cerr << "gameover\n";
        interface.GameOver(finalTime, isNewHighScore);
        break;
    case State::PAUSED:
        //std::cerr << "pause\n";
        interface.Pause();
        break;
    }

}

void Game::loadMedia() {
    animation.loadSpritesheet(gRenderer, "Image/run_right.png", Action::RUN, Direction::RIGHT, 8);
    animation.loadSpritesheet(gRenderer, "Image/run_left.png", Action::RUN, Direction::LEFT, 8);
    animation.loadSpritesheet(gRenderer, "Image/run_front.png", Action::RUN, Direction::FRONT, 8);
    animation.loadSpritesheet(gRenderer, "Image/run_back.png", Action::RUN, Direction::BACK, 8);
    animation.loadSpritesheet(gRenderer, "Image/idle_right.png", Action::IDLE, Direction::RIGHT, 4);
    animation.loadSpritesheet(gRenderer, "Image/idle_left.png", Action::IDLE, Direction::LEFT, 4);
    animation.loadSpritesheet(gRenderer, "Image/idle_front.png", Action::IDLE, Direction::FRONT, 4);
    animation.loadSpritesheet(gRenderer, "Image/idle_back.png", Action::IDLE, Direction::BACK, 4);
}