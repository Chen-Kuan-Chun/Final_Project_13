#define ALLEGRO_NO_MAGIC_MAIN
#include "GameWindow.h"

int real_main(int argc, char **argv)
{
    al_init();                // 初始化 Allegro
    al_install_mouse();       // 初始化滑鼠
    al_install_keyboard();    // 若有用鍵盤
    al_init_image_addon();    // 初始化圖片支援

    // ✅ 建立顯示器，並儲存回傳值
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Failed to create display\n");
        return -1;
    }

    // ✅ 建立計時器與事件佇列（推薦）
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60); // 60FPS
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // ✅ 啟動遊戲
    Game *game = New_Game();

    al_start_timer(timer);
    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        // ✅ 更新與畫圖分離
        game->update(game);

        al_clear_to_color(al_map_rgb(0, 0, 0));
        game->draw(game);  // 呼叫你的 draw function
        al_flip_display(); // ✅ 這行是關鍵
    }

    // 清理
    game->game_destroy(game);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}

int main(int argc, char **argv)
{
    return al_run_main(argc, argv, real_main);
}
