#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"
#include "../Animation.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;

    //The sourceRectangle cycles through an animation spritesheet
    SDL_Rect sourceRectangle;

    SDL_Rect destinationRectangle;
    bool isAnimated;
    int numOfFrames;
    int animSpeed;

    //Always stay in the same position in the screen - used for UI
    bool isFixed;

    bool hasDirections = false;

    //Map that keeps track of this sprite's animations
    std::map<std::string, Animation> animations;

    std::string currentAnimName;
    unsigned int animIndex = 0;

public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    bool isShooting = false;

    SpriteComponent(const char *assetId)
    {
        isAnimated = false;
        isFixed = false;
        SetTexture(assetId);
    }

    SpriteComponent(std::string id, int numFrames, int animSpeed, bool hasDirections, bool isFixed)
    {
        isAnimated = true;
        numOfFrames = numFrames;
        this->animSpeed = animSpeed;
        this->isFixed = isFixed;

        if (hasDirections)
        {
            this->hasDirections = true;
            Animation leftAnimation = Animation(0, numFrames, animSpeed);
            Animation downAnimation = Animation(1, numFrames, animSpeed);
            Animation upAnimation = Animation(2, numFrames, animSpeed);
            Animation rightAnimation = Animation(3, numFrames, animSpeed);
            Animation shootAnimation = Animation(4, 2, 1500);

            animations.emplace("LeftAnimation", leftAnimation);
            animations.emplace("DownAnimation", downAnimation);
            animations.emplace("UpAnimation", upAnimation);
            animations.emplace("RightAnimation", rightAnimation);
            animations.emplace("ShootAnimation", shootAnimation);

            this->animIndex = 0;
            this->currentAnimName = "LeftAnimation";
        }
        else
        {
            Animation singleAnim = Animation(0, numFrames, animSpeed);
            animations.emplace("SingleAnimation", singleAnim);
            this->animIndex = 0;
            this->currentAnimName = "SingleAnimation";
        }

        Play(this->currentAnimName);
        SetTexture(id);
    }

    void Play(std::string animationName)
    {
        numOfFrames = animations[animationName].numFrames;
        animIndex = animations[animationName].index;
        animSpeed = animations[animationName].animationSpeed;
        currentAnimName = animationName;
    }

    void SetTexture(std::string assetTextureId)
    {
        //Grab a texture from the asset manager using the given string ID
        texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void Initialize() override
    {
        //The transform is the owner entity's Transform Component
        transform = owner->GetComponent<TransformComponent>();
        sourceRectangle.x = 0;
        sourceRectangle.y = 0;
        sourceRectangle.w = transform->width;
        sourceRectangle.h = transform->height;
    }

    void Update(float deltaTime) override
    {
        //This is where the animation occurs
        if (isAnimated)
        {
            //It's cycling the source rectangle through the sprite sheet, per frame, times a given animation speed
            sourceRectangle.x = (sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animSpeed) % numOfFrames));
        }

        //Moving up and down on the sprite sheet cycles through facing directions
        sourceRectangle.y = animIndex * transform->height;

        //Allows sprites to move in relation with the camera
        destinationRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
        destinationRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
        destinationRectangle.w = transform->width * transform->scale;
        destinationRectangle.h = transform->height * transform->scale;

        if (isShooting)
        {
            Play("ShootAnimation");
        }
        else if (hasDirections)
        {
            //Play animations if transform is moving
            if (transform->velocity.x > 0)
            {
                Play("RightAnimation");
            }
            else if (transform->velocity.x < 0)
            {
                Play("LeftAnimation");
            }
            else if (transform->velocity.y > 0)
            {
                Play("DownAnimation");
            }
            else if (transform->velocity.y < 0)
            {
                Play("UpAnimation");
            }
        }
    }

    void Render() override
    {
        TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
    }
};

#endif