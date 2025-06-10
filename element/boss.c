#include "boss.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
/*
   [boss function]
*/
Elements *New_Boss(int label)
{
    Boss *pDerivedObj = (Boss *)malloc(sizeof(Boss));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->boss1 = al_load_bitmap("assets/image/boss1.png");
    pDerivedObj->boss2_1 = al_load_bitmap("assets/image/BOSS2-1.png");
    pDerivedObj->boss2_2 = al_load_bitmap("assets/image/BOSS2-2.png");
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Boss_update;
    pObj->Interact = Boss_interact;
    pObj->Draw = Boss_draw;
    pObj->Destroy = Boss_destory;
    return pObj;
}
void Boss_update(Elements *self) {}
void Boss_interact(Elements *self) {}
void Boss_draw(Elements *self)
{
    Boss *obj = ((Boss *)(self->pDerivedObj));
    if(COUNT < 50){
        al_draw_scaled_bitmap(
        obj->boss1,
        0, 0, al_get_bitmap_width(obj->boss1), al_get_bitmap_height(obj->boss1),
        310, 0, 250, 250,
        0
        );
        al_draw_filled_rectangle(240, 10, 640 - (COUNT)*8, 20, al_map_rgb(255, 0, 0));
    }else if(COUNT >= 50 && COUNT <100){
        al_draw_scaled_bitmap(
        obj->boss2_1,
        0, 0, al_get_bitmap_width(obj->boss2_1), al_get_bitmap_height(obj->boss2_1),
        310, 0, 250, 250,
        0
        );
        al_draw_filled_rectangle(240, 10, 640 - (COUNT - 50)*8, 20, al_map_rgb(255, 0, 0));
    }else if(COUNT >= 100 && COUNT <150){
        al_draw_scaled_bitmap(
        obj->boss2_2,
        0, 0, al_get_bitmap_width(obj->boss2_2), al_get_bitmap_height(obj->boss2_2),
        310, 0, 250, 250,
        0
        );
        al_draw_filled_rectangle(240, 10, 640 - (COUNT - 100)*8, 20, al_map_rgb(255, 0, 0));
    }
}
void Boss_destory(Elements *self)
{
    Boss *Obj = ((Boss *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->boss1);
    al_destroy_bitmap(Obj->boss2_1);
    al_destroy_bitmap(Obj->boss2_2);
    free(Obj);
    free(self);
}
