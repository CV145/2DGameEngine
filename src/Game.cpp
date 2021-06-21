#include <iostream>
#include <ctime>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "./Components/AIComponent.h"
#include "glm/glm.hpp"

EntityManager manager;
AssetManager *Game::assetManager = new AssetManager(&manager);
SDL_Renderer *Game::renderer;
SDL_Event Game::event;
Entity *player;

//A camera is a SDL Rectangle that's the same size as the window
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

Map *map;

Game::Game()
{
    this->isRunning = false;
    ticksLastFrame = 0;
}

Game::~Game()
{
}

bool Game::IsRunning() const
{
    return this->isRunning;
}

//Creates the Game window and renderer
void Game::Initialize(int width, int height)
{
    srand(time(NULL));

    //Initialize the SDL library
    //When SDL_Init is called GetTicks() begins to count ticks
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        //if 0 not returned, there is an error
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    LoadLevel(1);

    isRunning = true;
    return;
}

//All the level information is stored in a lua file
void Game::LoadLevel(int levelNumber)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    ///////////////////////////////////////////////////////////////////////////
    // LOAD LIST OF ASSETS FROM LUA CONFIG FILE
    ///////////////////////////////////////////////////////////////////////////
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];

    unsigned int assetIndex = 0;
    while (true)
    {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if (assetType.compare("texture") == 0)
            {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            }
            if (assetType.compare("font") == 0)
            {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->AddFont(assetId, assetFile.c_str(), fontSize);
            }
        }
        assetIndex++;
    }

    //Add waypoint entities
    int numWaypoints = levelData["waypoints"]["count"];
    int minDomain = levelData["waypoints"]["minDomain"];
    int maxDomain = levelData["waypoints"]["maxDomain"];
    int minRange = levelData["waypoints"]["minRange"];
    int maxRange = levelData["waypoints"]["maxRange"];

    for (int i = 0; i < numWaypoints; i++)
    {
        //Grab random x and y positions
        int xPos = (rand() % maxDomain) + minDomain;
        int yPos = (rand() % maxRange) + minRange;

        auto &newWaypoint(manager.AddEntity("waypoint" + std::to_string(i), WAYPOINT_LAYER));

        newWaypoint.AddComponent<TransformComponent>(xPos, yPos, 0, 0, 32, 32, 1);

        newWaypoint.AddComponent<ColliderComponent>("WAYPOINT", xPos, yPos, 32, 32);
    }

    ///////////////////////////////////////////////////////////////////////////
    // LOAD MAP AND TILE INFORMATION FROM LUA CONFIG FILE
    ///////////////////////////////////////////////////////////////////////////
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"]));

    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"]));

    ///////////////////////////////////////////////////////////////////////////
    // LOAD ENTITIES AND COMPONENTS FROM LUA CONFIG FILE
    ///////////////////////////////////////////////////////////////////////////
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    int copies = 1;
    while (true)
    {
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table entity = levelEntities[entityIndex];

            copies = entity["copies"];

            for (int i = 0; i < copies; i++)
            {

                std::string entityName = entity["name"];
                entityName += std::to_string(i);
                LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

                // Add new entity
                auto &newEntity(manager.AddEntity(entityName, entityLayerType));

                // Add Transform Component
                sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
                if (existsTransformComponent != sol::nullopt)
                {
                    newEntity.AddComponent<TransformComponent>(
                        static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                        static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                        static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                        static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                        static_cast<int>(entity["components"]["transform"]["width"]),
                        static_cast<int>(entity["components"]["transform"]["height"]),
                        static_cast<int>(entity["components"]["transform"]["scale"]));
                }

                // Add sprite component
                sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
                if (existsSpriteComponent != sol::nullopt)
                {
                    std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                    bool isAnimated = entity["components"]["sprite"]["animated"];
                    if (isAnimated)
                    {
                        newEntity.AddComponent<SpriteComponent>(
                            textureId,
                            static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                            static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                            static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                            static_cast<bool>(entity["components"]["sprite"]["fixed"]));
                    }
                    else
                    {
                        newEntity.AddComponent<SpriteComponent>(textureId.c_str());
                    }
                }

                // Add input control component
                sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
                if (existsInputComponent != sol::nullopt)
                {
                    sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                    if (existsKeyboardInputComponent != sol::nullopt)
                    {
                        std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                        std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                        std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                        std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                        std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                        newEntity.AddComponent<KeyboardControlComponent>(upKey, rightKey, downKey, leftKey, shootKey);
                    }

                    player = &newEntity;
                }

                // Add collider component
                sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
                if (existsColliderComponent != sol::nullopt)
                {
                    std::string colliderTag = entity["components"]["collider"]["tag"];

                    if (static_cast<int>(entity["components"]["collider"]["modified"] == 1))
                    {
                        newEntity.AddComponent<ColliderComponent>(
                            colliderTag,
                            static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                            static_cast<int>(entity["components"]["collider"]["deltaX"]),
                            static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                            static_cast<int>(entity["components"]["collider"]["deltaY"]),
                            static_cast<int>(entity["components"]["transform"]["width"]),
                            static_cast<int>(entity["components"]["collider"]["deltaW"]),
                            static_cast<int>(entity["components"]["transform"]["height"]),
                            static_cast<int>(entity["components"]["collider"]["deltaH"]));
                    }
                    else
                    {
                        newEntity.AddComponent<ColliderComponent>(
                            colliderTag,
                            static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                            static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                            static_cast<int>(entity["components"]["transform"]["width"]),
                            static_cast<int>(entity["components"]["transform"]["height"]));
                    }
                }

                Entity *projectile;
                // Add projectile emitter component
                sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
                if (existsProjectileEmitterComponent != sol::nullopt)
                {
                    int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                    int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                    int parentEntityWidth = entity["components"]["transform"]["width"];
                    int parentEntityHeight = entity["components"]["transform"]["height"];
                    int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                    int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                    int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                    int projectileRange = entity["components"]["projectileEmitter"]["range"];
                    int projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                    std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                    projectile = &manager.AddEntity("projectile", PROJECTILE_LAYER);
                    projectile->AddComponent<TransformComponent>(
                        parentEntityXPos + (parentEntityWidth / 2),
                        parentEntityYPos + (parentEntityHeight / 2),
                        0,
                        0,
                        projectileWidth,
                        projectileHeight,
                        1);
                    projectile->AddComponent<SpriteComponent>(textureAssetId.c_str());
                    projectile->AddComponent<ProjectileEmitterComponent>(
                        projectileSpeed,
                        projectileRange,
                        projectileShouldLoop,
                        newEntity, manager);
                    projectile->AddComponent<ColliderComponent>(
                        "PROJECTILE",
                        parentEntityXPos,
                        parentEntityYPos,
                        projectileWidth,
                        projectileHeight);
                }

                //Add AI component
                sol::optional<sol::table> existsAIComponent = entity["components"]["AI"];
                if (existsAIComponent != sol::nullopt)
                {
                    int speed = entity["components"]["AI"]["speed"];
                    newEntity.AddComponent<AIComponent>(&manager, speed, projectile);
                }
            }
        }
        entityIndex++;
    }
}

