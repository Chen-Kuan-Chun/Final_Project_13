#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "card.h"
#include <stdbool.h>
/*
   [Card function]
*/
Scene *New_Card(int label)
{
    Card *pDerivedObj = (Card *)malloc(sizeof(Card));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->cardback = al_load_bitmap("assets/image/cardback.JPG");
    pDerivedObj->card = al_load_bitmap("assets/image/card.PNG");
    pDerivedObj->card_x = 200;
    pDerivedObj->card_y = 0;
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = card_update;
    pObj->Draw = card_draw;
    pObj->Destroy = card_destroy;
    return pObj;
}
void card_update(Scene *self)
{
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    Card *Obj = ((Card *)(self->pDerivedObj));
    if(mouse_state.x >= 350 && mouse_state.x <= 550 && mouse_state.y >= Obj->card_y + 100 && mouse_state.y <= Obj->card_y + 300 && mouse_state.buttons){
        Obj->card_y = mouse_state.y - 150;
        if(Obj->card_y > HEIGHT - 300){
            // 播動畫
            self->scene_end = true;
            window = 0;
        }
    }
    return;
}
void card_draw(Scene *self)
{
    Card *Obj = ((Card *)(self->pDerivedObj));
        al_draw_scaled_bitmap(
        Obj->cardback,
        0, 0, al_get_bitmap_width(Obj->cardback), al_get_bitmap_height(Obj->cardback),
        200, 0, 500, HEIGHT,
        0
    );
    al_draw_scaled_bitmap(
        Obj->card,
        0, 0, al_get_bitmap_width(Obj->card), al_get_bitmap_height(Obj->card),
        Obj->card_x, Obj->card_y, 500, HEIGHT/2,
        0
    );
}
void card_destroy(Scene *self)
{
    Card *Obj = ((Card *)(self->pDerivedObj));
    free(Obj);
    free(self);
}
