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
    pDerivedObj->x = 85;
    pDerivedObj->y = HEIGHT - pDerivedObj->height;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
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
}
void Draw_destory(Elements *self)
{
    Draw *Obj = ((Draw *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
