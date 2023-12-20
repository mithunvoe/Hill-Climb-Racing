#include "Game.hpp"
#include "Map.hpp"
#include "ECS/Component.hpp"
#include "GameObject.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "highscore.hpp"

Map *mapObject;
SDL_Renderer *Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;
SDL_Surface *tempSurface;
SDL_Texture *brownstart;
SDL_Texture *brownend;
SDL_Texture *whitestart1;
SDL_Texture *whitend1;
SDL_Texture *whitestart2;
SDL_Texture *whitend2;
SDL_Texture *tempTex;
std::vector<ColliderComponent *> Game::colliders;
SDL_Rect src;
SDL_Rect dest;

string name;

Mix_Music *bgm;
Mix_Chunk *engine;

int groundLevel = 350;
int currentScore;
int x, y;

bool majhkhanerstart;
bool majhkhanerend;
bool startCursorCollision;
bool endCursorCollision;

auto &gari(manager.addEntity());
auto &wall(manager.addEntity());
auto &bg(manager.addEntity());
auto &menu(manager.addEntity());
auto &cursor(manager.addEntity());
auto &startbutton(manager.addEntity());
auto &endbutton(manager.addEntity());
auto &scoreCoin(manager.addEntity());

Entity *coin[10];
enum groupLabels : size_t
{
    groupMap,
    groupPlayers,
    groupColliders
};
enum animLabel : int
{
    noAnim,
    timeAnim,
    spaceAnim
};

Game::Game() {}
Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    cout << "Enter Your Name: ";
    system("clear");

    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    cin >> name;
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    bgm = Mix_LoadMUS("assets/bgm.mp3");
    engine = Mix_LoadWAV("assets/engine.mp3");
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    isRunning = (renderer) ? true : false;
    mapObject = new Map();
    Mix_PlayMusic(bgm, -1);

    menu.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    menu.addComponent<SpriteComponent>("assets/menu.jpg");

    cursor.addComponent<TransformComponent>(0, 0, 85, 150, 0.5);
    cursor.addComponent<SpriteComponent>("assets/cursor.png");
    cursor.addComponent<ColliderComponent>();

    brownstart = TextureManager::loadTexture("assets/startbuttonb.png");
    whitestart1 = TextureManager::loadTexture("assets/startbuttonw1.png");
    whitestart2 = TextureManager::loadTexture("assets/startbuttonw2.png");
    startbutton.addComponent<TransformComponent>(0, 250, 1084, 353, 0.3);
    startbutton.addComponent<SpriteComponent>(brownstart);
    startbutton.getComponent<TransformComponent>().position.x = width / 2 - startbutton.getComponent<SpriteComponent>().destRect.w / 2;
    startbutton.addComponent<ColliderComponent>("sbutton");

    brownend = TextureManager::loadTexture("assets/endbuttonb.png");
    whitend1 = TextureManager::loadTexture("assets/endbuttonw1.png");
    whitend2 = TextureManager::loadTexture("assets/endbuttonw2.png");
    endbutton.addComponent<TransformComponent>(0, 380, 1084, 353, 0.3);
    endbutton.addComponent<SpriteComponent>(brownend);
    endbutton.getComponent<TransformComponent>().position.x = width / 2 - endbutton.getComponent<SpriteComponent>().destRect.w / 2;

    endbutton.addComponent<ColliderComponent>("ebutton");
    SDL_ShowCursor(false);

    bg.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    bg.addComponent<SpriteComponent>("assets/bg.png");
    bg.addGroup(groupMap);

    scoreCoin.addComponent<TransformComponent>(20, 20, 100, 100, 0.5);
    scoreCoin.addComponent<SpriteComponent>("assets/coin.png");
    scoreCoin.addGroup(groupMap);

    gari.addComponent<TransformComponent>(1, groundLevel, 325, 215, .4);
    gari.addComponent<KeyboardController>();
    gari.addComponent<SpriteComponent>("assets/carAnim.png", spaceAnim, 5, "car");
    gari.addComponent<ColliderComponent>("gari");
    gari.addGroup(groupPlayers);

    int coinPos[] = {557, 112, 218, 314, 375, 469, 587, 790, 650, 850};
    for (int i = 0; i < 10; i++)
    {
        coin[i] = &manager.addEntity();
        coin[i]->addComponent<TransformComponent>(coinPos[i], groundLevel + 20, 100, 100, .4);
        coin[i]->addComponent<SpriteComponent>("assets/coin.png", timeAnim, 9, "coin");
        coin[i]->addComponent<ColliderComponent>("coin");
        coin[i]->addGroup(groupMap);
    }
    TTF_Init();
    Game::font = TTF_OpenFont("assets/fnt.ttf", 200);
}

