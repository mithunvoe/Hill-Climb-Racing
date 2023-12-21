#include "Game.hpp"
#include "Map.hpp"
#include "ECS/Component.hpp"
#include "GameObject.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "highscore.hpp"

double Game::currentFuel = 1000.0;
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
SDL_Texture *fuelbarTex;
std::vector<ColliderComponent *> Game::colliders;
SDL_Rect src;
SDL_Rect dest;

string name;

Mix_Music *bgm;
Mix_Chunk *engine;

int groundLevel = 350;
int currentScore;
// int currentFuel = 100;
int x, y;
// Game::currentFuel = 1000;
bool majhkhanerstart;
bool majhkhanerend;
bool startCursorCollision;
bool endCursorCollision;

auto &gari(manager.addEntity());
auto &wall(manager.addEntity());
auto &bg(manager.addEntity());
auto &bgg(manager.addEntity());
auto &menu(manager.addEntity());
auto &cursor(manager.addEntity());
auto &startbutton(manager.addEntity());
auto &endbutton(manager.addEntity());
auto &scoreCoin(manager.addEntity());
auto &fuel(manager.addEntity());
auto &fuelBorder(manager.addEntity());

Entity *coin[10];
enum groupLabels : size_t
{
    groupMap,
    groupPlayers,
    groupColliders,
    groupSlide
};
enum animLabel : int
{
    noAnim,
    timeAnim,
    spaceAnim,
    chakaAnim
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
    bg.addGroup(groupSlide);
    bgg.addComponent<TransformComponent>(960, 0, 960, 640, 1);
    bgg.addComponent<SpriteComponent>("assets/bg.png");
    bgg.addGroup(groupMap);
    bgg.addGroup(groupSlide);
    // swap(bg, bgg);
    // bgg=bg;

    scoreCoin.addComponent<TransformComponent>(20, 20, 100, 100, 0.5);
    scoreCoin.addComponent<SpriteComponent>("assets/coin.png");
    scoreCoin.addGroup(groupMap);


    fuelBorder.addComponent<TransformComponent>(666+5, 41+5, 210, 35, 0.99);
    fuelBorder.addComponent<SpriteComponent>("assets/fborder.png");
    fuelBorder.addGroup(groupPlayers);
    // fuelBorder.getComponent<TransformComponent>().entity.
    // fuelBorder.getComponent<TransformComponent>().;
    // fuelBorder.getComponent<TransformComponent>().width=200;

    fuel.addComponent<TransformComponent>(870, 14, 2068, 2072, 0.04);
    fuel.addComponent<SpriteComponent>("assets/fuel.png");
    fuel.addGroup(groupMap);


    gari.addComponent<TransformComponent>(1, groundLevel, 325, 215, .4);
    gari.addComponent<SpriteComponent>("assets/carAnim.png", chakaAnim, 5, "car", bgg.getComponent<TransformComponent>());
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
        coin[i]->addGroup(groupSlide);
    }
    for (auto &it : manager.getGroup(groupSlide))
        it->addComponent<KeyboardController>();
    TTF_Init();
    Game::font = TTF_OpenFont("assets/fnt.ttf", 200);
}

auto &tiles(manager.getGroup(groupMap));
auto &players(manager.getGroup(groupPlayers));
auto &slide(manager.getGroup(groupSlide));

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
    if (bg.getComponent<TransformComponent>().position.x < -960)
    {
        bg.getComponent<TransformComponent>().position.x = 0;
        bgg.getComponent<TransformComponent>().position.x = 960;
    }
    else if (bgg.getComponent<TransformComponent>().position.x > 960)
    {
        bg.getComponent<TransformComponent>().position.x = -960;
        bgg.getComponent<TransformComponent>().position.x = 0;
    }

    // cout<<bg.getComponent<TransformComponent>().position.x<<endl;

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
        // fuelTex = TextureManager::CreateTextTexture(Game::font, to_string(currentFuel));
        fuelbarTex = TextureManager::loadTexture("assets/fuelbar.png");
        for (auto &t : tiles)
            t->draw();
        TextureManager::Draw(tempTex, src, dest);
        dest.x += 600;
        src.x = 1000 - currentFuel;
        src.y = 0;
        dest.w = 200 * currentFuel / 1000;
        dest.h = 25;
        dest.x = 675 + 200 - dest.w;
        dest.y = 50;
        src.w = 1200;
        src.h = 150;
        TextureManager::Draw(fuelbarTex, src, dest);
        for (auto &p : players)
            p->draw();
        SDL_DestroyTexture(tempTex);
        SDL_DestroyTexture(fuelbarTex);
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
