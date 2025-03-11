#include "inventory.h"
#include "../pad.h"
#include <cstdio>

InventoryScene::InventoryScene(void) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    scroll = 0;
    selection = 0;
    editing = false;
	Terraria::LoadItemsList("romfs:/items.txt", &itemslist);
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

    //load save
    parser.readFile("romfs:/1.p");
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
    if (editing) {
        int index = Terraria::getIndex(curreplaceitem.id, parser.outdata);
        
        if (Pad::Held(Pad::KEY_L)) //modifiers
        {
            if (Pad::Pressed(Pad::KEY_DLEFT))
                parser.outdata.items[index].modifier -= 1;
            else if (Pad::Pressed(Pad::KEY_DRIGHT))
                parser.outdata.items[index].modifier += 1;
        }
        else { //items
            if (Pad::Pressed(Pad::KEY_DLEFT))
                parser.outdata.items[index].itemid -= 1;
            else if (Pad::Pressed(Pad::KEY_DRIGHT))
                parser.outdata.items[index].itemid += 1;
            else if (Pad::Pressed(Pad::KEY_DUP))
                parser.outdata.items[index].count += 1;
            else if (Pad::Pressed(Pad::KEY_DDOWN))
                parser.outdata.items[index].count -= 1;
        }
        //todo
//        if (Pad::Pressed(Pad::KEY_START))
  //          parser.writeFile();
    }
    else {
        if (Pad::Pressed(Pad::KEY_DLEFT) && selection % Terraria::NUM_INVENTORY_ROWS != 0)
            selection -= 1;
        else if (Pad::Pressed(Pad::KEY_DRIGHT) && selection % Terraria::NUM_INVENTORY_ROWS != Terraria::NUM_INVENTORY_ROWS-1)
            selection += 1;
        else if (Pad::Pressed(Pad::KEY_DUP) && selection >= Terraria::NUM_INVENTORY_ROWS)
            selection -= 5;
        else if (Pad::Pressed(Pad::KEY_DDOWN) && selection + Terraria::NUM_INVENTORY_ROWS < Terraria::NUM_INVENTORY_SLOTS)
            selection += 5;
    }
    if (Pad::Pressed(Pad::KEY_X))
        editing = !editing;

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

    int curindex = 0;
    int curreplaceindex = 0;
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
            curitem = Terraria::getItem(parser.chardata.items[i].itemid, itemslist);
            curreplaceitem = Terraria::getItem(parser.outdata.items[i].itemid, itemslist);
            curindex = Terraria::getIndex(curitem.id, parser.chardata);
            curreplaceindex = Terraria::getIndex(curreplaceitem.id, parser.outdata);
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

    char str[128];
    sprintf(str, "%s, Press start to save file", (editing ? "Press X to select item" : "Press X to edit"));

    app->fontManager.setScale(0.6);
    app->fontManager.print(app->screens->bottom, GFX::Left, 20, 10, str);

    //top screen
    infopanel.draw(app->screens->top);
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Center, 80, 126, "Replace with:");

    int yoff = 0;
    //INVENTORY
    if (editing) {
        hider = {160, 0, 240, 117};
        GFX::drawRect(app->screens->top, hider, app->clearcol);
        app->fontManager.setScale(0.6);
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "DPad L/R: change itemid\nDPad L/R + LT: change modifier\nDPad U/D: change item count\nY: type in itemid\nB: type in item count\nRT: type in item modifier id(todo make this name)");
    }
    else {
        //item name
        //todo print using modifier
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "%s", curitem.item.c_str());

        //item count and modifier
        app->fontManager.setScale(0.8);
        app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "%d in inventory\nmodifier(%d)", parser.chardata.items[curindex].count, parser.chardata.items[curindex].modifier);

        //item id
        app->fontManager.setScale(0.8);
        app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(%d)", curitem.id);
    }
    //REPLACING
    yoff = 118;
    //item name
    //todo print using modifier
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "%s", curreplaceitem.item.c_str());

    //item count and modifier
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "%d in inventory\nmodifier(%d)", parser.outdata.items[curreplaceindex].count, parser.outdata.items[curreplaceindex].modifier);

    //item id
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(%d)", curreplaceitem.id);
}

InventoryScene::~InventoryScene(void) {
	C2D_SpriteSheetFree(sprites);
}