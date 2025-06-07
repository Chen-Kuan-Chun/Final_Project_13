#define ALLEGRO_NO_MAGIC_MAIN
#include <stdio.h>                          // 🔧 新增：為了 stderr / fprintf
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>         // 🔧 新增：為了 al_init_image_addon
#include "GameWindow.h"

int real_main(int argc, char **argv)
{
    al_init();                // 初始化 Allegro
    al_install_mouse();       //  初始化滑鼠
    al_init_image_addon();    //  初始化圖片載入
    if (!al_create_display(800, 600)) {
        fprintf(stderr, "Failed to create display\n");
        return -1;
    }

    Game *game = New_Game();
    game->execute(game);              //  根據你自己的 Game 結構改這裡
    game->game_destroy(game);
    return 0;
}

int main(int argc, char **argv)
{
    return al_run_main(argc, argv, real_main);
}
