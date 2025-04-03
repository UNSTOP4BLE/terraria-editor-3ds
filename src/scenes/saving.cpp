#include "saving.h"
#include "../app.h"
#include "fileselection.h"

SavingScene::SavingScene(void) {
    setSceneCol(C2D_Color32(0, 0, 0, 255));
    starttime = app->elapsed;
}

void SavingScene::update(void) {
    if (app->elapsed-starttime >= 2)
        setScene(new SelectionScene());
}

void SavingScene::draw(void) { 
    app->fontManager.print(app->screens->top, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, "Save successful!");
}

SavingScene::~SavingScene(void) {
    
}