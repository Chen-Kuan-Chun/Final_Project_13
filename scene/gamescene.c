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
/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/stage.jpg");       // 5秒限制
    pDerivedObj->font1 = al_load_ttf_font("assets/font/DIN Condensed Bold.ttf", 28, 0);
    pObj->pDerivedObj = pDerivedObj;
    // register element
    //_Register_elements(pObj, New_Floor(Floor_L));
    //_Register_elements(pObj, New_Teleport(Teleport_L));
    //_Register_elements(pObj, New_Tree(Tree_L));
    //_Register_elements(pObj, New_Character(Character_L));
    Generate_Bead_Grid(pObj);
    _Register_elements(pObj, New_Boss1(Boss1_L));
    _Register_elements(pObj, New_DamageInput(DamageInput_L));
    _Register_elements(pObj, New_BlackLine(BlackLine_L));
    _Register_elements(pObj, New_MatchManager(MatchMgr_L));
    _Register_elements(pObj, New_Draw(Draw_L));

    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}



void game_scene_update(Scene *self)
{
    // update every element
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
    al_draw_bitmap(gs->background, 0, 0, 0);
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
    al_draw_text(gs->font1, al_map_rgb(255, 0, 0), 500, 200, ALLEGRO_ALIGN_LEFT, gs->boss_round_str);
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
    free(Obj);
    free(self);
}
