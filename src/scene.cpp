#include "app.h"
#include "scene.h"

void setScene(Scene *scn) {
    app->scenemgr.next = scn;
}

void setSceneCol(uint32_t color) {
    app->clearcol = color;
}

void checkSwapScene(void) {
    if (app->scenemgr.next != NULL) {
        if (app->scenemgr.cur != NULL)
            delete app->scenemgr.cur;

        app->scenemgr.cur = app->scenemgr.next;
        app->scenemgr.next = NULL;
    }
}