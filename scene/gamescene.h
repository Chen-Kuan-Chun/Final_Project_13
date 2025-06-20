#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>

/*
   [game scene object]
*/
typedef enum EleType
{
    Floor_L,
    Teleport_L,
    Tree_L,
    Draw_L,
    Character_L,
    Projectile_L,
    Bead_L,
    Boss1_L,
    DamageInput_L,
    BlackLine_L,
    MatchMgr_L,
    Boss_L
} EleType;
typedef struct _GameScene
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *background1;
    ALLEGRO_BITMAP *background2;
    ALLEGRO_BITMAP *back;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    bool mouse_back;
    bool bead_stop;
    ALLEGRO_FONT *font1;
    char round_str[20];
    char boss_round_str[20];

} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);

#endif
