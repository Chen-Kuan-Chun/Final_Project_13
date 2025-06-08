#ifndef DEFEAT_H_INCLUDED
#define DEFEAT_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include "scene.h"

typedef struct _Defeat {
    ALLEGRO_FONT  *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
} Defeat;

Scene *New_Defeat(int label);
void   defeat_update(Scene*);
void   defeat_draw(Scene*);
void   defeat_destroy(Scene*);
#endif