auto &tiles(manager.getGroup(groupMap));
auto &players(manager.getGroup(groupPlayers));

void Game::setMenu()
{
    inMenu = inMenu ^ 1;
    majhkhanerstart = 0;
    majhkhanerend = 0;
}
void Game::handleEvents()
{
    SDL_PollEvent(&event);
    startCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), startbutton.getComponent<ColliderComponent>());
    endCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), endbutton.getComponent<ColliderComponent>());
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        Score::inputScore();
        Score::addScore(currentScore, name);
        break;
    case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            setMenu();
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (inMenu && event.button.button == SDL_BUTTON_LEFT)
        {
            if (startCursorCollision)
            {
                majhkhanerstart = 1;
                startbutton.getComponent<SpriteComponent>().setTexfromTex(whitestart2);
            }
            if (endCursorCollision)
            {
                majhkhanerend = 1;
                endbutton.getComponent<SpriteComponent>().setTexfromTex(whitend2);
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (inMenu && event.button.button == SDL_BUTTON_LEFT)
        {
            if (startCursorCollision)
                setMenu();
            else if (endCursorCollision)
            {
                isRunning = false;
                Score::inputScore();
                Score::addScore(currentScore, name);
            }
            else
                majhkhanerend = majhkhanerstart = 0;
        }
    default:
        break;
    }
    if (!majhkhanerstart && startCursorCollision)
        startbutton.getComponent<SpriteComponent>().setTexfromTex(whitestart1);
    else if (!majhkhanerstart)
        startbutton.getComponent<SpriteComponent>().setTexfromTex(brownstart);

    if (!majhkhanerend && endCursorCollision)
        endbutton.getComponent<SpriteComponent>().setTexfromTex(whitend1);
    else if (!majhkhanerend)
        endbutton.getComponent<SpriteComponent>().setTexfromTex(brownend);
}
void Game::update()
{

    manager.refresh();
    manager.update();
    if (gari.getComponent<TransformComponent>().velocity.x > 0.1)
        Mix_PlayChannel(-1, engine, 0);

    SDL_GetMouseState(&x, &y);
    cursor.getComponent<TransformComponent>().position.x = x;
    cursor.getComponent<TransformComponent>().position.y = y;

    for (auto cc : colliders)
    {
        if (Collision::AABB(gari.getComponent<ColliderComponent>(), *cc))
        {
            if (cc->tag == (string) "coin")
            {
                cc->tag = "khawaCoin";
                cc->entity->getComponent<SpriteComponent>().remove();
                currentScore += 10;
            }
        }
    }
}
void Game::render()
{

    SDL_RenderClear(renderer);
    if (inMenu)
    {
        menu.draw();
        startbutton.draw();
        endbutton.draw();
        cursor.draw();
    }
    else
    {
        src.x = src.y = 0;
        dest.x = 85;
        dest.y = 15;
        dest.w = 100 / 3 * (int)(log10(currentScore) + 1);
        dest.h = 90 / 3 * 2;
        src.w = 1000;
        src.h = 900;
        tempTex = TextureManager::CreateTextTexture(Game::font, to_string(currentScore));

        for (auto &t : tiles)
            t->draw();
        for (auto &p : players)
            p->draw();
        TextureManager::Draw(tempTex, src, dest);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(brownstart);
    SDL_DestroyTexture(brownstart);
    SDL_DestroyTexture(brownend);
    SDL_DestroyTexture(whitend1);
    SDL_DestroyTexture(whitend2);
    SDL_DestroyTexture(whitestart1);
    SDL_DestroyTexture(whitestart2);
    SDL_DestroyTexture(tempTex);
    SDL_Quit();
    printf("Game Cleaned\nScore: %d\n", currentScore);
    
}
void Game::AddTile(int id, int x, int y, int w, int h)
{
}
