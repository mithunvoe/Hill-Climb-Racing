#ifndef Game_hpp
#define Game_hpp
#include <bits/stdc++.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
using namespace std;
class ColliderComponent;

class Game
{
public:
    Game();
    ~Game();
    void init(const char *title, int xpos, int yos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    void gameOver();
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;
    static bool inMenu;
    static bool isOver;
    static long long i;
    static deque<int> dq;
    bool musicOn = 1;
    static bool isHill, previsHill;
    bool inLeaderboard = 0;
    void setMenu();
    bool running() { return isRunning; };
    TTF_Font *font;
    static double currentFuel;

private:
    bool isRunning;
    int cnt = 0;
    SDL_Window *window;
};

#endif