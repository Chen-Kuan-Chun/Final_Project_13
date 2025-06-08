#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"

/*
   [game scene object]
*/
typedef enum EleType
{
    Floor_L,
    Teleport_L,
    Tree_L,
    Character_L,
    Projectile_L,
    Bead_L,
    Boss1_L,
    DamageInput_L,
    BlackLine_L,
    MatchMgr_L
} EleType;
typedef struct _GameScene
{
    ALLEGRO_BITMAP *background;
    bool bead_stop;

} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);

#endif
