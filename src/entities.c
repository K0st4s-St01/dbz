#include "../include/entities.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <string.h>

#define ANIMATION_LENGTH 4

Stat stat_init(int max){
    Stat stat;
    stat.current=max;
    stat.max=max;
    return stat;
}

int max(int a,int b){
    if (a>b) {
        return a;
    }else {
        return b;
    }
}
int min(int a ,int b){
    if (a<b) {
        return a;
    }else {
        return b;
    }
}

void stat_add(Stat *stat,int amount){
    stat->current = max(stat->current+amount, 0);
    stat->current = min(stat->current, stat->max);
}


Animation* animation_init(State when_to_play,SDL_Texture** textures,int textures_length,int current_index){
    Animation* animation = (Animation*)malloc(sizeof(Animation));
    animation->when_to_play=when_to_play;
    animation->textures=textures;
    animation->textures_length=textures_length;
    animation->current_index=0;
    return animation;
}
void animation_tick(Animation *animation){
    animation->current_index=(animation->current_index+1) % animation->textures_length;
}
void animation_render(Animation *animation, SDL_Renderer *renderer, Position position,Position *player,int size){
    if (player->z == position.z) {
        SDL_Rect rect = (SDL_Rect){.x=position.x-player->x+400,.y=position.y-player->y+400,.w=size,.h=size};
        SDL_RenderCopy(renderer, animation->textures[animation->current_index], NULL,&rect );
    }
}
void animation_player_render(Animation *animation, SDL_Renderer *renderer,int size){
        SDL_Rect rect = (SDL_Rect){.x=400,.y=400,.w=size,.h=size};
        SDL_RenderCopy(renderer, animation->textures[animation->current_index], NULL,&rect );
}
void animation_free(Animation *animation){
    for (int i=0; i<animation->textures_length; i++) {
        free(animation->textures[i]);
    }
    free(animation);
}


Entity* entity_init(Position position,char* name,Species species,Animation** animations,int meele_power_level,int ki_power_level,int endurance_power_level){
    Entity* entity=(Entity*)malloc(sizeof(Entity));
    entity->name = name;
    entity->position = position;
    entity->species = species;
    entity->meele_power_level=meele_power_level;
    entity->ki_power_level=ki_power_level;
    entity->endurance_power_level = endurance_power_level;
    entity->health=stat_init(endurance_power_level*10);
    entity->ki=stat_init(ki_power_level*10);
    entity->animations = animations;
    entity->animation_length = ANIMATION_LENGTH;
    return entity;
}
int entity_compute_power_level(Entity *entity){
    return entity->meele_power_level+entity->ki_power_level+entity->endurance_power_level;
}
void entity_meele_attack(Entity *attacker, Entity *defender){
    stat_add(&(defender->health), -attacker->meele_power_level);
}
void entity_ki_attack(Entity *attacker, Entity *defender,int ki_amount){
    if (attacker->ki.current>ki_amount) {
        attacker->ki.current-=ki_amount;
        stat_add(&(defender->health), -attacker->ki_power_level);
    }
}
void entity_getStronger(Entity *entity, int meele, int ki, int endurance){
    entity->meele_power_level+=meele;
    entity->ki_power_level+=ki;
    entity->endurance_power_level+=endurance;
}
void entity_tick(Entity *entity){

}
void entity_render(Entity *entity, SDL_Renderer *renderer,int size,Entity *player){
    switch (entity->state) {

    case IDLE:
        animation_render(entity->animations[IDLE],renderer,entity->position,&player->position,size);
        break;
    case MEELE:
        animation_render(entity->animations[MEELE],renderer,entity->position,&player->position,size);
        break;
    case KI_BLAST:
        animation_render(entity->animations[KI_BLAST],renderer,entity->position,&player->position,size);
        break;
    case CHARGING:
        animation_render(entity->animations[CHARGING],renderer,entity->position,&player->position,size);
      break;
    }
}
void entity_free(Entity *entity){
    for (int i; i<ANIMATION_LENGTH; i++) {
        animation_free(entity->animations[i]);
    }
    free(entity);
}

EntityList* entity_list_init(Entity* entity){
    EntityList* entity_list = (EntityList*)malloc(sizeof(EntityList));
    entity_list->entity = entity;
    entity_list->next = NULL;
    return entity_list;
}
void entity_list_add(EntityList *list, Entity *entity){
    EntityList* current = list;
    while(current->next != NULL){
        current=current->next;
    }
    current->next = entity_list_init(entity);
}
void entity_list_remove_dead(EntityList *list){
    EntityList* current = list;
    EntityList* prev = NULL;

    while (current != NULL) {
        if (current->entity->health.current==0) {
            if (prev == NULL) {
                EntityList* temp = current;
                current=current->next;
                entity_free(temp->entity);
                free(temp);
            }else{
                prev->next = current->next;
                entity_free(current->entity);
                free(current);
                current = prev->next;
            }
        }else{
            prev = current;
            current = current->next;
        }
    }
}

void entity_list_free(EntityList *list){
   EntityList *current = list;
    while (current != NULL) {
        EntityList *next = current->next;
        entity_free(current->entity); 
        free(current); 
        current = next; 
    } 
}
void entity_list_render(EntityList* list,SDL_Renderer *renderer, int size, Entity *player){
   EntityList *current = list;
    while (current != NULL) {
        if(current->entity!=NULL){
            entity_render(current->entity, renderer, size, player);
        }
        current=current->next;
    } 
}
void entity_list_tick(EntityList *list){
   EntityList *current = list;
    while (current != NULL) {
        if(current->entity!=NULL){
            entity_tick(current->entity);
        }
        current=current->next;
    } 
}