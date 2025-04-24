#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "player.h"
#include "skill.h"
#include "global.h"
#include "fireball.h"
#include "lasers.h"
#include "explosions.h"
#include "collision.h"
#include "interface.h"
#include "animation.h"
enum class State {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();
    void update(float deltaTime, Uint32 currentTime);
    void render(Uint32 currentTime);
    void handleInput(SDL_Event& event, Uint32 currentTime);
    void resetGame(Uint32 currentTime);
    void loadHighScores();
    void saveHighScores();
    void loadMedia();
    Uint32 getLastFramTime() { return lastFrameTime; }
    State getState() { return state; }
private:
    Player player;
    Animation animation;
    Skills skill;
    FireBall fireball;
    Lasers laser;
    Explosion explosion;
    Collision collisision;
    Interface interface;
    std::vector<ClearEffect> clearEffect;
    //Time
    Uint32 gameStartTime;
    Uint32 lastSpawnTime;
    Uint32 lastFrameTime;
    Uint32 lastLaserTime;
    Uint32 lastExplosionTime;
    Uint32 finalTime;
    State state;
    std::vector<int> highScore;
    bool isNewHighScore;
    bool showTutorial;
    bool showHighScore;


};

