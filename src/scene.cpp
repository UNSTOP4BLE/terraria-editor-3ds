#include "app.h"
#include "scene.h"

void setScene(Scene *scn) {
    if (app->currentScene != NULL)
        delete app->currentScene;
    app->currentScene = scn;
}

void setSceneCol(uint32_t color) {
    app->clearcol = color;
}