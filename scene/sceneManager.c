#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "defeat.h" 
#include "victory.h"
#include "card.h"
#include "about.h"
#include "gg.h"
#include "yy.h"

Scene *scene = NULL;
void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        break;
    case Card_L: 
        scene = New_Card(Card_L);
        break;
    case About_L: 
        scene = New_About(About_L);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L);
        break;
    case Yy_L: 
        scene = New_Yy(Yy_L);
        break;
    case Gg_L: 
        scene = New_Gg(Gg_L);
        break;

    default:
        break;
    }
}
