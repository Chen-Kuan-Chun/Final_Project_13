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
    pDerivedObj->page = 0;
    pDerivedObj->intro_1 = al_load_bitmap("assets/image/intro1.PNG");
    pDerivedObj->intro_2 = al_load_bitmap("assets/image/intro2.PNG");
    pDerivedObj->intro_3 = al_load_bitmap("assets/image/intro3.PNG");
    pDerivedObj->intro_4 = al_load_bitmap("assets/image/intro4.PNG");
    pDerivedObj->intro_5 = al_load_bitmap("assets/image/intro5.PNG");
    pDerivedObj->intro_6 = al_load_bitmap("assets/image/intro6.PNG");
    // setting derived object function
    pObj->Update = about_update;
    pObj->Draw = about_draw;
    pObj->Destroy = about_destroy;
    return pObj;
}
void about_update(Scene *self)
{
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);

    About *Obj = ((About *)(self->pDerivedObj));

    // 左鍵切換頁面
    if (al_key_down(&key_state, ALLEGRO_KEY_LEFT)) {
        if (Obj->page > 0) {
            Obj->page--;
            al_rest(0.2);  // 防止一次按太快切多頁
        }
    }
    // 右鍵切換頁面
    else if (al_key_down(&key_state, ALLEGRO_KEY_RIGHT)) {
        if (Obj->page < 5) {
            Obj->page++;
            al_rest(0.2);  // 防止一次按太快切多頁
        }
    }

    // 按 ESC 回主選單
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
        self->scene_end = true;
        window = 0; // 假設 0 是主選單
    }
}

void about_draw(Scene *self)
{
    About *Obj = ((About *)(self->pDerivedObj));
    if(Obj->page == 0){
        al_draw_bitmap(Obj->intro_1, 0, 60, 0);
    }else if(Obj->page == 1){
        al_draw_bitmap(Obj->intro_2, 0, 60, 0);
    }else if(Obj->page == 2){
        al_draw_bitmap(Obj->intro_3, 0, 60, 0);
    }else if(Obj->page == 3){
        al_draw_bitmap(Obj->intro_4, 0, 60, 0);
    }else if(Obj->page == 4){
        al_draw_bitmap(Obj->intro_5, 0, 60, 0);
    }else if(Obj->page == 5){
        al_draw_bitmap(Obj->intro_6, 0, 60, 0);
    }

}
void about_destroy(Scene *self)
{
    About *Obj = ((About *)(self->pDerivedObj));
    free(Obj);
    free(self);
}
