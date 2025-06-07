/* ============================================================
   boss_1.h – sprite boss with numeric HP
   ------------------------------------------------------------ */
   #ifndef BOSS_1_H_INCLUDED
   #define BOSS_1_H_INCLUDED
   #include "element.h"
   #include <allegro5/allegro.h>          /* ALLEGRO_BITMAP */
   #include <stdbool.h>
   
   typedef struct _Boss1 {
       int  x, y;          /* left-top position  */
       int  width, height; /* sprite W/H         */
       int draw_w, draw_h;
       int  hp;
       bool dir;           /* reserve for future flip */
       ALLEGRO_BITMAP *bmp;/* boss1.png sprite   */
   } Boss1;
   
   /* ctor / interface */
   Elements *New_Boss1(int label);
   void      Boss1_update(Elements *self);
   void      Boss1_interact(Elements *self);
   void      Boss1_draw(Elements *self);
   void      Boss1_destroy(Elements *self);
   void      Boss1_take_damage(Elements *self, int dmg);
   
   #endif /* BOSS_1_H_INCLUDED */
   