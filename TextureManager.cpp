#include "TextureManager.hpp"

SDL_Texture *TextureManager::loadTexture(const char *texture)
{
    SDL_Surface *tempSurface = IMG_Load(texture);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

SDL_Texture *TextureManager::CreateTextTexture(TTF_Font *font, string text,int r, int g, int b )
{
    SDL_Surface *tmpSurface = TTF_RenderText_Solid(font, text.c_str(), {r, g, b});
    if (tmpSurface == NULL)
    {
        cout << "surface error" ;
        exit(0);
    }
    SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    if (tmpTex == NULL)
    {
        cout << "surface error"  ;
        exit(0);
    }
    SDL_FreeSurface(tmpSurface);
    
    return tmpTex;
}