/* ============================================================
   damage.h – a tiny on‑screen numeric input box
   ------------------------------------------------------------ */
   #ifndef DAMAGE_H_INCLUDED
   #define DAMAGE_H_INCLUDED
   #include "element.h"
   #include <allegro5/allegro_font.h>
   #include <allegro5/allegro_ttf.h>
   #include <stdbool.h>
   
   typedef struct _DamageInput {
       char  buf[16];   /* accumulated characters */
       int   len;       /* current length         */
       bool  active;    /* always true in this simple version */
       ALLEGRO_FONT *font;
       int   x, y, w, h; /* display rectangle */
   } DamageInput;
   
   Elements *New_DamageInput(int label);
   void      DamageInput_update(Elements *self);
   void      DamageInput_interact(Elements *self);
   void      DamageInput_draw(Elements *self);
   void      DamageInput_destroy(Elements *self);
   
   #endif /* DAMAGE_H_INCLUDED */