#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED
#include "scene.h"
extern Scene *scene;
typedef enum SceneType
{
    Menu_L = 0,
    GameScene_L,
    Card_L,
    About_L,
    Defeat_L,
    Victory_L
} SceneType;
void create_scene(SceneType);

#endif