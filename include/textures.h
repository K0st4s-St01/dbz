#ifndef TEXTURES_H
#define TEXTURES_H


#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);

#endif