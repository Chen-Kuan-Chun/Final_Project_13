#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "menu.h"
#include <stdbool.h>
/*
   [Menu function]
*/
Scene *New_Menu(int label)
{
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 18, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    pDerivedObj->mouse_over_new_game = false;
    pDerivedObj->mouse_over_level = false;
    pDerivedObj->mouse_over_about = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene *self)
{
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);

    Menu *Obj = ((Menu *)(self->pDerivedObj));

    Obj->mouse_over_new_game = (mouse_state.x >= Obj->title_x - 150 && mouse_state.x <= Obj->title_x + 150 &&
                                mouse_state.y >= Obj->title_y - 100 && mouse_state.y <= Obj->title_y - 40);

    Obj->mouse_over_level = (mouse_state.x >= Obj->title_x - 150 && mouse_state.x <= Obj->title_x + 150 &&
                             mouse_state.y >= Obj->title_y - 20 && mouse_state.y <= Obj->title_y + 40);

    Obj->mouse_over_about = (mouse_state.x >= Obj->title_x - 150 && mouse_state.x <= Obj->title_x + 150 &&
                             mouse_state.y >= Obj->title_y + 60 && mouse_state.y <= Obj->title_y + 114);


    if (mouse_state.buttons & 1) {
        if (Obj->mouse_over_new_game) {
            self->scene_end = true;
            window = 1;
        }
        else if (Obj->mouse_over_level){
            self->scene_end = true;
            window = 2;
        }
        else if (Obj->mouse_over_about){
            self->scene_end = true;
            window = 3;
        }
    }
    return;
}
void menu_draw(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    ALLEGRO_COLOR new_game_color = Obj->mouse_over_new_game ? al_map_rgb(179, 209, 255) : al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR level_color = Obj->mouse_over_level ? al_map_rgb(179, 209, 255) : al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR about_color = Obj->mouse_over_about ? al_map_rgb(179, 209, 255) : al_map_rgb(255, 255, 255);

    al_draw_text(Obj->font, new_game_color, Obj->title_x, Obj->title_y - 80, ALLEGRO_ALIGN_CENTRE, "START");
    al_draw_rectangle(Obj->title_x - 150, Obj->title_y - 100, Obj->title_x + 150, Obj->title_y - 40, new_game_color, 3);
    al_draw_text(Obj->font, level_color, Obj->title_x, Obj->title_y, ALLEGRO_ALIGN_CENTRE, "CRAD");
    al_draw_rectangle(Obj->title_x - 150, Obj->title_y - 20, Obj->title_x + 150, Obj->title_y + 40, level_color, 3);
    al_draw_text(Obj->font, about_color, Obj->title_x, Obj->title_y + 80, ALLEGRO_ALIGN_CENTRE, "ABOUT");
    al_draw_rectangle(Obj->title_x - 150, Obj->title_y + 60, Obj->title_x + 150, Obj->title_y + 120, about_color, 3);
    al_play_sample_instance(Obj->sample_instance);
}
void menu_destroy(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
