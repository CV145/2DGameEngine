#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"

class TextLabelComponent : public Component
{
private:
    SDL_Rect position;
    std::string text;
    std::string fontFamily;
    SDL_Color color;
    SDL_Texture *texture;

public:
    TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color &color)
    {
        this->position.x = x;
        this->position.y = y;
        this->text = text;
        this->fontFamily = fontFamily;
        this->color = color;
        SetLabelText(text, fontFamily);
    }

    void SetLabelText(std::string text, std::string fontFamily)
    {
        /*
        An SDL_Surface contains the pixels of an image and the data needed to render it. The surface is returned by the TTF_RenderText_Blended method which takes in a font, text, and the color to render text in
        */

        SDL_Surface *surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text.c_str(), color);

        if (surface == NULL)
        {
            std::cerr << "ERROR: couldn't load font" << std::endl;
            printf("Oh My Goodness, an error : %s", TTF_GetError());

            exit(1);
        }

        texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

        //Free resources from a surface
        SDL_FreeSurface(surface);

        //Store texture information along with its width and height
        SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
    }

    void Render() override
    {
        //Draw the font texture at the given position
        FontManager::Draw(texture, position);
    }

    std::string GetText()
    {
        return text;
    }
};

#endif
