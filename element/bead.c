#include "bead.h"
#include "../GAME_ASSERT.h"
#include "../global.h"
#include "../scene/gamescene.h"  
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
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

    pDerivedObj->font1 = al_load_ttf_font("assets/font/DIN Condensed Bold.ttf", 24, 0);
    pDerivedObj->x = get_grid_cx(col);
    pDerivedObj->y = get_grid_cy(row);
    pDerivedObj->w = 67;
    pDerivedObj->h = 67;
    pDerivedObj->skill1_1 = 4;
    pDerivedObj->skill1_2 = 3;
    pDerivedObj->skill2_1 = 8;
    pDerivedObj->skill2_2 = 6;
    pDerivedObj->round = 0;
    pDerivedObj->recovery = 0;
    pDerivedObj->recovery_add = false;
    pDerivedObj->type = type;
    pDerivedObj->click = false;
    pDerivedObj->img = bead_imgs[pDerivedObj->type];
    pDerivedObj->bead_start_time = al_get_time() - 5;  // 轉珠開始的時間
    pDerivedObj->bead_time_limit = 5.0;     
    pDerivedObj->now = al_get_time();
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
    b->now = al_get_time();

    // 印出滑鼠狀態
    // printf("[Mouse] x=%d y=%d down=%d\n", mstate.x, mstate.y, mouse_down);

    // 滑鼠按下、剛開始拖曳
    if (mouse_down && !mouse_was_down && dragging_bead == NULL ) {
        
        printf("[Info] Mouse down, check which bead is clicked.\n");
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                Bead* target = bead_grid[r][c];
                if (target) {
                    float dx = fabs(mstate.x - target->x);
                    float dy = fabs(mstate.y - target->y);
                    // printf(" - Check bead[%d][%d]: x=%.1f y=%.1f dx=%.1f dy=%.1f\n", r, c, target->x, target->y, dx, dy);

                    if (dx < 30 && dy < 30) {
                        dragging_bead = target;
                        dragging_row = r;
                        dragging_col = c;
                        last_grid_row = r;
                        last_grid_col = c;
                        b->bead_start_time = al_get_time();
                        // printf("[Success] Drag start on bead[%d][%d]\n", r, c);
                        return;
                    }
                }
            }
        }
    }
    // 正在拖曳
    else if (mouse_down && dragging_bead && (b->now - b->bead_start_time < b->bead_time_limit)) {
        // printf("[Dragging] Bead at x=%d y=%d\n", mstate.x, mstate.y);
        dragging_bead->x = mstate.x;
        dragging_bead->y = mstate.y;
        b->click = true;
        b->recovery_add = false;

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
    // 拖曳結束，滑鼠放開
    }else if (b->now - b->bead_start_time < b->bead_time_limit) {
        mouse_down = false;
        b->click = false;
    }else if (!mouse_down && dragging_bead) {
        // printf("[Release] Bead dropped at grid[%d][%d]\n", last_grid_row, last_grid_col);
        move_bead_to(dragging_bead, last_grid_row, last_grid_col);
        dragging_bead = NULL;
        b->click = false;
        ROUND ++;
    }
    mouse_was_down = mouse_down;
    if((b->skill1_1 - ROUND)<= 0){
        if(mouse_down && 290 < mstate.x && mstate.x < 315 && 225 < mstate.y && mstate.y < 250){
            b->bead_time_limit = 7.0;
            b->round = ROUND;
            b->skill1_1 = ROUND + 3;
        }
    }else if(b->round == (ROUND - 1)){
            b->bead_time_limit = 5.0;
    }if((b->skill2_2 - ROUND)<= 0){
        if(mouse_down && 365 < mstate.x && mstate.x < 390 && 250 < mstate.y && mstate.y < 275 && !b->recovery_add){
            b->recovery ++;
            b->recovery_add = true;
            b->skill2_2 = ROUND + 5;
        }
    }if((b->skill2_1 - ROUND)<= 0){
        if(mouse_down && 365 < mstate.x && mstate.x < 390 && 225 < mstate.y && mstate.y < 250){
            if(b->type == 1){
                b->img = bead_imgs[2];
            }
            b->skill2_1 = ROUND + 7;
        }
    }
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
    al_draw_filled_rectangle(240, 225, 290, 275, al_map_rgb(100, 100, 50));
    al_draw_filled_rectangle(290, 250, 315, 275, al_map_rgb(150, 150, 150));
    al_draw_filled_rectangle(315, 225, 365, 275, al_map_rgb(50, 100, 100));
    al_draw_filled_rectangle(365, 225, 390, 250, al_map_rgb(100, 100, 100));
    al_draw_filled_rectangle(365, 250, 390, 275, al_map_rgb(150, 150, 150));
    if((obj->skill1_1 - ROUND) <= 0){
        al_draw_filled_rectangle(290, 225, 315, 250, al_map_rgb(0, 250, 100));
    }else{
        al_draw_filled_rectangle(290, 225, 315, 250, al_map_rgb(100, 100, 100));
        snprintf(obj->skill1_1_str, sizeof(obj->skill1_1_str), "%d", obj->skill1_1 - ROUND);
        al_draw_text(obj->font1, al_map_rgb(205, 0, 55), 298, 228, ALLEGRO_ALIGN_LEFT, obj->skill1_1_str);
    }
    if((obj->skill1_2 - ROUND) <= 0){
        al_draw_filled_rectangle(290, 250, 315, 275, al_map_rgb(0, 250, 100));
    }else{
        al_draw_filled_rectangle(290, 250, 315, 275, al_map_rgb(100, 100, 100));
        snprintf(obj->skill1_2_str, sizeof(obj->skill1_2_str), "%d", obj->skill1_2 - ROUND);
        al_draw_text(obj->font1, al_map_rgb(205, 0, 55), 298, 253, ALLEGRO_ALIGN_LEFT, obj->skill1_2_str);
    }
    if((obj->skill2_1 - ROUND) <= 0){
        al_draw_filled_rectangle(365, 225, 390, 250, al_map_rgb(0, 250, 100));
    }else{
        al_draw_filled_rectangle(365, 225, 390, 250, al_map_rgb(100, 100, 100));
        snprintf(obj->skill2_1_str, sizeof(obj->skill2_1_str), "%d", obj->skill2_1 - ROUND);
        al_draw_text(obj->font1, al_map_rgb(205, 0, 55), 373, 228, ALLEGRO_ALIGN_LEFT, obj->skill2_1_str);
    }
    if((obj->skill2_2 - ROUND) <= 0){
        al_draw_filled_rectangle(365, 250, 390, 275, al_map_rgb(0, 250, 100));
    }else{
        al_draw_filled_rectangle(365, 250, 390, 275, al_map_rgb(100, 100, 100));
        snprintf(obj->skill2_2_str, sizeof(obj->skill2_2_str), "%d", obj->skill2_2 - ROUND);
        al_draw_text(obj->font1, al_map_rgb(205, 0, 55), 373, 253, ALLEGRO_ALIGN_LEFT, obj->skill2_2_str);
    }
    if((obj->now - obj->bead_start_time < 5) && obj->click){
        al_draw_filled_rectangle(240, 275, (obj->bead_start_time - obj->now + obj->bead_time_limit)*80 + 240, 290, al_map_rgb(55, 255, 100));
    }if(((ROUND-1)/3 - obj->recovery) <= 4){
        al_draw_filled_rectangle(240, 290, 640 - ((ROUND-1)/3 - obj->recovery)*100, 305, al_map_rgb(255, 192, 203));
    }
}

void Bead_destory(Elements *self) {
    Bead *obj = (Bead *)(self->pDerivedObj);
    free(obj->hitbox);
    free(obj);
    free(self);
}
