#include "inventory.h"
#include "../items.h"

InventoryScene::InventoryScene(void) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    
//	Terraria::LoadItemsList("romfs:/items.txt");

    sprites = C2D_SpriteSheetLoad("romfs:/inventory.t3x");
    box_idle = new GFX::Sprite2D(sprites, 0);

}

void InventoryScene::update(void) {
}

void InventoryScene::draw(void) {
    box_idle->pos = {(int)app->touch.px, (int)app->touch.py, box_idle->pos.w, box_idle->pos.h};
    box_idle->draw(app->screens->bottom);
}

InventoryScene::~InventoryScene(void) {
    delete box_idle;
	C2D_SpriteSheetFree(sprites);
}