#include "./TextureManager.h"

//Creates a texture from the given file directory
SDL_Texture *TextureManager::LoadTexture(const char *fileName)
{
    //Load an image as an SDL_Surface (pixel data) from the given directory
    SDL_Surface *surface = IMG_Load(fileName);

    //The texture is created using the surface and a renderer
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

    //Frees resources
    SDL_FreeSurface(surface);

    return texture;
}

//Draw the given texture onto a rectangle, applying the RendererFLip to it
void TextureManager::Draw(SDL_Texture *texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
}

/*
SDL_RenderCopy
- Copies a portion of the given texture to a rendering target (the main game renderer)

The source rectangle is the portion of the texture to copy

The destination rectangle is the portion of the renderer, or rather *where* in the renderer to place the texture in. The texture will stretch to fit the target rectangle

return 0 = success

*/