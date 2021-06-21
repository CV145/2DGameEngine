#include "./FontManager.h"

TTF_Font *FontManager::LoadFont(const char *fileName, int fontSize)
{
    TTF_Font *font = TTF_OpenFont(fileName, fontSize);

    if (font == NULL)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
}

void FontManager::Draw(SDL_Texture *texture, SDL_Rect position)
{
    //The Ex version just has extra parameters
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}