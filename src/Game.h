#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/lua/sol.hpp"
#include "./Entity.h"
#include "./Components/Component.h"
#include "./EntityManager.h"

/*

Sol is a framework used to bind C++ and Lua

*/

class AssetManager;

class Game
{
private:
    bool isRunning;
    SDL_Window *window;

public:
    Game();
    ~Game();
    int ticksLastFrame;

    //Checks if the game is still running
    bool IsRunning() const; //getters are constant

    //Pointer to the renderer
    static SDL_Renderer *renderer;

    static AssetManager *assetManager;
    static SDL_Event event;

    //A camera is an SDL Rectangle
    static SDL_Rect camera;

    void LoadLevel(int levelNumber);
    void Initialize(int width, int height);
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
    void HandleCameraMovement();
    void CheckCollisions();
    void ProcessNextLevel(int levelNum);
    void ProcessGameOver();
};

#endif