#include "global.h"

// 遊戲常數
const double FPS = 60.0;
const int WIDTH = 900;
const int HEIGHT = 672;

int window = 0;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES] = {false};
Point mouse = {0, 0};

ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
bool debug_mode = false;
extern int COUNT = 0;

// ✅ 滑鼠狀態初始化
int global_mouse_x = 0;
int global_mouse_y = 0;
int ROUND = 1;
bool global_mouse_down = false;