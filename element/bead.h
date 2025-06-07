#ifndef Bead_H_INCLUDED
#define Bead_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include "../scene/menu.h"
/*
   [Bead object]
*/
typedef struct _Bead
{
    float x, y, w, h;
    ALLEGRO_COLOR c;
    Shape *hitbox;
} Bead;
Elements *New_Bead(int label, int col, int row);
void Bead_update(Elements *self);
void Bead_interact(Elements *self);
void Bead_draw(Elements *self);
void Bead_destory(Elements *self);

#endif
