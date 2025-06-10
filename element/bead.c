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
#include <allegro5/allegro_audio.h>
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

static ALLEGRO_SAMPLE *destroy_smp = NULL;   /* 只存指標，不要重複載入 */

static void ensure_destroy_sample(void)
{
    al_reserve_samples(32);
    destroy_smp = al_load_sample("assets/sound/destroy.wav");
    GAME_ASSERT(destroy_smp, "Failed to load destroy.wav");
}

static float get_grid_cx(int col) { return col * 72 + 230; }
static float get_grid_cy(int row) { return row * 72 + 300; }

// 建立單顆 bead
Elements *New_Bead(int label, int col, int row, int type) {
    Bead *pDerivedObj = (Bead *)malloc(sizeof(Bead));
    Elements *pObj = New_Elements(label);

    pDerivedObj->font1 = al_load_ttf_font("assets/font/DIN Condensed Bold.ttf", 24, 0);
    pDerivedObj->bk = al_load_bitmap("assets/image/break.png");
    pDerivedObj->x = get_grid_cx(col);
    pDerivedObj->y = get_grid_cy(row);
    pDerivedObj->w = 67;
    pDerivedObj->h = 67;
    pDerivedObj->skill1_1 = 4;
    pDerivedObj->skill1_2 = 3;
    pDerivedObj->skill2_1 = 8;
    pDerivedObj->skill2_2 = 6;
    pDerivedObj->round = 0;
    pDerivedObj->round2 = 0;
    pDerivedObj->atk = 1;
    pDerivedObj->recovery = 0;
    pDerivedObj->recovery_add = false;
    pDerivedObj->type = type;
    pDerivedObj->click = false;
    pDerivedObj->img = bead_imgs[type];
    pDerivedObj->bead_start_time = al_get_time() - 5;  // 轉珠開始的時間
    pDerivedObj->bead_time_limit = 5.0;     
    pDerivedObj->now = al_get_time();
    pDerivedObj->destroyed = false;
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
    srand(time(NULL));//把目前的累積時間當作亂數

    for (int i = 0; i < NUM_BEAD_TYPES; i++) {
        char path[50];
        sprintf(path, "assets/image/bead%d.png", i + 1);
        bead_imgs[i] = al_load_bitmap(path);
        GAME_ASSERT(bead_imgs[i], "Failed to load %s", path);
    }//自動載入每一種珠子圖檔，根據編號 1～N 對應檔案名 bead1.png～beadN.png，確保每個類型的珠子有對應的圖可以顯示，若圖片找不到會立刻報錯退出，避免 NULL 崩潰

    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            int type = rand() % NUM_BEAD_TYPES;
            Elements *bead = New_Bead(Bead_L, c, r, type);//rand()%int(定植)，0~定值這表示從 0 到 n - 1 之間隨機取一個整數值，
            //若想要k~2，則可以增加平移量 rand() % NUM_BEAD_TYPES *+2*
            _Register_elements(scene, bead);//意思就是把某個 Elements 物件（例如 Bead）加到場景的元素清單中。for (int r = 0; r < 5; r++) {如果你不用這個架構會怎樣？你就要手動這樣做
  /*for (int c = 0; c < 6; c++) {
    if (bead_grid[r][c])
      Draw_Bead(bead_grid[r][c]);*/
        }
    }
    COUNT = 1;
    while(COUNT){
        COUNT = 0;
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                if (bead_grid[r][c]) {
                    check_match(bead_grid[r][c]);
                }
            }
        }
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                Bead* b = bead_grid[r][c];
                if (b && b->destroyed) {
                    // 這裡可以依你需求換成你要的轉換邏輯：
                    b->type = rand() % 4; // 假設 NUM_TYPES 是總種類數
                    b->img = bead_imgs[b->type];
                    b->destroyed = false; // 清除標記
                    COUNT = 1;
                }
            }
        }
    }
}



