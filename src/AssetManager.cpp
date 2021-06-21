#include "./AssetManager.h"

AssetManager::AssetManager(EntityManager *manager) : manager(manager)
{
}

//Clears all textures in the textures map
void AssetManager::ClearData()
{
    textures.clear();
    fonts.clear();
}

//Add a texture made out of the given path and put it into the texture map
void AssetManager::AddTexture(std::string textureId, const char *filePath)
{
    textures.emplace(textureId, TextureManager::LoadTexture(filePath));
}

void AssetManager::AddFont(std::string fontId, const char *filePath, int fontSize)
{
    fonts.emplace(fontId, FontManager::LoadFont(filePath, fontSize));
}

SDL_Texture *AssetManager::GetTexture(std::string textureId)
{
    return textures[textureId];
}

TTF_Font *AssetManager::GetFont(std::string fontId)
{
    return fonts[fontId];
}