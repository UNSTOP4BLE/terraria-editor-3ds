#include "disclamer.h"
#include "../app.h"
#include "../pad.h"
#include "fileselection.h"

DisclamerScene::DisclamerScene(void) {
    setSceneCol(C2D_Color32(0, 0, 0, 255));
}

void DisclamerScene::update(void) {
    if (app->elapsed >= 3 || Pad::Pressed(Pad::KEY_A))
        setScene(new SelectionScene());
}

void DisclamerScene::draw(void) {
    
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2-30*3, "DISCLAIMER:");
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2-30*2, "I am not held responsible if you corrupt");
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2-30, "your saves, this tool is made for people");
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, "who know what they are doing");
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2+30, "dont mess around with your save files if");
    app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2+30*2, "you dont know what youre doing!");
    app->fontManager.print(app->screens->bottom, GFX::SCR_BTM_W, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2, "Made by: UNSTOP4BLE");
    app->fontManager.print(app->screens->bottom, GFX::SCR_BTM_W, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2+30, "With help from Sono");

}

DisclamerScene::~DisclamerScene(void) {
    
}