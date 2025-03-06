#include "inventory.h"
#include "../items.h"

InventoryScene::InventoryScene(void) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    
	Terraria::LoadItemsList("romfs:/items.txt");

    sprites = C2D_SpriteSheetLoad("romfs:/inventory.t3x");
    box_idle = new GFX::Sprite2D(sprites, 0);
    box_hotbar = new GFX::Sprite2D(sprites, 1);
    box_selected = new GFX::Sprite2D(sprites, 2);
    inventorypanel = new GFX::Sprite2D(sprites, 4);
    scrollbar = new GFX::Sprite2D(sprites, 5);
    infopanel = new GFX::Sprite2D(sprites, 6);
}

void InventoryScene::update(void) {
    for (int i = 0; i < Terraria::NUM_INVENTORY_SLOTS; i ++) {
       
    }
}

void InventoryScene::draw(void) {
    inventorypanel->setXY(188, 137);
    inventorypanel->draw(app->screens->bottom);
    scrollbar->setXY(311, 48);
    scrollbar->draw(app->screens->bottom);

    int _xy[2] = {87, 65}; //hardcoded cause fuck good code
    int xy[2];
    for (int i = 0; i < Terraria::NUM_INVENTORY_SLOTS; i ++) {
        int spacing = 49;
        if (i == 0) {
            xy[0] = _xy[0];
            xy[1] = _xy[1];
        }
        else {
            if (i%5 != 0) {
                xy[0] += spacing;
            }
            else {
                xy[0] = _xy[0];
                xy[1] += spacing;
            }
        }
        if (i < Terraria::NUM_HOTBAR_SLOTS) {
            box_hotbar->setXY(xy[0], xy[1]);
            box_hotbar->draw(app->screens->bottom);
        }
        else if (selection == i) {
            box_selected->setXY(xy[0], xy[1]);
            box_selected->draw(app->screens->bottom);
        }
        else {
            box_idle->setXY(xy[0], xy[1]);
            box_idle->draw(app->screens->bottom);
        }
    }
    infopanel->setXY(GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2);
    infopanel->draw(app->screens->top);
}

InventoryScene::~InventoryScene(void) {
    delete box_idle;
    delete box_hotbar;
    delete infopanel;
    delete inventorypanel;
	C2D_SpriteSheetFree(sprites);
}