#include "../include/world.h"
#include <SDL2/SDL_rect.h>


TileMap* tilemap_init(Position key, int value){
    TileMap* result = (TileMap*)malloc(sizeof(TileMap));
    result->key=key;
    result->value=value;
    result->left=NULL;
    result->right=NULL;
    return result; 
}
TileMap* tilemap_insert(TileMap *tilemap, Position key, int value){
    if(tilemap == NULL){
        return tilemap_init(key,value);
    }
    if (position_eq(&key, &(tilemap->key))) {
        return tilemap_init(key,value);
    }
    if (position_smaller(&(tilemap->key), &key)) {
        tilemap->right=tilemap_insert(tilemap->right, key, value);
    }else{
        tilemap->left=tilemap_insert(tilemap->left, key, value);
    }
}
void tilemap_render(TileMap *tilemap, SDL_Renderer *renderer,SDL_Texture **tile_sprites,Entity *player,int size){
    if(tilemap != NULL){
        if (tilemap->key.z == player->position.z) {
            int x = tilemap->key.x*size - player->position.x + 400;
            int y = tilemap->key.y*size - player->position.y + 400;
            SDL_Rect rect = (SDL_Rect){.x=x,.y=y,.w=size,.h=size};

            SDL_RenderCopy(renderer,tile_sprites[tilemap->value],NULL,&rect);
        }
        tilemap_render(tilemap->right, renderer, tile_sprites, player, size);
        tilemap_render(tilemap->left, renderer, tile_sprites, player, size);
    }
}
void tilemap_free(TileMap *tilemap){
    if(tilemap == NULL){
        return;
    }
    tilemap_free(tilemap->left);
    tilemap_free(tilemap->right);

    free(tilemap);
}
int tilemap_get(TileMap *tilemap, Position key){
    if (tilemap == NULL ){
        return -1;
    }else if (position_eq(&key, &(tilemap->key))) {
        return tilemap->value;
    }
    if (position_smaller(&(tilemap->key),&key)) {
        return tilemap_get(tilemap->right,  key);
    }
    return tilemap_get(tilemap->left,  key);
}
