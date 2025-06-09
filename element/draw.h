#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
/*
   [tree object]
*/
typedef struct _Draw
{
    ALLEGRO_BITMAP *role1;
    ALLEGRO_BITMAP *role2;
    ALLEGRO_BITMAP *heart;
    ALLEGRO_BITMAP *time;
} Draw;
Elements *New_Draw(int label);
void Draw_update(Elements *self);
void Draw_interact(Elements *self);
void Draw_draw(Elements *self);
void Draw_destory(Elements *self);

#endif
