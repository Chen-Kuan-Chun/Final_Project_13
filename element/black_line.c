 /* ============================================================
   black_line.c  –  draw divider using assets/image/black_line.png
   ------------------------------------------------------------ */
   #include "black_line.h"
   #include "../scene/gamescene.h"      /* for enum labels if needed */
   #include <allegro5/allegro_image.h>
   #include <allegro5/allegro_primitives.h>
   #include <stdio.h>
   
   /* one‑shot init for image + primitives addons */
   static void ensure_bl_gfx(void)
   {
       static bool ok = false;
       if (!ok) {
           al_init_image_addon();
           al_init_primitives_addon();
           ok = true;
       }
   }
   
   Elements *New_BlackLine(int label)
   {
       ensure_bl_gfx();
   
       BlackLine *pDerivedObj = (BlackLine*)malloc(sizeof(BlackLine));
       Elements  *pObj        = New_Elements(label);
   
       pDerivedObj->bmp = al_load_bitmap("assets/image/black_line.png");
       if (!pDerivedObj->bmp) {
           fprintf(stderr, "WARNING: failed to load assets/image/black_line.png – fallback to primitives line\n");
       }
       pDerivedObj->h = 15;               /* default thickness */
       pDerivedObj->y = HEIGHT / 2 - 400;      /* vertical centre */
   
       pObj->pDerivedObj = pDerivedObj;
       pObj->Update   = BlackLine_update;
       pObj->Interact = BlackLine_interact;
       pObj->Draw     = BlackLine_draw;
       pObj->Destroy  = BlackLine_destroy;
   
       return pObj;
   }
   
   void BlackLine_update(Elements *self) {}
   void BlackLine_interact(Elements *self) {}
   
   void BlackLine_draw(Elements *self)
   {
       BlackLine *bl = (BlackLine*)self->pDerivedObj;
       if (bl->bmp) {
           float bmp_w = (float)al_get_bitmap_width (bl->bmp);
           float bmp_h = (float)al_get_bitmap_height(bl->bmp);
           /* scale bitmap proportionally in X to fill screen width, keep requested height bl->h */
           al_draw_scaled_bitmap(bl->bmp,
                                 0, 0, bmp_w, bmp_h,
                                 0, bl->y - bl->h / 2,
                                 WIDTH, bl->h,
                                 0);
       } else {
           /* fallback – primitives black line ±2 px */
           al_draw_filled_rectangle(0, bl->y - bl->h/2, WIDTH, bl->y + bl->h/2,
                                    al_map_rgb(0, 0, 0));
       }
   }
   
   void BlackLine_destroy(Elements *self)
   {
       BlackLine *bl = (BlackLine*)self->pDerivedObj;
       if (bl->bmp) al_destroy_bitmap(bl->bmp);
       free(bl);
       free(self);
   }
   