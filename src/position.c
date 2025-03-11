#include "../include/position.h"

Position position_init(int x,int y,int z,int w,int h){
    return (Position){.x=x,.y=y,.z=z,.w=w,.h=h};
}
bool position_colision(Position *a, Position *b){
    return (a->x < b->x + b->w && 
            a->x + a->w > b->x && 
            a->y < b->y + b->h && 
            a->y + a->h > b->y);
}
Position position_add(Position *a, Position *b){
    Position sum;
    sum.x = a->x + b->x;
    sum.y = a->y + b->y;
    sum.z = a->z + b->z;
    sum.w = a->w + b->w;
    sum.h = a->h + b->h;
    return sum;
}
bool position_smaller(Position *current, Position *other){
    if(current->z == other->z){
        if (current->y == other->y) {
            return current->x < other->x;
        }else{
            return current->y < other->y;
        }
    }else{
        return current->z < other->z;
    }
}
bool position_eq(Position *a, Position *b){
    return a->x == b->x 
    && a->y == b->y 
    && a->z == b->z 
    && a->w == b->w 
    && a->h == b->h;
}
