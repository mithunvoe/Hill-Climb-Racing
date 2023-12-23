#include "Game.hpp"
#include "ECS/Component.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "highscore.hpp"
// #include "Map.hpp"
// #include "GameObject.hpp"

SDL_Renderer *Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;
SDL_Surface *tempSurface;
SDL_Texture *brownend;
SDL_Texture *whitend1;
SDL_Texture *brownstart;
SDL_Texture *whitestart1;
SDL_Texture *whitestart2;
SDL_Texture *whitend2;
SDL_Texture *whitel1;
SDL_Texture *whitel2;
SDL_Texture *brownl;
SDL_Texture *tempTex;
SDL_Texture *fuelbarTex;
SDL_Texture *musiconTex;
SDL_Texture *musicoffTex;
SDL_Texture *leaderboardTex;
SDL_Texture *nameTex;
SDL_Texture *scoreTex;
SDL_Texture *bgwoHill;

std::vector<ColliderComponent *> Game::colliders;
SDL_Rect src;
SDL_Rect dest;

double Game::currentFuel = 1000.0;
bool Game::inMenu = 1;
bool Game::isHill = 0;
bool Game::previsHill = 0;

Mix_Music *bgm;
Mix_Chunk *engine;
Mix_Chunk *coinSound;

int groundLevel = 400;
int currentScore;
// int currentFuel = 100;
int x, y;
// Game::currentFuel = 1000;
bool majhkhanerstart;
bool majhkhanerend;
bool majhkhanerl;
bool startCursorCollision;
bool lCursorCollision;
bool endCursorCollision;
bool musicCursorCollision;

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
auto &musicButton(manager.addEntity());
auto &leaderboard(manager.addEntity());
auto &lbutton(manager.addEntity());
auto &sky(manager.addEntity());
string name;
//
Entity *coin[10];

enum groupLabels : size_t
{
    groupMap,
    groupPlayers,
    groupColliders,
    groupSlide,
    groupBg
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
    // bgm = Mix_LoadMUS("assets/bgm.mp3");
    coinSound = Mix_LoadWAV("assets/coin.wav");
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    isRunning = (renderer) ? true : false;
    Mix_PlayMusic(bgm, -1);
    Score::inputScore();

    menu.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    menu.addComponent<SpriteComponent>("assets/menu.jpg");

    cursor.addComponent<TransformComponent>(0, 0, 85, 150, 0.27);
    cursor.addComponent<SpriteComponent>("assets/cursor.png");
    cursor.addComponent<ColliderComponent>();
    SDL_ShowCursor(false);

    brownstart = TextureManager::loadTexture("assets/startbuttonb.png");
    whitestart1 = TextureManager::loadTexture("assets/startbuttonw1.png");
    whitestart2 = TextureManager::loadTexture("assets/startbuttonw2.png");
    startbutton.addComponent<TransformComponent>(0, 250 - 10, 1084, 353, 0.23);
    startbutton.addComponent<SpriteComponent>(brownstart);
    startbutton.getComponent<TransformComponent>().position.x = width / 2 - startbutton.getComponent<SpriteComponent>().destRect.w / 2;
    startbutton.addComponent<ColliderComponent>("sbutton");

    brownl = TextureManager::loadTexture("assets/lbuttonb.png");
    whitel1 = TextureManager::loadTexture("assets/lbuttonw1.png");
    whitel2 = TextureManager::loadTexture("assets/lbuttonw2.png");
    lbutton.addComponent<TransformComponent>(0, 350 - 10, 1084, 353, 0.23);
    lbutton.addComponent<SpriteComponent>(brownl);
    lbutton.getComponent<TransformComponent>().position.x = width / 2 - lbutton.getComponent<SpriteComponent>().destRect.w / 2;
    lbutton.addComponent<ColliderComponent>("lbutton");

    brownend = TextureManager::loadTexture("assets/endbuttonb.png");
    whitend1 = TextureManager::loadTexture("assets/endbuttonw1.png");
    whitend2 = TextureManager::loadTexture("assets/endbuttonw2.png");
    endbutton.addComponent<TransformComponent>(0, 450 - 10, 1084, 353, 0.23);
    endbutton.addComponent<SpriteComponent>(brownend);
    endbutton.getComponent<TransformComponent>().position.x = width / 2 - endbutton.getComponent<SpriteComponent>().destRect.w / 2;
    endbutton.addComponent<ColliderComponent>("ebutton");

