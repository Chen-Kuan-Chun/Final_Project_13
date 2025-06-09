#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"
/*
   [Card object]
*/
typedef struct _Card
{    
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;
    int card_x, card_y;
    bool mouse_over_new_game, mouse_over_level, mouse_over_about;
    ALLEGRO_BITMAP *card;
    ALLEGRO_BITMAP *cardback;
} Card;
Scene *New_Card(int label);
void card_update(Scene *self);
void card_draw(Scene *self);
void card_destroy(Scene *self);

#endif
