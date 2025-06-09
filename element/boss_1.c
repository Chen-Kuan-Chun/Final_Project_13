#include "boss_1.h"
#include "../scene/gamescene.h"
#include "../scene/sceneManager.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
extern void switch_scene(SceneType);

/* -------------- helpers ------------------------------------------------ */
static void ensure_gfx(void)
{
    static bool ok = false;
    if (!ok) { al_init_image_addon(); ok = true; }
}
static inline ALLEGRO_FONT *hp_font(void)
{
    static ALLEGRO_FONT *f = NULL;
    if (!f) {
        al_init_font_addon();  al_init_ttf_addon();
        f = al_load_ttf_font("assets/font/Consolas.ttf", 36, 0);
        if (!f) f = al_create_builtin_font();
    }
    return f;
}

/* -------------- ctor --------------------------------------------------- */
#define TARGET_W 200      /* 想縮成多寬就改這裡 */

Elements *New_Boss1(int label)
{
    ensure_gfx();
    Boss1   *b = calloc(1, sizeof(Boss1));
    Elements* e = New_Elements(label);

    b->bmp = al_load_bitmap("assets/image/boss1.png");
    if (!b->bmp) {
        fprintf(stderr,"WARNING: load boss1.png failed – fallback box\n");
        b->width = b->height = 150;
    } else {
        b->width  = al_get_bitmap_width (b->bmp);
        b->height = al_get_bitmap_height(b->bmp);
    }
    /* 計算縮放後要畫多大 (等比例) */
    b->draw_w = TARGET_W;
    b->draw_h = (int)(b->height * (TARGET_W / (float)b->width));
    b->element = 3;
    
    b->x  = 300;
    b->y  = 100;
    b->hp = 100;

    e->pDerivedObj = b;
    e->Update   = Boss1_update;
    e->Interact = Boss1_interact;
    e->Draw     = Boss1_draw;
    e->Destroy  = Boss1_destroy;
    return e;
}

/* -------------- methods ------------------------------------------------ */
void Boss1_update(Elements *self)   {}
void Boss1_interact(Elements *self) {}

void Boss1_draw(Elements *self)
{
    Boss1 *b = self->pDerivedObj;

    /* 1. 畫 Boss（縮放後尺寸 draw_w / draw_h） */
    if (b->bmp) {
        al_draw_scaled_bitmap(b->bmp,
                              0,0, b->width, b->height,
                              b->x, b->y,
                              b->draw_w, b->draw_h, 0);
    } else {   /* fallback 紅塊 */
        al_draw_filled_rectangle(b->x, b->y,
                                 b->x + b->draw_w, b->y + b->draw_h,
                                 al_map_rgb(200,50,50));
        al_draw_rectangle(b->x, b->y,
                          b->x + b->draw_w, b->y + b->draw_h,
                          al_map_rgb(0,0,0), 2);
    }

    /* 2. 畫血量數字 – 緊貼縮放後圖片上方置中 */
    char buf[8]; snprintf(buf,sizeof(buf),"%d",b->hp);
    ALLEGRO_FONT *f = hp_font();
    int cx = b->x + b->draw_w/2;
    int cy = b->y - al_get_font_line_height(f) - 4;
    al_draw_text(f, al_map_rgb(255,0,0), cx, cy, ALLEGRO_ALIGN_CENTRE, buf);
}

void Boss1_destroy(Elements *self)
{
    Boss1 *b = self->pDerivedObj;
    if (b->bmp) al_destroy_bitmap(b->bmp);
    free(b);  free(self);
}
void Boss1_take_damage(Elements *self,int dmg)
{
    Boss1 *b = self->pDerivedObj;
    if ((b->hp -= dmg) <= 0) self->dele = true;
    if (b->hp <= 0) {
        create_scene(Victory_L);
        return;
    }
}
