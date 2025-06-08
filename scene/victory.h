#ifndef VICTORY_H_INCLUDED
#define VICTORY_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include "scene.h"

typedef struct _Victory {
    ALLEGRO_FONT  *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
} Victory;

Scene *New_Victory(int label);
void   victory_update(Scene*);
void   victory_draw(Scene*);
void   victory_destroy(Scene*);
#endif