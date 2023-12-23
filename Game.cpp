#include "Game.hpp"
#include "ECS/Component.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "highscore.hpp"

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
SDL_Texture *bgwohillTex;
SDL_Texture *bgTex;
SDL_Texture *coinTexture;
SDL_Texture *fuelTexture;
SDL_Texture /* Game:: */ *brake1;
SDL_Texture /* Game:: */ *brake2;
SDL_Texture /* Game:: */ *gas1;
SDL_Texture /* Game:: */ *gas2;

std::vector<ColliderComponent *> Game::colliders;
SDL_Rect src;
SDL_Rect dest;

double Game::currentFuel = 1000.0;
bool Game::inMenu = 1;
bool Game::isHill = 0;
bool Game::previsHill = 0;
bool Game::isOver = 0;
long long Game::i = 1;
deque<bool> Game::dq = {0, 0, 0};

Mix_Music *bgm;
Mix_Chunk *engine;
Mix_Chunk *coinSound;

int groundLevel = 400;
int currentScore;
int x, y;
bool majhkhanerstart;
bool majhkhanerend;
bool majhkhanerl;
bool startCursorCollision;
bool lCursorCollision;
bool endCursorCollision;
bool musicCursorCollision;
bool gasCursorCollision;
bool brakeCursorCollision;
bool Game::moveLeft;
bool Game::moveRight;

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
auto &gameover(manager.addEntity());
auto &eyn(manager.addEntity());
auto &gasButton(manager.addEntity());
auto &brakeButton(manager.addEntity());

