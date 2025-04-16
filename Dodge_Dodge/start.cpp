#include <SDL.h>
#include "game.h"
#include "global.h"

int main(int args, char* argc[]) {
	std::srand(static_cast<unsigned>(std::time(NULL)));
	if (!init()) {
		std::cout << "false Init \n";
	}
	else if (!loadMedia()) {
		std::cerr << "false mediaload \n";
	}
	else {
		Game game;
		bool quit = false;
		SDL_Event e;
		game.loadMedia();
		while (!quit) {
			Uint32 currentTime = SDL_GetTicks();
			float deltaTime = (currentTime - game.getLastFramTime()) / 1000.0f;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				game.handleInput(e, currentTime);
			}
			if (game.getState() == State::PLAYING) {
				game.update(deltaTime, currentTime);
			}
			game.render(currentTime);
			SDL_RenderPresent(gRenderer);
			SDL_Delay(16);
		}
	}
	close();
	return 0;
}