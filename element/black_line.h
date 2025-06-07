/* ============================================================
   black_line.h  –  divider element using a bitmap or fallback line
   ------------------------------------------------------------ */
   #ifndef BLACK_LINE_H_INCLUDED
   #define BLACK_LINE_H_INCLUDED
   #include "element.h"                 /* pulls in global.h for WIDTH / HEIGHT */
   #include <allegro5/allegro.h>        /* bitmap definitions */
   #include <stdbool.h>
   
   typedef struct _BlackLine {
       ALLEGRO_BITMAP *bmp;   /* loaded divider image (can be NULL) */
       int y;                 /* vertical centre position */
       int h;                 /* desired thickness in pixels */
   } BlackLine;
   
   Elements *New_BlackLine(int label);
   void      BlackLine_update(Elements *self);
   void      BlackLine_interact(Elements *self);
   void      BlackLine_draw(Elements *self);
   void      BlackLine_destroy(Elements *self);
   
   #endif /* BLACK_LINE_H_INCLUDED */
   