string Game::name;
//
// Entity *coin[10];

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
    // cout << "Enter Your Name: ";
    system("clear");

    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    // cin >> name;
    SDL_Init(SDL_INIT_EVERYTHING);

    // cout<<3;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    bgm = Mix_LoadMUS("assets/bgm.mp3");
    coinSound = Mix_LoadWAV("assets/coin.wav");
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    isRunning = (renderer) ? true : false;
    Mix_PlayMusic(bgm, -1);
    Score::inputScore();
    TTF_Init();
    Game::font = TTF_OpenFont("assets/fnt.ttf", 200);

    menu.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    menu.addComponent<SpriteComponent>("assets/menu.jpg");

    cursor.addComponent<TransformComponent>(0, 0, 85, 150, 0.27);
    cursor.addComponent<SpriteComponent>("assets/cursor.png");
    cursor.addComponent<ColliderComponent>();
    SDL_ShowCursor(false);
    coinTexture = TextureManager::loadTexture("assets/coin.png");
    fuelTexture = TextureManager::loadTexture("assets/fuel.png");
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

    brake1 = TextureManager::loadTexture("assets/brake1.png");
    brake2 = TextureManager::loadTexture("assets/brake2.png");
    gas1 = TextureManager::loadTexture("assets/gas1.png");
    gas2 = TextureManager::loadTexture("assets/gas2.png");
    brakeButton.addComponent<TransformComponent>(10, 640 - 256 * 0.6 + 5, 256, 256, 0.6);
    brakeButton.addComponent<SpriteComponent>(brake1);
    brakeButton.addComponent<ColliderComponent>("brake");
    gasButton.addComponent<TransformComponent>(960 - 10 - 256 * 0.6, 640 - 256 * 0.6 + 5, 256, 256, 0.6);
    gasButton.addComponent<SpriteComponent>(gas1);
    gasButton.addComponent<ColliderComponent>("gas");

    musiconTex = TextureManager::loadTexture("assets/musicon.png");
    musicoffTex = TextureManager::loadTexture("assets/musicoff.png");

    musicButton.addComponent<TransformComponent>(890, 570, 900, 900, 0.073);
    musicButton.addComponent<SpriteComponent>(musiconTex);
    musicButton.addComponent<ColliderComponent>("mbutton");

    bgTex = TextureManager::loadTexture("assets/bg.png");
    bgwohillTex = TextureManager::loadTexture("assets/bgwohill.png");

    sky.addComponent<TransformComponent>(0, -1920 + 600, 1920, 1920, 1);
    sky.addComponent<SpriteComponent>("assets/sky.png");
    sky.addGroup(groupMap);
    bg.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    bg.addComponent<SpriteComponent>("assets/bgwohill.png", "amibg");
    bg.addGroup(groupMap);
    bg.addGroup(groupBg);
    bg.addGroup(groupSlide);
    bgg.addComponent<TransformComponent>(960, 0, 960, 640, 1);
    bgg.addComponent<SpriteComponent>("assets/bgwohill.png");
    bgg.addGroup(groupMap);
    bgg.addGroup(groupSlide);
    bgg.addGroup(groupBg);
    eyn.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    eyn.addComponent<SpriteComponent>("assets/eynsoto.png");

    leaderboard.addComponent<TransformComponent>(0, 0, 960, 640, 1);
    leaderboard.addComponent<SpriteComponent>("assets/leaderboard.png");

    scoreCoin.addComponent<TransformComponent>(20, 20, 100, 100, 0.5);
    scoreCoin.addComponent<SpriteComponent>("assets/coin.png");
    scoreCoin.addGroup(groupMap);

    fuel.addComponent<TransformComponent>(870, 14, 2068, 2072, 0.04);
    fuel.addComponent<SpriteComponent>(fuelTexture);
    fuel.addGroup(groupMap);

    fuelBorder.addComponent<TransformComponent>(666 + 5, 41 + 4, 210, 35, 0.988);
    fuelBorder.addComponent<SpriteComponent>("assets/fborder.png");

    gari.addComponent<TransformComponent>(20, groundLevel, 325, 215, .2);
    gari.addComponent<SpriteComponent>("assets/carAnim.png", chakaAnim, 5, "car", bg.getComponent<TransformComponent>());
    gari.addComponent<ColliderComponent>("gari");
    gari.addGroup(groupPlayers);

    gameover.addComponent<TransformComponent>(230, 110, 1000, 694, .5);
    gameover.addComponent<SpriteComponent>("assets/gameover.png");
    for (auto &it : manager.getGroup(groupSlide))
        it->addComponent<KeyboardController>(bg.getComponent<TransformComponent>());
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
void Game::gameOverFunc()
{
    gameover.draw();
    scoreTex = TextureManager::CreateTextTexture(font, "Score: " + to_string(currentScore), 0, 0, 0);
    src.x = src.y = 0;
    dest.y = 460;
    dest.w = 100 / 3 * (int)(to_string(currentScore).size() + 7);
    dest.x = (960 - dest.w) >> 1;
    dest.h = 90 / 3 * 2;
    src.w = 10000;
    src.h = 9000;
    TextureManager::Draw(scoreTex, src, dest);
    bg.getComponent<TransformComponent>().position.x = 0;
    bg.getComponent<TransformComponent>().position.y = 0;
    bg.getComponent<TransformComponent>().velocity.x = 0;
    bg.getComponent<TransformComponent>().velocity.y = 0;
    bg.getComponent<SpriteComponent>().setTexfromTex(bgwohillTex);

    bgg.getComponent<TransformComponent>().position.x = 960;
    bgg.getComponent<TransformComponent>().position.y = 0;
    bgg.getComponent<TransformComponent>().velocity.x = 0;
    bgg.getComponent<TransformComponent>().velocity.y = 0;
    bgg.getComponent<SpriteComponent>().setTexfromTex(bgwohillTex);

    // Score::inputScore();
    Score::addScore(currentScore, name);
    Game::i = 0;
    Game::isHill = Game::previsHill = 0;
    currentScore = 0;
    Game::currentFuel = 1000.0;
    manager.refresh();
    manager.update();
    setMenu();
    SDL_RenderPresent(renderer);
    SDL_Delay(4000);
    SDL_DestroyTexture(scoreTex);
}
void Game::handleEvents()
{
    SDL_PollEvent(&event);
    startCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), startbutton.getComponent<ColliderComponent>());
    endCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), endbutton.getComponent<ColliderComponent>());
    lCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), lbutton.getComponent<ColliderComponent>());
    gasCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), gasButton.getComponent<ColliderComponent>());
    brakeCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), brakeButton.getComponent<ColliderComponent>());

    musicCursorCollision = Collision::AABB(cursor.getComponent<ColliderComponent>(), musicButton.getComponent<ColliderComponent>());
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (Game::event.key.keysym.sym)
        {
        case SDLK_RIGHT:
            moveRight = 1;
            break;
        case SDLK_LEFT:
            moveLeft = 1;
            break;
        }
        break;
    case SDL_KEYUP:
        moveLeft = moveRight = 0;
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
        if (event.key.keysym.sym == SDLK_g)
        {
            gameOverFunc();
        }
        break;
    case SDL_QUIT:
        isRunning = false;
        if (currentScore)
        {
            // Score::inputScore();
            Score::addScore(currentScore, name);
        }

        break;
    case SDL_MOUSEBUTTONDOWN:
        if (!inMenu && event.button.button == SDL_BUTTON_LEFT)
        {
            if (gasCursorCollision)
            {
                moveLeft = 1;
                gasButton.getComponent<SpriteComponent>().setTexfromTex(gas2);
            }

            else if (brakeCursorCollision)
            {
                moveLeft = 1;
                brakeButton.getComponent<SpriteComponent>().setTexfromTex(brake2);
            }
        }
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
        moveLeft = moveRight = 0;
        gasButton.getComponent<SpriteComponent>().setTexfromTex(gas1);
        brakeButton.getComponent<SpriteComponent>().setTexfromTex(brake1);

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
                    if (currentScore)
                    {
                        Score::addScore(currentScore, name);
                    }
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

