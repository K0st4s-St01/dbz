#include "../include/textures.h"

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    // Load image
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == NULL) {
        printf("Unable to create texture from surface! SDL_Error: %s\n", SDL_GetError());
    }

    // Free the loaded surface as we no longer need it
    SDL_FreeSurface(loadedSurface);
    return texture;
}