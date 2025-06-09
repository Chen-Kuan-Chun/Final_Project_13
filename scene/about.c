#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "about.h"
#include <stdbool.h>
/*
   [About function]
*/
Scene *New_About(int label)
{
    About *pDerivedObj = (About *)malloc(sizeof(About));
    Scene *pObj = New_Scene(label);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = about_update;
    pObj->Draw = about_draw;
    pObj->Destroy = about_destroy;
    return pObj;
}
void about_update(Scene *self)
{
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    About *Obj = ((About *)(self->pDerivedObj));
}
void about_draw(Scene *self)
{
    About *Obj = ((About *)(self->pDerivedObj));

}
void about_destroy(Scene *self)
{
    About *Obj = ((About *)(self->pDerivedObj));
    free(Obj);
    free(self);
}
