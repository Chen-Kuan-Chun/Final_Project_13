/* ============================================================
   damage.c
   ------------------------------------------------------------ */
   #include <stdio.h>
   #include "damage.h"
   #include "boss_1.h"
   #include <stdlib.h>                    /* for atoi */
   #include "../scene/sceneManager.h"     /* extern Scene *scene */
   #include "../scene/gamescene.h"        /* label enum */
   #include <allegro5/allegro_primitives.h>
   
   /* Create the numeric input box */
   Elements *New_DamageInput(int label)
   {
       DamageInput *pDerivedObj = (DamageInput*)malloc(sizeof(DamageInput));
       Elements    *pObj        = New_Elements(label);
   
       pDerivedObj->x = 20;
       pDerivedObj->y = 20;
       pDerivedObj->w = 200;
       pDerivedObj->h = 40;
       pDerivedObj->len = 0;
       pDerivedObj->buf[0] = '\0';
       pDerivedObj->font = al_create_builtin_font();
       pDerivedObj->active = true;
   
       pObj->pDerivedObj = pDerivedObj;
       pObj->Draw     = DamageInput_draw;
       pObj->Update   = DamageInput_update;
       pObj->Interact = DamageInput_interact;
       pObj->Destroy  = DamageInput_destroy;
   
       return pObj;
   }
   
   /* collect keystrokes (very naive – repeats on key‑hold) */
   void DamageInput_update(Elements *self)
   {
       DamageInput *di = (DamageInput*)self->pDerivedObj;
   
       /* digits 0‑9 */
       for (int k = ALLEGRO_KEY_0; k <= ALLEGRO_KEY_9; k++) {
           if (key_state[k] && di->len < 15) {
               di->buf[di->len++] = '0' + (k - ALLEGRO_KEY_0);
               di->buf[di->len]   = '\0';
           }
       }
       /* backspace */
       if (key_state[ALLEGRO_KEY_BACKSPACE] && di->len > 0) {
           di->buf[--di->len] = '\0';
       }
   }
   
   /* on ENTER, apply dmg to every boss_1 */
   void DamageInput_interact(Elements *self)
   {
       DamageInput *di = (DamageInput*)self->pDerivedObj;
       if (key_state[ALLEGRO_KEY_ENTER] && di->len > 0) {
           int dmg = atoi(di->buf);
           di->len = 0;
           di->buf[0] = '\0';
   
           /* broadcast to all Boss1 instances */
           ElementVec bosses = _Get_label_elements(scene, Boss1_L);
           for (int i = 0; i < bosses.len; i++) {
               Boss1_take_damage(bosses.arr[i], dmg);
           }
       }
   }
   
   void DamageInput_draw(Elements *self)
   {
       DamageInput *di = (DamageInput*)self->pDerivedObj;
       ALLEGRO_COLOR border = al_map_rgb(255,255,255);
       ALLEGRO_COLOR bg     = al_map_rgba(0,0,0,200);
   
       al_draw_filled_rectangle(di->x, di->y, di->x + di->w, di->y + di->h, bg);
       al_draw_rectangle(di->x, di->y, di->x + di->w, di->y + di->h, border, 2);
       al_draw_text(di->font, border, di->x + 8, di->y + 10, 0, di->buf);
   }
   
   void DamageInput_destroy(Elements *self)
   {
       DamageInput *di = (DamageInput*)self->pDerivedObj;
       al_destroy_font(di->font);
       free(di);
       free(self);
   }