#include "saving.h"
#include "../app.h"
#include "fileselection.h"

SavingScene::SavingScene(std::string _msg) {
    setSceneCol(C2D_Color32(0, 0, 0, 255));
    starttime = app->elapsed;
    msg = _msg;
}

void SavingScene::update(void) {
    if (app->elapsed-starttime >= 3)
        setScene(new SelectionScene());
}

void SavingScene::draw(void) { 
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, msg.c_str());
}

SavingScene::~SavingScene(void) {
    
}