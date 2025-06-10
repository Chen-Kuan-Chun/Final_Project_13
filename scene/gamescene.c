#include <allegro5/allegro_audio.h>
#include "gamescene.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/bead.h"
#include "../element/boss_1.h"
#include "../element/damage.h"
#include "../element/black_line.h"
#include "../element/count_points.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Shape.h"
#include "../element/draw.h"
#include "../element/boss.h"
/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/stage.jpg");  
    pDerivedObj->background1 = al_load_bitmap("assets/image/background1.png");     
    pDerivedObj->background2 = al_load_bitmap("assets/image/background2.png");
    pDerivedObj->back = al_load_bitmap("assets/image/back.png");        
    pDerivedObj->font1 = al_load_ttf_font("assets/font/DIN Condensed Bold.ttf", 28, 0);
    pDerivedObj->mouse_back = false;
    pObj->pDerivedObj = pDerivedObj;

    al_reserve_samples(20);
    pDerivedObj->song = al_load_sample("assets/sound/bgm.wav");
    if (!pDerivedObj->song) {
        fprintf(stderr, "Failed to load assets/sound/bgm.wav\n");
    }
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    // register element
    //_Register_elements(pObj, New_Floor(Floor_L));
    //_Register_elements(pObj, New_Teleport(Teleport_L));
    //_Register_elements(pObj, New_Tree(Tree_L));
    //_Register_elements(pObj, New_Character(Character_L));
    Generate_Bead_Grid(pObj);
    // _Register_elements(pObj, New_Boss1(Boss1_L));
    // _Register_elements(pObj, New_DamageInput(DamageInput_L));
    // _Register_elements(pObj, New_BlackLine(BlackLine_L));
    // _Register_elements(pObj, New_MatchManager(MatchMgr_L));
    _Register_elements(pObj, New_Draw(Draw_L));
    _Register_elements(pObj, New_Boss(Boss_L));
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    OVER = false;
    return pObj;
}



void game_scene_update(Scene *self)
{
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);
    // update every element
    gs->mouse_back = (mouse_state.x >= 800 && mouse_state.x <= 900 &&
                                mouse_state.y >= 50 && mouse_state.y <= 150);
    if (mouse_state.buttons & 1) {
        if (gs->mouse_back) {
            self->scene_end = true;
            ROUND = 1;
            RECOVER = 0;
            window = 0;
        }
    }
    if (al_key_down(&key_state, ALLEGRO_KEY_N)) {
        COUNT += 50;
        al_rest(0.2);
    }
    if (COUNT >= 150 || al_key_down(&key_state, ALLEGRO_KEY_V)) {
        self->scene_end = true;
        window = 4;
    }
    if (OVER || al_key_down(&key_state, ALLEGRO_KEY_D)) {
        self->scene_end = true;
        window = 5;
    }
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele);
    }
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Interact(ele);
    }
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
}
void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    if(COUNT >= 50){
        al_draw_bitmap(gs->background2, 0, 0, 0);
    }else{
        al_draw_bitmap(gs->background1, 0, 0, 0);
    }
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
    al_draw_text(gs->font1, al_map_rgb(255, 255, 255), 80, 100, ALLEGRO_ALIGN_LEFT, "R O U N D :");
    snprintf(gs->round_str, sizeof(gs->round_str), "%d", ROUND);
    al_draw_text(gs->font1, al_map_rgb(255, 255, 255), 170, 100, ALLEGRO_ALIGN_LEFT, gs->round_str);
    snprintf(gs->boss_round_str, sizeof(gs->round_str), "%d", 3 - (ROUND-1)%3);
    al_draw_text(gs->font1, al_map_rgb(255, 0, 0), 300, 100, ALLEGRO_ALIGN_LEFT, gs->boss_round_str);
    al_draw_text(gs->font1, al_map_rgb(255, 0, 0), 270, 100, ALLEGRO_ALIGN_LEFT, "CD");
    al_draw_scaled_bitmap(
        gs->back,
        0, 0, al_get_bitmap_width(gs->back), al_get_bitmap_height(gs->back),
        800, 50, 100, 100,
        0
    );
    al_play_sample_instance(gs->sample_instance);
}
void game_scene_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
