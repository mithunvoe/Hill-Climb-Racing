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
    void gameOverFunc();
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;
    static bool inMenu;
    static bool isOver;
    static long long i;
    static deque<bool> dq;
    bool musicOn = 1;
    static bool isHill, previsHill;
    bool inLeaderboard = 0;
    bool inInfo = 0;
    void setMenu();
    bool running() { return isRunning; };
    TTF_Font *font;
    static string name;
    static double currentFuel;
    void takeNameInput();
    bool isNameMenu = 1;
    SDL_Texture *brake1, *brake2, *gas1, *gas2;
    bool startCursorCollision;
    bool lCursorCollision;
    bool iCursorCollision;
    bool endCursorCollision;
    bool musicCursorCollision;
    bool brakeCursorCollision;
    bool gasCursorCollision;
    static bool moveLeft, moveRight;

private:
    bool isRunning;
    int cnt = 0;
    SDL_Window *window;
};

#endif