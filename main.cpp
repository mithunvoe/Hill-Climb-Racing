#include "Game.hpp"
Game *game = nullptr;
int main()
{
    const int FPS = 350;
    const int frameDelay = 1000 / FPS;
    uint32_t frameStart;
    int frameTime;

    game = new Game();
    game->init("Gari Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640, false);
   
    while (game->running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        if (!game->isOver)
        {
            game->update();
            game->render();
        }
        game->isOver = 0;

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();
    return 0;
}