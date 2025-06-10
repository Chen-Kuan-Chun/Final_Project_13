#ifndef ABOUT_H_INCLUDED
#define ABOUT_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"
/*
   [About object]
*/
typedef struct _About
{
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
    bool mouse_over_new_game, mouse_over_level, mouse_over_about;
    ALLEGRO_BITMAP *intro_1;
    ALLEGRO_BITMAP *intro_2;
    ALLEGRO_BITMAP *intro_3;
    ALLEGRO_BITMAP *intro_4;
    ALLEGRO_BITMAP *intro_5;
    ALLEGRO_BITMAP *intro_6;
    int page;
} About;
Scene *New_About(int label);
void about_update(Scene *self);
void about_draw(Scene *self);
void about_destroy(Scene *self);

#endif
