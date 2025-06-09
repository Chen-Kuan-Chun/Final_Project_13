#include "draw.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
/*
   [draw function]
*/
Elements *New_Draw(int label)
{
    Draw *pDerivedObj = (Draw *)malloc(sizeof(Draw));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->role1 = al_load_bitmap("assets/image/role1.png");
    pDerivedObj->role2 = al_load_bitmap("assets/image/role2.png");
    pDerivedObj->heart = al_load_bitmap("assets/image/heart.png");
    pDerivedObj->time = al_load_bitmap("assets/image/time.png");
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Draw_update;
    pObj->Interact = Draw_interact;
    pObj->Draw = Draw_draw;
    pObj->Destroy = Draw_destory;
    return pObj;
}
void Draw_update(Elements *self) {}
void Draw_interact(Elements *self) {}
void Draw_draw(Elements *self)
{
    Draw *obj = ((Draw *)(self->pDerivedObj));
    al_draw_scaled_bitmap(
        obj->role1,
        0, 0, al_get_bitmap_width(obj->role1), al_get_bitmap_height(obj->role1),
        240, 225, 50, 50,
        0
    );
    al_draw_scaled_bitmap(
        obj->role2,
        0, 0, al_get_bitmap_width(obj->role2), al_get_bitmap_height(obj->role2),
        315, 225, 50, 50,
        0
    );
    al_draw_scaled_bitmap(
        obj->heart,
        0, 0, al_get_bitmap_width(obj->heart), al_get_bitmap_height(obj->heart),
        210, 290, 425, 15,
        0
    );
     al_draw_scaled_bitmap(
        obj->time,
        0, 0, al_get_bitmap_width(obj->time), al_get_bitmap_height(obj->time),
        210, 275, 425, 15,
        0
    );
}
void Draw_destory(Elements *self)
{
    Draw *Obj = ((Draw *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->role1);
    al_destroy_bitmap(Obj->role2);
    al_destroy_bitmap(Obj->heart);
    free(Obj);
    free(self);
}
