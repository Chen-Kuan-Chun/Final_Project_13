#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "yy.h"
#include <stdbool.h>
/*
   [Yy function]
*/
Scene *New_Yy(int label)
{
    Yy *pDerivedObj = (Yy *)malloc(sizeof(Yy));
    Scene *pObj = New_Scene(label);
    pObj->pDerivedObj = pDerivedObj;

    al_reserve_samples(20);
    pDerivedObj->song = al_load_sample("assets/sound/victory.wav");
    if (!pDerivedObj->song) {
        fprintf(stderr, "Failed to load assets/sound/victory.wav\n");
    }
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);

    pDerivedObj->yy = al_load_bitmap("assets/image/yy.JPG");
    // setting derived object function

    pObj->Update = yy_update;
    pObj->Draw = yy_draw;
    pObj->Destroy = yy_destroy;
    return pObj;
}
void yy_update(Scene *self)
{
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);

    // 按 ESC 回主選單
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
        self->scene_end = true;
        window = 0; // 假設 0 是主選單
    }
}

void yy_draw(Scene *self)
{
    Yy *Obj = ((Yy *)(self->pDerivedObj));
    al_draw_scaled_bitmap(
        Obj->yy,
        0, 0, al_get_bitmap_width(Obj->yy), al_get_bitmap_height(Obj->yy),
        0, 0, WIDTH, HEIGHT,
        0
    );
    al_play_sample_instance(Obj->sample_instance);
}
void yy_destroy(Scene *self)
{
    Yy *Obj = ((Yy *)(self->pDerivedObj));
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
