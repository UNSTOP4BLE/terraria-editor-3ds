#include "inventory.h"
#include "../items.h"
#include "../pad.h"

InventoryScene::InventoryScene(void) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    scroll = 0;
    selection = 0;
	Terraria::LoadItemsList("romfs:/items.txt");
    sprites = C2D_SpriteSheetLoad("romfs:/inventory.t3x");
    box_idle = GFX::LoadSprite2D(sprites, 0);
    box_hotbar = GFX::LoadSprite2D(sprites, 1);
    box_selected = GFX::LoadSprite2D(sprites, 2);
    inventorypanel = GFX::LoadSprite2D(sprites, 4);
    scrollbar = GFX::LoadSprite2D(sprites, 5);
    infopanel = GFX::LoadSprite2D(sprites, 6);
}

int clamp(int d, int min, int max) {
    const int t = d < min ? min : d;
    return t > max ? max : t;
}

void InventoryScene::update(void) {
    selection = clamp(selection, 0, Terraria::NUM_INVENTORY_SLOTS - 1);

    // Handle directional input
    if (Pad::Pressed(Pad::KEY_DLEFT) && selection % 5 != 0) {
        selection -= 1;
    } 
    else if (Pad::Pressed(Pad::KEY_DRIGHT) && selection % 5 != 4) {
        selection += 1;
    } 
    else if (Pad::Pressed(Pad::KEY_DUP) && selection >= 5) {
        selection -= 5;
    } 
    else if (Pad::Pressed(Pad::KEY_DDOWN) && selection + 5 < Terraria::NUM_INVENTORY_SLOTS) {
        selection += 5;
    }
}

void InventoryScene::draw(void) {
    inventorypanel.setXY(188, 137);
    inventorypanel.draw(app->screens->bottom);
    scrollbar.setXY(311, 48);
    scrollbar.draw(app->screens->bottom);
        
    // Initial position and spacing
    int _xy[2] = {87, 65 - scroll};  // hardcoded cause fuck good code
    int xy[2] = {_xy[0], _xy[1]};
    const int spacing = 49;

    for (int i = 0; i < Terraria::NUM_INVENTORY_SLOTS; i++) {
        // Update position based on the index (i)
        if (i % 5 == 0 && i != 0) {
            xy[0] = _xy[0];  // Reset to initial x
            xy[1] += spacing; // Move down
        } else if (i != 0) {
            xy[0] += spacing; // Move right
        }

        // Set the box based on selection and type
        if (selection == i) {
            box_selected.setXY(xy[0], xy[1]);
            box_selected.draw(app->screens->bottom);
        } else {
            auto& box = (i < Terraria::NUM_HOTBAR_SLOTS) ? box_hotbar : box_idle;
            box.setXY(xy[0], xy[1]);
            box.draw(app->screens->bottom);
        }
    }
    
    //top screen
    infopanel.setXY(GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2);
    infopanel.draw(app->screens->top);

	app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "Copper Pickaxe");
}

InventoryScene::~InventoryScene(void) {
	C2D_SpriteSheetFree(sprites);
}