    musiconTex = TextureManager::loadTexture("assets/musicon.png");
    musicoffTex = TextureManager::loadTexture("assets/musicoff.png");

    musicButton.addComponent<TransformComponent>(890, 570, 900, 900, 0.073);
    musicButton.addComponent<SpriteComponent>(musiconTex);
    musicButton.addComponent<ColliderComponent>("mbutton");

    sky.addComponent<TransformComponent>(0, -1920 + 250, 1920, 1920, 1);
    sky.addComponent<SpriteComponent>("assets/sky.png");
    sky.addGroup(groupMap);
    bg.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    bg.addComponent<SpriteComponent>("assets/bgwohill.png");
    bg.addGroup(groupMap);
    bg.addGroup(groupBg);
    bg.addGroup(groupSlide);
    bgg.addComponent<TransformComponent>(960, 0, 960, 640, 1);
    bgg.addComponent<SpriteComponent>("assets/bgwohill.png");
    bgg.addGroup(groupMap);
    bgg.addGroup(groupSlide);
    bgg.addGroup(groupBg);


    leaderboard.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    leaderboard.addComponent<SpriteComponent>("assets/leaderboard.png");

    scoreCoin.addComponent<TransformComponent>(20, 20, 100, 100, 0.5);
    scoreCoin.addComponent<SpriteComponent>("assets/coin.png");
    scoreCoin.addGroup(groupMap);

    fuel.addComponent<TransformComponent>(870, 14, 2068, 2072, 0.04);
    fuel.addComponent<SpriteComponent>("assets/fuel.png");
    fuel.addGroup(groupMap);

    fuelBorder.addComponent<TransformComponent>(666 + 5, 41 + 4, 210, 35, 0.988);
    fuelBorder.addComponent<SpriteComponent>("assets/fborder.png");

