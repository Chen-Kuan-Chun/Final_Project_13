#include "bead.h"
#include "../GAME_ASSERT.h"
#include "../global.h"
#include "../scene/gamescene.h"  
#include "../scene/sceneManager.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//
ALLEGRO_BITMAP *bead_imgs[NUM_BEAD_TYPES];
Bead* bead_grid[GRID_ROWS][GRID_COLS] = {NULL};
Bead* dragging_bead = NULL;
int dragging_row, dragging_col;
int last_grid_row, last_grid_col;
bool mouse_was_down = false;

static float get_grid_cx(int col) { return col * 72 + 230; }
static float get_grid_cy(int row) { return row * 72 + 300; }

// 建立單顆 bead
Elements *New_Bead(int label, int col, int row, int type) {
    Bead *pDerivedObj = (Bead *)malloc(sizeof(Bead));
    Elements *pObj = New_Elements(label);

    pDerivedObj->x = get_grid_cx(col);
    pDerivedObj->y = get_grid_cy(row);
    pDerivedObj->w = 67;
    pDerivedObj->h = 67;
    pDerivedObj->type = type;
    pDerivedObj->img = bead_imgs[type];
    pObj->pDerivedObj = pDerivedObj;

    pObj->Update = Bead_update;
    pObj->Interact = Bead_interact;
    pObj->Draw = Bead_draw;
    pObj->Destroy = Bead_destory;

    bead_grid[row][col] = pDerivedObj;
    pDerivedObj->row = row;
    pDerivedObj->col = col;

    return pObj;
}

void Generate_Bead_Grid(Scene *scene) {
    srand(time(NULL));

    for (int i = 0; i < NUM_BEAD_TYPES; i++) {
        char path[50];
        sprintf(path, "assets/image/bead%d.png", i + 1);
        bead_imgs[i] = al_load_bitmap(path);
        GAME_ASSERT(bead_imgs[i], "Failed to load %s", path);
    }

    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            int type = rand() % NUM_BEAD_TYPES;
            Elements *bead = New_Bead(Bead_L, c, r, type);
            _Register_elements(scene, bead);
        }
    }
}

void move_bead_to(Bead* b, int row, int col) {
    b->row = row;
    b->col = col;
    b->x = get_grid_cx(col);
    b->y = get_grid_cy(row);
    bead_grid[row][col] = b;
}

void Bead_update(Elements *self) {
    Bead *b = (Bead *)(self->pDerivedObj);

    ALLEGRO_MOUSE_STATE mstate;
    al_get_mouse_state(&mstate);

    bool mouse_down = al_mouse_button_down(&mstate, 1);

    // 印出滑鼠狀態
    printf("[Mouse] x=%d y=%d down=%d\n", mstate.x, mstate.y, mouse_down);

    // 滑鼠按下、剛開始拖曳
    if (mouse_down && !mouse_was_down && dragging_bead == NULL) {
        printf("[Info] Mouse down, check which bead is clicked.\n");
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                Bead* target = bead_grid[r][c];
                if (target) {
                    float dx = fabs(mstate.x - target->x);
                    float dy = fabs(mstate.y - target->y);
                    printf(" - Check bead[%d][%d]: x=%.1f y=%.1f dx=%.1f dy=%.1f\n", r, c, target->x, target->y, dx, dy);

                    if (dx < 30 && dy < 30) {
                        dragging_bead = target;
                        dragging_row = r;
                        dragging_col = c;
                        last_grid_row = r;
                        last_grid_col = c;
                        printf("[Success] Drag start on bead[%d][%d]\n", r, c);
                        return;
                    }
                }
            }
        }
    }
    // 正在拖曳
    else if (mouse_down && dragging_bead) {
        printf("[Dragging] Bead at x=%d y=%d\n", mstate.x, mstate.y);
        dragging_bead->x = mstate.x;
        dragging_bead->y = mstate.y;

        int new_col = (mstate.x - 230 + 36) / 72;
        int new_row = (mstate.y - 300 + 36) / 72;

        if (new_row >= 0 && new_row < GRID_ROWS && new_col >= 0 && new_col < GRID_COLS &&
            (new_row != last_grid_row || new_col != last_grid_col)) {
            Bead* target = bead_grid[new_row][new_col];
            if (target && target != dragging_bead) {
                printf("[Swap] Bead at [%d][%d] <--> [%d][%d]\n", dragging_row, dragging_col, new_row, new_col);
                move_bead_to(target, last_grid_row, last_grid_col);
                bead_grid[dragging_row][dragging_col] = NULL;
                last_grid_row = new_row;
                last_grid_col = new_col;
                bead_grid[new_row][new_col] = dragging_bead;
            }
        }
    }
    // 拖曳結束，滑鼠放開
    else if (!mouse_down && dragging_bead) {
        printf("[Release] Bead dropped at grid[%d][%d]\n", last_grid_row, last_grid_col);
        move_bead_to(dragging_bead, last_grid_row, last_grid_col);
        dragging_bead = NULL;
    }

    mouse_was_down = mouse_down;
}


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
