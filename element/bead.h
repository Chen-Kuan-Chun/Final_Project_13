#ifndef BEAD_H_INCLUDED
#define BEAD_H_INCLUDED
struct _Scene;         // 告訴編譯器 Scene 是個結構
typedef struct _Scene Scene;

#include "element.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>

#define NUM_BEAD_TYPES 4
#define GRID_COLS 6
#define GRID_ROWS 5

typedef struct _Bead {
    float x, y, w, h;
    ALLEGRO_BITMAP *img; // 圖片
    Shape *hitbox;
    int type;            // 0~3 對應哪一張圖
} Bead;

// Bead 函式
Elements *New_Bead(int label, int col, int row, int type);
void Bead_update(Elements *self);
void Bead_interact(Elements *self);
void Bead_draw(Elements *self);
void Bead_destory(Elements *self);

// 產生 5x6 隨機 Bead
// 原本
// void Generate_Bead_Grid();
void Generate_Bead_Grid(Scene *scene);


#endif
