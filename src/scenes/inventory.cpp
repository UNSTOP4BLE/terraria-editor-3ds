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
    inventorypanel.setXY(188, 137);
    scrollbar = GFX::LoadSprite2D(sprites, 5);
    scrollbar.setXY(311, 46);
    infopanel = GFX::LoadSprite2D(sprites, 6);
    infopanel.setXY(GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2);
}

int clamp(int d, int min, int max) {
    const int t = d < min ? min : d;
    return t > max ? max : t;
}

float map_value(float value, float input_min, float input_max, float output_min, float output_max) {
    return (value - input_min) / (input_max - input_min) * (output_max - output_min) + output_min;
}


void InventoryScene::update(void) {
    selection = clamp(selection, 0, Terraria::NUM_INVENTORY_SLOTS - 1);
    
    // Handle directional input
    if (Pad::Pressed(Pad::KEY_DLEFT) && selection % Terraria::NUM_INVENTORY_ROWS != 0) {
        selection -= 1;
    } 
    else if (Pad::Pressed(Pad::KEY_DRIGHT) && selection % Terraria::NUM_INVENTORY_ROWS != Terraria::NUM_INVENTORY_ROWS-1) {
        selection += 1;
    } 
    else if (Pad::Pressed(Pad::KEY_DUP) && selection >= Terraria::NUM_INVENTORY_ROWS) {
        selection -= 5;
    } 
    else if (Pad::Pressed(Pad::KEY_DDOWN) && selection + Terraria::NUM_INVENTORY_ROWS < Terraria::NUM_INVENTORY_SLOTS) {
        selection += 5;
    }
    
    touchPosition pos = Pad::GetTouchPos();
    GFX::Rect touchbar = {310, 46, 20, 230};
    if (Pad::isTouching(touchbar)) {
        int min = 46;
        int max = 230;
        scrollbar.setXY(311, clamp(pos.py, min, max));                                
        int spacing = Terraria::INVENTORY_SLOT_SPACING;
                                                                                                    //4 rows fit on the screen
        scroll = map_value(scrollbar.pos.y, min, max, 0, (Terraria::NUM_INVENTORY_SLOTS/5)*spacing - 4*spacing);
    }
}

void InventoryScene::draw(void) {
    inventorypanel.draw(app->screens->bottom);
    scrollbar.draw(app->screens->bottom);
        
    // Initial position and spacing
    int _xy[2] = {87, 65 - scroll};  // hardcoded cause fuck good code
    int xy[2] = {_xy[0], _xy[1]};       

    int spacing = Terraria::INVENTORY_SLOT_SPACING;

    for (int i = 0; i < Terraria::NUM_INVENTORY_SLOTS; i++) {
        // Update position based on the index (i)
        if (i % Terraria::NUM_INVENTORY_ROWS == 0 && i != 0) {
            xy[0] = _xy[0];  // Reset to initial x
            xy[1] += spacing; // Move down
        } else if (i != 0) {
            xy[0] += spacing; // Move right
        }

        // Set the box based on selection and type
        if (Pad::isTouching({xy[0]-((spacing-3)/2), xy[1]-((spacing-3)/2), spacing-3, spacing-3}))
            selection = i;
        if (selection == i) {
            box_selected.setXY(xy[0], xy[1]);
            box_selected.draw(app->screens->bottom);
        } else {
            auto& box = (i < Terraria::NUM_HOTBAR_SLOTS) ? box_hotbar : box_idle;
            box.setXY(xy[0], xy[1]);
            box.draw(app->screens->bottom);
        }
    }
    //hider
    GFX::Rect hider = {0, 0, GFX::SCR_BTM_W, 37};
    GFX::drawRect(app->screens->bottom, hider, app->clearcol);
    hider = {0, GFX::SCR_BTM_H-2, GFX::SCR_BTM_W, 20};
    GFX::drawRect(app->screens->bottom, hider, app->clearcol);
    
    //top screen
    infopanel.draw(app->screens->top);

//	app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "Copper Pickaxe");
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "%d %d", scroll, scrollbar.pos.y);
}

InventoryScene::~InventoryScene(void) {
	C2D_SpriteSheetFree(sprites);
}