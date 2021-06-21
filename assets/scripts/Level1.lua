Level1 = {
    assets = {
        [0] = {type = "texture", id = "plains-texture", file = "./assets/tilemaps/Plains.png"},
        [1] = {type = "texture", id = "pegasus-texture", file = "./assets/images/PegasusSpriteSheet.png"},
        [2] = {type = "texture", id = "archer-texture", file = "./assets/images/Archer.png"},
        [3] = {type = "texture", id = "projectile-texture", file = "./assets/images/enemyProjectile.png"},
        [4] = {type = "font", id = "charriot-font", file = "./assets/fonts/charriot.ttf", fontSize = 14}
    },
    map = {
        textureAssetId = "plains-texture",
        file = "./assets/tilemaps/saved.map",
        scale = 2,
        tileSize = 32,
        mapSizeX = 25,
        mapSizeY = 20
    },
    entities = {
        [0] = {
            name = "player",
            layer = 3,
            copies = 1,
            components = {
                transform = {
                    position = {
                        x = 240,
                        y = 106
                    },
                    velocity = {
                        x = 0,
                        y = 0
                    },
                    width = 32,
                    height = 32,
                    scale = 2,
                    rotation = 0
                },
                sprite = {
                    textureAssetId = "pegasus-texture",
                    animated = true,
                    frameCount = 4,
                    animationSpeed = 90,
                    fixed = false,
                    hasDirections = true
                },
                collider = {
                    tag = "PLAYER",
                    modified = 1,
                    deltaX = 16,
                    deltaY = 16,
                    deltaW = -32,
                    deltaH = -32
                },
                input = {
                    keyboard = {
                        up = "w",
                        left = "a",
                        down = "s",
                        right = "d",
                        shoot = "space"
                    }
                }
            }
        },
        [1] = {
            name = "archer-enemy",
            layer = 2,
            copies = 25,
            components = {
                transform = {
                    position = {
                        x = 850,
                        y = 805
                    },
                    velocity = {
                        x = 40,
                        y = 40
                    },
                    width = 16,
                    height = 16,
                    scale = 2,
                    rotation = 0
                },
                sprite = {
                    textureAssetId = "archer-texture",
                    animated = true,
                    frameCount = 4,
                    animationSpeed = 120,
                    fixed = false,
                    hasDirections = true
                },
                collider = {
                    tag = "ENEMY",
                    modified = 0
                },
                projectileEmitter = {
                    textureAssetId = "projectile-texture",
                    width = 4,
                    height = 4,
                    speed = 160,
                    range = 600,
                    shouldLoop = 1
                },
                AI = {
                    speed = 40
                }
            }
        }
    },
    waypoints = {
        count = 30,
        minDomain = 32,
        maxDomain = 1536,
        minRange = 32,
        maxRange = 1216,
        tag = "WAYPOINT"
    }
}
