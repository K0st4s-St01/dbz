#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "entities.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct TileMap{
    Position key;
    int value;
    struct TileMap* left;
    struct TileMap* right;
}TileMap;

TileMap* tilemap_init(Position key,int value);
TileMap* tilemap_insert(TileMap* tilemap,Position key,int value);
void tilemap_render(TileMap* tilemap,SDL_Renderer* renderer,SDL_Texture **tile_sprites,Entity *player,int size);
int tilemap_get(TileMap* tilemap,Position key);
void tilemap_free(TileMap* tilemap);

#endif