    gari.addComponent<TransformComponent>(20, groundLevel, 325, 215, .2);
    gari.addComponent<SpriteComponent>("assets/carAnim.png", chakaAnim, 5, "car", bg.getComponent<TransformComponent>());
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
        it->addComponent<KeyboardController>(bg.getComponent<TransformComponent>());
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
    lCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), lbutton.getComponent<ColliderComponent>());
    musicCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), musicButton.getComponent<ColliderComponent>());
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        Score::addScore(currentScore, name);
        break;
    case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            setMenu();
        else if (event.key.keysym.sym == SDLK_m)
        {
            if (musicOn)
            {
                Mix_PauseMusic();
                musicOn = 0;
                musicButton.getComponent<SpriteComponent>().setTexfromTex(musicoffTex);
            }
            else
            {
                Mix_ResumeMusic();
                musicOn = 1;
                musicButton.getComponent<SpriteComponent>().setTexfromTex(musiconTex);
            }
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (inMenu && event.button.button == SDL_BUTTON_LEFT)
        {
            if (!inLeaderboard)
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
                if (lCursorCollision)
                {
                    majhkhanerl = 1;
                    lbutton.getComponent<SpriteComponent>().setTexfromTex(whitel2);
                }
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (inMenu && event.button.button == SDL_BUTTON_LEFT)
        {
            if (musicCursorCollision)
            {
                if (musicOn)
                {
                    musicOn = 0;
                    Mix_PauseMusic();
                    musicButton.getComponent<SpriteComponent>().setTexfromTex(musicoffTex);
                }
                else
                {
                    musicOn = 1;
                    Mix_ResumeMusic();
                    musicButton.getComponent<SpriteComponent>().setTexfromTex(musiconTex);
                }
                break;
            }
            if (inLeaderboard)
                inLeaderboard = majhkhanerl = 0;
            else
            {
                if (startCursorCollision)
                    setMenu();
                else if (endCursorCollision)
                {
                    isRunning = false;
                    Score::inputScore();
                    Score::addScore(currentScore, name);
                }
                else if (lCursorCollision)
                {
                    inLeaderboard ^= 1;
                }

                else
                    majhkhanerend = majhkhanerstart = majhkhanerl = 0;
            }
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

    if (!majhkhanerl && lCursorCollision)
        lbutton.getComponent<SpriteComponent>().setTexfromTex(whitel1);
    else if (!majhkhanerl)
        lbutton.getComponent<SpriteComponent>().setTexfromTex(brownl);
}
void Game::update()
{
    // cout<<isHill<<endl;
    manager.refresh();
    manager.update();
    if (bg.getComponent<TransformComponent>().velocity.x < 0)
    {
        Mix_PlayChannel(-1, engine, 0);
    }
    if (bg.getComponent<TransformComponent>().position.x < -960)
    {
        bg.addComponent<SpriteComponent>().setTexfromTex(bgg.getComponent<SpriteComponent>().texture);
        previsHill = isHill;
        isHill = (bool)(rand() & 1);

        if (!isHill)
        {
            bgg.addComponent<SpriteComponent>("assets/bgwohill.png");
        }
        else
        {
            bgg.addComponent<SpriteComponent>("assets/bg.png");
        }
        bg.getComponent<TransformComponent>().position.x = 0;
        bgg.getComponent<TransformComponent>().position.x = 960;
        sky.getComponent<TransformComponent>().position.x = 0;
    }
    else if (bgg.getComponent<TransformComponent>().position.x > 960)
    {
        bgg.addComponent<SpriteComponent>().setTexfromTex(bg.getComponent<SpriteComponent>().texture);
        isHill = (bool)(rand() & 1);
        previsHill = isHill;

        if (!isHill)
        {
            bg.addComponent<SpriteComponent>("assets/bgwohill.png");
        }
        else
        {
            bg.addComponent<SpriteComponent>("assets/bg.png");
        }
        bg.getComponent<TransformComponent>().position.x = -960;
        bgg.getComponent<TransformComponent>().position.x = 0;
        sky.getComponent<TransformComponent>().position.x = -960;
    }

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
                Mix_PlayChannel(-1, coinSound, 0);
            }
        }
    }
}
void Game::render()
{
    SDL_RenderClear(renderer);
    if (inMenu)
    {
        if (inLeaderboard)
        {
            leaderboard.draw();
            int level = 155;
            vector<pair<int, string>> v = Score::topFive();
            // cout << v.size() << endl;
            for (int i = 0; i < v.size(); i++, level += 80)
            {
                nameTex = TextureManager::CreateTextTexture(font, to_string(i + 1) + ". " + v[i].second, 255, 225, 134);
                scoreTex = TextureManager::CreateTextTexture(font, to_string(v[i].first), 255, 225, 134);
                src.x = src.y = 0;
                dest.x = 200;
                dest.y = level;
                dest.w = 100 / 3.5 * (int)(3 + v[i].second.size());
                dest.h = 90 / 3 * 2;
                src.w = 1000;
                src.h = 900;
                TextureManager::Draw(nameTex, src, dest);
                dest.x = 620;
                dest.w = 100 / 3.5 * (int)(v[i].first ? log10(v[i].first) + 1 : 0 + 1);
                TextureManager::Draw(scoreTex, src, dest);
                SDL_DestroyTexture(nameTex);
                SDL_DestroyTexture(scoreTex);
            }
        }
        else
        {
            menu.draw();
            startbutton.draw();
            lbutton.draw();
            endbutton.draw();
        }
        musicButton.draw();
        cursor.draw();
    }
    else
    {
        src.x = src.y = 0;
        dest.x = 85;
        dest.y = 15;
        dest.w = 100 / 3 * (int)(currentScore ? log10(currentScore) + 1 : 0 + 1);
        dest.h = 90 / 3 * 2;
        src.w = 1000;
        src.h = 900;
        tempTex = TextureManager::CreateTextTexture(Game::font, to_string(currentScore), 83, 51, 44);
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
        fuelBorder.draw();
        gari.draw();
        SDL_DestroyTexture(tempTex);
        SDL_DestroyTexture(fuelbarTex);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(brownend);
    SDL_DestroyTexture(whitend1);
    SDL_DestroyTexture(whitend2);
    SDL_DestroyTexture(brownstart);
    SDL_DestroyTexture(whitestart1);
    SDL_DestroyTexture(whitestart2);
    SDL_DestroyTexture(brownl);
    SDL_DestroyTexture(whitel1);
    SDL_DestroyTexture(whitel2);
    SDL_DestroyTexture(tempTex);
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    SDL_Quit();
    printf("Game Cleaned\nScore: %d\n", currentScore);
}