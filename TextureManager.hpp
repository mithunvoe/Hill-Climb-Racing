#include "Game.hpp"

class TextureManager
{
public:
    static SDL_Texture *loadTexture(const char *filename);
    static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest);
    static void DrawGari(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, float angle);
    static SDL_Texture *CreateTextTexture(TTF_Font *font, string text, int r, int g, int b);
    // static SDL_Surface *tmpSurface = TTF_RenderText_Solid(font, text.c_str(), {83, 51, 44});
    static SDL_Surface *tmpSurface;
};