void Game::ProcessInput()
{
    SDL_PollEvent(&event); //listen/receive for the event

    //action based on event type
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        //get escape key symbol from the event
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            isRunning = false;
        }
    default:
        break;
    }
}

//This function gets called repeatedly in a while loop inside main()
void Game::Update()
{
    //difference between target frame time and the time it took to render last frame
    int remainingTime = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    //delay if frame rendered faster than the target time
    if (remainingTime > 0 && remainingTime <= FRAME_TARGET_TIME)
    {
        SDL_Delay(remainingTime);
    }

    //delta time is time (ticks) since last frame in seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    //clamp delta time to a max value - delta time cannot be longer than 0.05 ms
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    //milliseconds since SDL_Init called
    ticksLastFrame = SDL_GetTicks();

    //call the manager.update to update all entities
    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

//Render the game
void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

    //clear the back buffer with a drawing color
    SDL_RenderClear(renderer);

    //call the manager.render to render all entities
    if (manager.HasNoEntities())
    {
        return;
    }
    manager.Render();

    //swaps the newly updated back buffer with the old front buffer
    SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement()
{
    TransformComponent *mainPlayerTransform = player->GetComponent<TransformComponent>();

    //Move when player reaches half of the window
    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    //Prevent update of camera when outside boundaries of map
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions()
{
    CollisionType collisionType = manager.CheckCollisions();

    if (collisionType == PLAYER_ENEMY_COLLISION)
    {
        ProcessGameOver();
    }
    if (collisionType == PLAYER_PROJECTILE_COLLISION)
    {
        ProcessGameOver();
    }
    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION)
    {
        ProcessNextLevel(1);
    }
}

void Game::ProcessNextLevel(int levelNumber)
{
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}

void Game::ProcessGameOver()
{
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*
There are back buffers and front buffers in the renderer
These are swapped out to prevent flickering

So you have a buffer shown on screen, and the back buffer where the new work and drawing is done
*/

/*
- - Movement and Delta Time - -

Move 20 pixels per second
There are 45-58 frames per second

On the screen, how much should a pixel move per frame?

DeltaTime = time between frames ... usually a fraction of a second

So move a fraction of 20 pixels per fraction of a second


Ensures you're moving 20 pixels per second

*/

/*

-- While Loops --

While loops are processor instructions and become part of executable

A CPU core is a CPU's processor
One core can work on one task while an different core works on another

While loops can use a lot of CPU resources - potentially 100% of CPU core

A process needs to share resources with other tasks


-- SDL_Delay(x) --

Tell the operating system to give attention to other processes for x milliseconds

*/

/*

DOUBLE BUFFERING
The idea of having both a back and a front buffer. The back buffer renders while the front buffer shows information

1. Clear the back buffer
2. Draw all game objects
3. Swap front and back buffers


-- Color Buffer --

A bitmap (grid array of bits) that lists each pixel on-screen and its color

*/