#include "bead.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Shape.h"
#include "../global.h"
#include <allegro5/allegro_primitives.h>
#include "../scene/sceneManager.h"
/*
   [Bead function]
*/
Elements *New_Bead(int label, int col, int row)
{
    Bead *pDerivedObj = (Bead *)malloc(sizeof(Bead));
    Elements *pObj = New_Elements(label);

    // setting derived object member
    pDerivedObj->x = col * 72 + 10;
    pDerivedObj->y = row * 72;
    pDerivedObj->w = 67;
    pDerivedObj->h = 67;
    pDerivedObj->c = al_map_rgb(255, 255, 255);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Bead_update;
    pObj->Interact = Bead_interact;
    pObj->Draw = Bead_draw;
    pObj->Destroy = Bead_destory;
    return pObj;
}
void Bead_update(Elements *self) {
}
void Bead_interact(Elements *self){
}
void Bead_draw(Elements *self)
{
    Bead *Obj = ((Bead *)(self->pDerivedObj));
    al_draw_filled_rectangle(Obj->x, Obj->y, Obj->x + Obj->w, Obj->y + Obj->h, Obj->c);
}
void Bead_destory(Elements *self)
{
    Bead *Obj = ((Bead *)(self->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(self);
}