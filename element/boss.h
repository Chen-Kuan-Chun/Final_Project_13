#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
/*
   [tree object]
*/
typedef struct _Boss
{
    ALLEGRO_BITMAP *boss1;
    ALLEGRO_BITMAP *boss2_1;
    ALLEGRO_BITMAP *boss2_2;
} Boss;
Elements *New_Boss(int label);
void Boss_update(Elements *self);
void Boss_interact(Elements *self);
void Boss_draw(Elements *self);
void Boss_destory(Elements *self);

#endif
