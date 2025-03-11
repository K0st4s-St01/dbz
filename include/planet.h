#ifndef PLANETS_H
#define PLANETS_H

#include "entities.h"
#include "world.h"
typedef struct{
    TileMap* tilemap;
    Entity* player;
    int seed;
    EntityList* entities;
}Planet ;


void planet_init(Species species);
void planet_render(Planet* planet,SDL_Renderer* renderer,int size);
void planet_tick(Planet* planet);
void planet_free(Planet* planet);


typedef struct{
    int seed;
}
Generator ;


#endif