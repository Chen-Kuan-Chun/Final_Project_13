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
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
    ALLEGRO_BITMAP *role1;
    ALLEGRO_BITMAP *role2;
} Draw;
Elements *New_Draw(int label);
void Draw_update(Elements *self);
void Draw_interact(Elements *self);
void Draw_draw(Elements *self);
void Draw_destory(Elements *self);

#endif
