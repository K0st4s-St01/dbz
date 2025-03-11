#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>

typedef struct{
    int x,y,z;
    int w,h;
}Position;

Position position_init(int x,int y,int z,int w,int h);
bool position_colision(Position* a,Position* b);
Position position_add(Position*a,Position* b);
bool position_smaller(Position* current,Position* other);
bool position_eq(Position* a ,Position* b);
#endif