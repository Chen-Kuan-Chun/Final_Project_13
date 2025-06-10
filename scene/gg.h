#ifndef GG_H_INCLUDED
#define GG_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"
/*
   [Gg object]
*/
typedef struct _Gg
{
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
    bool mouse_over_new_game, mouse_over_level, mouse_over_about;
    ALLEGRO_BITMAP *gg;
} Gg;
Scene *New_Gg(int label);
void gg_update(Scene *self);
void gg_draw(Scene *self);
void gg_destroy(Scene *self);

#endif