void kiBackgroundMathaNoshtoLagaCoin(bool is_hill)
{

    double coinX = min(-480, -480 - 200 + rand() % 960);
    double y_;
    for (int j = 0; j < 5; j++)
    {
        auto &a(manager.addEntity());
        if (is_hill)
            y_ = -2 * 1000000 / (coinX * coinX + 10000) + 380;
        else
            y_ = 380;

        a.addComponent<TransformComponent>(480 + coinX + 960 + 10, y_, 100, 100, .4);
        a.addComponent<SpriteComponent>(coinTexture, timeAnim, 9, "coin");
        a.addComponent<ColliderComponent>("coin");
        a.addComponent<KeyboardController>(bg.getComponent<TransformComponent>());
        a.addGroup(groupMap);
        coinX += 50;
    }
    if (Game::i % 8 == 0)
    {
        auto &a(manager.addEntity());
        if (is_hill)
            y_ = -2 * 1000000 / (coinX * coinX + 10000) + 380;
        else
            y_ = 380;
        a.addComponent<TransformComponent>(480 + coinX + 960 + 10, y_, 2068, 2072, 0.02);
        a.addComponent<SpriteComponent>(coinTexture, noAnim, 1, "coin");
        a.getComponent<SpriteComponent>().setTexfromTex(fuelTexture);
        a.addComponent<ColliderComponent>("fuel");
        a.addComponent<KeyboardController>(bg.getComponent<TransformComponent>());
        a.addGroup(groupMap);
    }
    // cout << endl;
}

