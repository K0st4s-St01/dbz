#ifndef ENTITIES_H
#define ENTITIES_H


#include "position.h"
#include <SDL2/SDL_render.h>
#include <stdlib.h>



typedef struct {
    int max;
    int current;
} Stat;

Stat stat_init(int max);
void stat_add(Stat *stat,int amount);

typedef enum Species{
    SAIYAN,
    HUMAN,
    NAMEKIAN,
    FROSTDEMON
} Species;


typedef enum State{
    IDLE,
    MEELE,
    KI_BLAST,
    CHARGING,
} State;

typedef struct{
    State when_to_play;
    SDL_Texture** textures;
    int textures_length;
    int current_index;
}Animation;

Animation* animation_init(State when_to_play,SDL_Texture** textures,int textures_length,int current_index);
void animation_tick(Animation* animation);
void animation_render(Animation* animation,SDL_Renderer* renderer,Position position,Position* player,int size);
void animation_player_render(Animation* animation,SDL_Renderer* renderer,int size);
void animation_free(Animation* animation);

typedef struct{
    Position position;
    
    char* name;
    Species species;

    int meele_power_level;
    int ki_power_level;
    int endurance_power_level;
    Stat health;
    Stat ki;
    Animation **animations;
    int animation_length;
    State state;


}Entity;


Entity *entity_init(Position position,char* name,Species species,Animation* animations[4],int meele_power_level,int ki_power_level,int endurance_power_level);
int entity_compute_power_level(Entity *entity);
void entity_meele_attack(Entity *attacker,Entity *defender);
void entity_ki_attack(Entity *attacker,Entity *defender,int ki_amount);
void entity_getStronger(Entity *entity,int meele,int ki,int endurance);
void entity_tick(Entity *entity);
void entity_render(Entity *entity, SDL_Renderer *renderer,int size,Entity *player);
void player_render(Entity *player,SDL_Renderer *renderer);
void entity_free(Entity *entity);



typedef struct EntityList{
    Entity* entity;
    struct EntityList* next;
} EntityList ;

EntityList* entity_list_init(Entity* entity);
void entity_list_remove_dead(EntityList* list);
void entity_list_add(EntityList* list,Entity* entity);
void entity_list_free(EntityList* list);
void entity_list_render(EntityList* list,SDL_Renderer *renderer,int size,Entity* player);
void entity_list_tick(EntityList* list);


#endif