void move_bead_to(Bead* b, int row, int col) {
    b->row = row;
    b->col = col;
    b->x = get_grid_cx(col);
    b->y = get_grid_cy(row);//更新珠子「實際在畫面上的 pixel 座標」這樣之後 Draw_Bead(b) 才會畫在正確格子的中心
    bead_grid[row][col] = b;
}//bead_grid[r][c]	快速查出這一格是哪顆珠子（或空的）b->row / b->col	珠子自己知道它在哪格b->x / b->y	珠子畫出來的實際像素位置（畫面上哪裡）

void Bead_update(Elements *self) {
    Bead *b = (Bead *)(self->pDerivedObj);

    ALLEGRO_MOUSE_STATE mstate;
    al_get_mouse_state(&mstate);

    bool mouse_down = al_mouse_button_down(&mstate, 1);//if1表示滑鼠有點左鍵
    b->now = al_get_time();
    if(COUNT < 50 && ROUND > 3){
        b->bead_time_limit = 4;
    }


    // 滑鼠按下、剛開始拖曳
    if (mouse_down && !mouse_was_down && dragging_bead == NULL ) {
        //mousewasdown是上一貞是否按左鍵 if沒按但此貞按了就可以 「如果這一幀是 剛剛 按下左鍵，並且你目前沒在拖珠子，才進入拖曳模式」
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                Bead* target = bead_grid[r][c];
                if (target) {
                    float dx = fabs(mstate.x - target->x);
                    float dy = fabs(mstate.y - target->y);

                    if (dx < 30 && dy < 30) {
                        dragging_bead = target;
                        dragging_row =  r;
                        dragging_col = c;
                        last_grid_row = r;
                        last_grid_col = c;//滑鼠剛點的時候記的
                        b->bead_start_time = al_get_time();
                        
                        return;
                    }
                }
            }
        }
    /*偵測是否「剛按下滑鼠左鍵」	mouse_down && !mouse_was_down
找出滑鼠點到哪顆珠子	用 fabs() 比對滑鼠與珠子位置
記錄拖曳狀態與起始格子	dragging_bead = target 等等
記錄開始拖曳的時間（可動畫）	bead_start_time = al_get_time()*/

}
    // 正在拖曳且在時限內
    else if (mouse_down && dragging_bead && (b->now - b->bead_start_time < b->bead_time_limit)) {
        
        dragging_bead->x = mstate.x;
        dragging_bead->y = mstate.y;
        b->click = true;
        b->recovery_add = false;
        int new_col = (mstate.x - 230 + 36) / 72;
        int new_row = (mstate.y - 300 + 36) / 72;
        
//這裡是把滑鼠的 pixel 位置 ➜ 轉成盤面格子座標：
        if (new_row >= 0 && new_row < GRID_ROWS && new_col >= 0 && new_col < GRID_COLS &&
            (new_row != last_grid_row || new_col != last_grid_col)) //只要滑鼠移動到新的格子中心，才會進行交換（避免重複交換同一格）//且不會讀到記憶體未定義區域（陣列越界）
            {
            Bead* target = bead_grid[new_row][new_col];//確認這格有珠子，而且不是自己（不能跟自己換）
     if (target && target != dragging_bead) {
    // 先讓 target 移到原本拖曳珠的位置
    move_bead_to(target, dragging_row, dragging_col);

    // 再更新 dragging_bead 到新的格子
    move_bead_to(dragging_bead, new_row, new_col);

    // 更新拖曳紀錄
    last_grid_row = new_row;
    last_grid_col = new_col;
    dragging_row = new_row;
    dragging_col = new_col;
}

        }
    // 拖曳結束，滑鼠放開
    }
    else if (b->now - b->bead_start_time < b->bead_time_limit) {
        mouse_down = false;
        b->click = false;
    }
    else if (!mouse_down && dragging_bead)//玩家剛剛拖了一顆珠子，現在放開了滑鼠
    {
        move_bead_to(dragging_bead, last_grid_row, last_grid_col);
        dragging_bead = NULL;
        b->click = false;
        ROUND ++;
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                if (bead_grid[r][c]) {
                    check_match(bead_grid[r][c]);
                }
            }
        }
        for (int r = 0; r < GRID_ROWS; r++) {
            for (int c = 0; c < GRID_COLS; c++) {
                Bead* b = bead_grid[r][c];
                if (b && b->destroyed) {
                    // 這裡可以依你需求換成你要的轉換邏輯：
                    if(b->type == 2){
                        COUNT += (b->atk*2);
                    }else if(b->type == 0){
                        COUNT += b->atk;
                    }else if(b->type == 1){
                        RECOVER ++;
                    }
                    b->type = rand() % 4; // 假設 NUM_TYPES 是總種類數
                    b->img = bead_imgs[b->type];
                    b->destroyed = false; // 清除標記
                }
            }
        }
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
    }
    if((b->skill1_2 - ROUND)<= 0){
        if(mouse_down && 290 < mstate.x && mstate.x < 315 && 250 < mstate.y && mstate.y < 275){
            b->round2 = ROUND;
            b->skill1_2 = ROUND + 2;
            b->atk = 2;
        }
    }else if(b->round2 == (ROUND - 1)){
        b->atk = 1;
    }if((b->skill2_2 - ROUND)<= 0){
        if(mouse_down && 365 < mstate.x && mstate.x < 390 && 250 < mstate.y && mstate.y < 275 && !b->recovery_add){
            b->recovery ++;
            b->recovery_add = true;
            b->skill2_2 = ROUND + 5;
        }
    }if((b->skill2_1 - ROUND)<= 0){
        if(mouse_down && 365 < mstate.x && mstate.x < 390 && 225 < mstate.y && mstate.y < 250){
            if(b->type == 1){
                b->type = 2;
                b->img = bead_imgs[b->type];
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
        al_draw_filled_rectangle(290, 250, 315, 275, al_map_rgb(125, 125, 125));
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
        al_draw_filled_rectangle(365, 250, 390, 275, al_map_rgb(125, 125, 125));
        snprintf(obj->skill2_2_str, sizeof(obj->skill2_2_str), "%d", obj->skill2_2 - ROUND);
        al_draw_text(obj->font1, al_map_rgb(205, 0, 55), 373, 253, ALLEGRO_ALIGN_LEFT, obj->skill2_2_str);
    }
    if((obj->now - obj->bead_start_time < 5) && obj->click){
        al_draw_filled_rectangle(240, 275, (obj->bead_start_time - obj->now + obj->bead_time_limit)*80 + 240, 290, al_map_rgb(250, 255, 100));//條寬 = 剩餘時間 × 80 像素
        //前提是：正在點擊 (obj->click == true) 且拖曳還在 5 秒內
    }if(((ROUND-1)/3 - obj->recovery) <= 4){
        al_draw_filled_rectangle(240, 290, 640 - ((ROUND-1)/3 - obj->recovery - (RECOVER/3))*100, 305, al_map_rgb(255, 192, 203));
    }
    if((4 - (ROUND-1)/3 + obj->recovery + (RECOVER/3)) <= 0){
        OVER = true;
    }
}

void Bead_destory(Elements *self) {
    Bead *obj = (Bead *)(self->pDerivedObj);
    free(obj->hitbox);
    free(obj);
    free(self);
}

void check_match(Bead* b) {
    if (!b) return;
    int r = b->row;
    int c = b->col;
    int t = b->type;

    // 檢查右邊兩顆是否和本身一樣
    if (c + 2 < GRID_COLS) {
        Bead* b1 = bead_grid[r][c + 1];
        Bead* b2 = bead_grid[r][c + 2];
        if (b1 && b2 && b1->type == t && b2->type == t) {
            b->destroyed = true;
            b1->destroyed = true;
            b2->destroyed = true;
        }
    }

    // 檢查下方兩顆是否和本身一樣
    if (r + 2 < GRID_ROWS) {
        Bead* b1 = bead_grid[r + 1][c];
        Bead* b2 = bead_grid[r + 2][c];
        if (b1 && b2 && b1->type == t && b2->type == t) {
            b->destroyed = true;
            b1->destroyed = true;
            b2->destroyed = true;
        }
    }
}