void Game::update()
{
    if (isOver)
    {
        printf("khelashesh");
        gameOverFunc();
    }
    cout << currentFuel << endl;
    manager.refresh();
    manager.update();
    if (currentFuel <= 0)
        gameOverFunc();
    if (bg.getComponent<TransformComponent>().velocity.x < 0)
    {
        Mix_PlayChannel(-1, engine, 0);
    }

    if (bg.getComponent<TransformComponent>().position.x < -960)
    {
        i++;
        cout << i << endl;
        bg.getComponent<SpriteComponent>().setTexfromTex(bgg.getComponent<SpriteComponent>().texture);
        previsHill = isHill;
        isHill = (bool)(rand() & 1);
        if (dq.size() <= i)
            dq.push_back(isHill);
        if (!dq[i])
        {
            bgg.getComponent<SpriteComponent>().setTexfromTex(bgwohillTex);
        }
        else
        {
            bgg.getComponent<SpriteComponent>().setTexfromTex(bgTex);
        }
        bg.getComponent<TransformComponent>().position.x = 0;
        bgg.getComponent<TransformComponent>().position.x = 960;
        sky.getComponent<TransformComponent>().position.x = 0;
        kiBackgroundMathaNoshtoLagaCoin(dq[i]);
    }
    else if (bgg.getComponent<TransformComponent>().position.x > 960)
    {
        i--;
        cout << i << endl;
        bgg.getComponent<SpriteComponent>().setTexfromTex(bg.getComponent<SpriteComponent>().texture);
        isHill = (bool)(rand() & 1);
        previsHill = isHill;
        if (i <= 0)
            dq.push_front(isHill), i++;
        if (!dq[i - 1])
            dq.push_front(isHill), i++;
        if (!dq[i - 1])
        {
            bg.getComponent<SpriteComponent>().setTexfromTex(bgwohillTex);
        }
        else
        {
            bg.getComponent<SpriteComponent>().setTexfromTex(bgTex);
        }
        bg.getComponent<TransformComponent>().position.x = -960;
        bgg.getComponent<TransformComponent>().position.x = 0;
        sky.getComponent<TransformComponent>().position.x = -960;
    }
    // if (abs(gari.getComponent<SpriteComponent>().angle - gari.getComponent<SpriteComponent>().prevAngle) > 30 and abs(gari.getComponent<KeyboardController>().jiniser_y - gari.getComponent<KeyboardController>().matir_y) < 1)
    //     gameOverFunc();

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
                if (musicOn)
                    Mix_PlayChannel(-1, coinSound, 0);
            }
            if (cc->tag == (string) "fuel")
            {
                cc->tag = "khawaFuel";
                cc->entity->getComponent<SpriteComponent>().remove();
                Game::currentFuel = 1000.0;
            }
        }
    }
}
void Game::render()
{
    SDL_RenderClear(renderer);

    if (isNameMenu)
    {

        takeNameInput();
        isNameMenu = 0;
    }
    else if (inMenu)
    {
        if (inLeaderboard)
        {
            leaderboard.draw();
            int level = 155;
            vector<pair<int, string>> v = Score::topFive();
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
        src.x = 1000 - Game::currentFuel;
        src.y = 0;
        dest.w = 200 * Game::currentFuel / 1000;
        dest.h = 25;
        dest.x = 675 + 200 - dest.w;
        dest.y = 50;
        src.w = 1200;
        src.h = 150;
        TextureManager::Draw(fuelbarTex, src, dest);
        fuelBorder.draw();
        gari.draw();
        brakeButton.draw();
        gasButton.draw();

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
    SDL_DestroyTexture(brownstart);
    SDL_DestroyTexture(whitestart1);
    SDL_DestroyTexture(whitestart2);
    SDL_DestroyTexture(whitend2);
    SDL_DestroyTexture(whitel1);
    SDL_DestroyTexture(whitel2);
    SDL_DestroyTexture(brownl);
    SDL_DestroyTexture(tempTex);
    SDL_DestroyTexture(fuelbarTex);
    SDL_DestroyTexture(musiconTex);
    SDL_DestroyTexture(musicoffTex);
    SDL_DestroyTexture(leaderboardTex);
    SDL_DestroyTexture(nameTex);
    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(bgwohillTex);
    SDL_DestroyTexture(bgTex);
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    SDL_Quit();
    printf("Game Cleaned\nScore: %d\n", currentScore);
}
void Game::takeNameInput()
{

    SDL_StartTextInput();
    bool running = true;
    // SDL_Event *event;
    while (running)
    {
        SDL_RenderClear(renderer);
        eyn.draw();
        SDL_GetMouseState(&x, &y);
        cout << x << ' ' << y << endl;
        cout << cursor.getComponent<TransformComponent>().position.x << ' ' << cursor.getComponent<TransformComponent>().position.y << endl;
        cursor.getComponent<TransformComponent>().position.x = x;
        cursor.getComponent<TransformComponent>().position.y = y;
        while (SDL_PollEvent(&Game::event) != 0)
        {
            switch (Game::event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                running = false;
                // return;
                break;
            case SDL_TEXTINPUT:
                if (name.size() < 9)
                    name += Game::event.text.text;
                break;
            case SDL_KEYDOWN:
                if (Game::event.key.keysym.sym == SDLK_BACKSPACE && !name.empty())
                {
                    name.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN)
                {
                    running = false;
                }
                break;
                // case SDL_KEYUP:
                //     if (event.key.keysym.sym == SDLK_RETURN)
                //         eyn.getComponent<SpriteComponent>().setTexfromTex();
            }

            // cursor.draw();
            // tempTex;
        }
        src.x = src.y = 0;
        dest.y = 352;
        dest.w = 85 / 3 * (int)(name.size());
        dest.x = (960 - dest.w) >> 1;
        dest.h = 85 / 3 * 2;
        src.w = 10000;
        src.h = 9000;

        // cout << name << endl;
        if (name.size())
            tempTex = TextureManager::CreateTextTexture(Game::font, name, 83, 51, 44);
        TextureManager::Draw(tempTex, src, dest);

        SDL_DestroyTexture(tempTex);
        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();

    // cout << 33 << name << endl;
}