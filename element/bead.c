#include "bead.h"
#include "../GAME_ASSERT.h"
#include "../global.h"
#include "../scene/gamescene.h"  
#include "../scene/sceneManager.h"
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>

ALLEGRO_BITMAP *bead_imgs[NUM_BEAD_TYPES]; // 全域：圖片資源



// 建立單顆 bead
Elements *New_Bead(int label, int col, int row, int type) {
    Bead *pDerivedObj = (Bead *)malloc(sizeof(Bead));
    Elements *pObj = New_Elements(label);

    pDerivedObj->x = col * 72 + 230;
    pDerivedObj->y = row * 72 +300;
    pDerivedObj->w = 67;
    pDerivedObj->h = 67;
    pDerivedObj->type = type;
    pDerivedObj->img = bead_imgs[type];
    pObj->pDerivedObj = pDerivedObj;

    pObj->Update = Bead_update;
    pObj->Interact = Bead_interact;
    pObj->Draw = Bead_draw;
    pObj->Destroy = Bead_destory;
    return pObj;
}

// 原本
// void Generate_Bead_Grid() {
void Generate_Bead_Grid(Scene *scene) {
    srand(time(NULL));

    for (int i = 0; i < NUM_BEAD_TYPES; i++) {
        char path[50];
        sprintf(path, "assets/image/bead%d.png", i + 1);
        bead_imgs[i] = al_load_bitmap(path);
        GAME_ASSERT(bead_imgs[i], "Failed to load %s", path);
        for (int i = 0; i < NUM_BEAD_TYPES; i++) {
    char path[50];
    sprintf(path, "assets/image/bead%d.png", i + 1);
    bead_imgs[i] = al_load_bitmap(path);

   
}

    }

    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            int type = rand() % NUM_BEAD_TYPES;
            Elements *bead = New_Bead(Bead_L, c, r, type);
            _Register_elements(scene, bead);  // ✅ 現在傳進來的 scene 才正確
        }
    }
}


void Bead_update(Elements *self) {}

void Bead_interact(Elements *self) {}

void Bead_draw(Elements *self) {
    Bead *obj = (Bead *)(self->pDerivedObj);
    al_draw_scaled_bitmap(
        obj->img,
        0, 0, al_get_bitmap_width(obj->img), al_get_bitmap_height(obj->img),
        obj->x, obj->y, obj->w, obj->h,
        0
    );
}

void Bead_destory(Elements *self) {
    Bead *obj = (Bead *)(self->pDerivedObj);
    free(obj->hitbox);
    free(obj);
    free(self);
}
