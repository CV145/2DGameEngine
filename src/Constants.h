#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

//Universal constants

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const unsigned int targetFPS = 60;

//time it takes for a single frame to go through? 16.x milliseconds per frame
const unsigned int FRAME_TARGET_TIME = 1000 / targetFPS;

enum CollisionType
{
    NO_COLLISION,
    PLAYER_ENEMY_COLLISION,
    PLAYER_PROJECTILE_COLLISION,
    ENEMY_PROJECTILE_COLLISION,
    ENEMY_WAYPOINT_COLLISION,
    PLAYER_VEGETATION_COLLISION,
    PLAYER_LEVEL_COMPLETE_COLLISION
};

//There are layers for tiles, players, enemies, UI, projectiles, etc... Each entity will belong to a layer
enum LayerType
{
    TILEMAP_LAYER = 0,
    VEGETATION_LAYER = 1,
    ENEMY_LAYER = 2,
    PLAYER_LAYER = 3,
    PROJECTILE_LAYER = 4,
    UI_LAYER = 5,
    OBSTACLE_LAYER = 6,
    WAYPOINT_LAYER = 7
};

const unsigned int NUM_LAYERS = 8;

const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color GREEN_COLOR = {0, 255, 0, 255};

#endif
