#include "inventory.h"
#include "../items.h"

InventoryScene::InventoryScene(void) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    
//	Terraria::LoadItemsList("romfs:/items.txt");

    sprites = C2D_SpriteSheetLoad("romfs:/inventory.t3x");
    box_idle = new GFX::Sprite2D(sprites, 0);
    infopanel = new GFX::Sprite2D(sprites, 6);

}

void InventoryScene::update(void) {
}

void InventoryScene::draw(void) {
    box_idle->setXY((int)app->touch.px, (int)app->touch.py);
    box_idle->draw(app->screens->bottom);
    infopanel->setXY(GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2);
    infopanel->draw(app->screens->top);
}

InventoryScene::~InventoryScene(void) {
    delete box_idle;
    delete infopanel;
	C2D_SpriteSheetFree(sprites);
}