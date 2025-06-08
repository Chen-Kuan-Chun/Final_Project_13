#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include "shapes/Point.h"

#define GAME_TERMINATE -1
#define MAX_ELEMENT 100
#define debug(x) \
    if (x) { \
        printf("check at line number %d in file %s\n", __LINE__, __FILE__); \
    }

#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) < (y) ? (y) : (x))

// 系統變數
extern const double FPS;
extern const int WIDTH;
extern const int HEIGHT;
extern int window;
extern int TIME;

// 鍵盤與滑鼠狀態
extern bool key_state[ALLEGRO_KEY_MAX];
extern bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];
extern Point mouse;

// Allegro 元件
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_EVENT event;
extern ALLEGRO_TIMER *fps;

extern bool debug_mode;

// ✅ 滑鼠追蹤變數
extern int global_mouse_x;
extern int global_mouse_y;
extern bool global_mouse_down;

#endif
