#ifndef YY_H_INCLUDED
#define YY_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"
/*
   [Yy object]
*/
typedef struct _Yy
{
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
    bool mouse_over_new_game, mouse_over_level, mouse_over_about;
    ALLEGRO_BITMAP *yy;
} Yy;
Scene *New_Yy(int label);
void yy_update(Scene *self);
void yy_draw(Scene *self);
void yy_destroy(Scene *self);

#endif
