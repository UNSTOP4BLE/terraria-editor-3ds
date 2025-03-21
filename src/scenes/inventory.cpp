#include "inventory.h"
#include "../pad.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

InventoryScene::InventoryScene(void) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    scroll = 0;
    max_scroll = 0;
    selection = 0;
    editing = false;
	Terraria::LoadItemsList("romfs:/items.txt", &itemslist);
	Terraria::LoadModifierList("romfs:/modifiers.txt", &modifierlist);
    trashButton.init("romfs:/trash.t3x");
    trashButton.pos().x = 30;
    trashButton.pos().y = 65;
    restoreButton.init("romfs:/restore.t3x");
    restoreButton.pos().x = 30;
    restoreButton.pos().y = 65+50;

    sprites = C2D_SpriteSheetLoad("romfs:/inventory.t3x");
    box_idle = GFX::LoadSprite2D(sprites, 0);
    box_hotbar = GFX::LoadSprite2D(sprites, 1);
    box_selected = GFX::LoadSprite2D(sprites, 2);
    inventorypanel = GFX::LoadSprite2D(sprites, 3);
    inventorypanel.setXY(188, 137);
    scrollbar = GFX::LoadSprite2D(sprites, 4);
    scrollbar.setXY(311, 46);
    infopanel = GFX::LoadSprite2D(sprites, 5);
    infopanel.setXY(GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2);

    itemsprites.push_back(C2D_SpriteSheetLoad("romfs:/items/items1.t3x"));
    itemsprites.push_back(C2D_SpriteSheetLoad("romfs:/items/items2.t3x"));
    itemsprites.push_back(C2D_SpriteSheetLoad("romfs:/items/items3.t3x"));

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

void scaleItem(GFX::Sprite2D &spr, float scl, int max) {
    float newscale = scl;  // Start with the initial scale

    float maxw = std::max(spr.pos().w, spr.pos().h);
    // If scaling by 2 exceeds the max width, adjust the scale factor
    if (static_cast<int>(maxw*newscale) > max) {
        newscale = static_cast<float>(max) / maxw;
    }

    spr.scale(newscale);  // Apply the final calculated scale
    
	C2D_SpriteSetCenter(&spr.spr, 0.5, 0.5);
}


void InventoryScene::update(void) {
    selection = clamp(selection, 0, NUM_TOTAL_SLOTS - 1);
    
    int index = selection;
    // Handle directional input
    if (editing) {
        
        if (Pad::Held(Pad::KEY_L) && parser.outdata.items[index].itemid != 0) //modifiers
        {
            if (Pad::Pressed(Pad::KEY_DLEFT) && parser.outdata.items[index].modifier > 0)
                parser.outdata.items[index].modifier -= 1;
            else if (Pad::Pressed(Pad::KEY_DRIGHT) && parser.outdata.items[index].modifier < modifierlist.size())
                parser.outdata.items[index].modifier += 1;
        }
        else { //items
            if (Pad::Pressed(Pad::KEY_DLEFT) && parser.outdata.items[index].itemid > -NUM_NEGATIVE_IDS)
                parser.outdata.items[index].itemid -= 1;
            else if (Pad::Pressed(Pad::KEY_DRIGHT) && index < static_cast<int>(itemslist.size()))
                parser.outdata.items[index].itemid += 1;
            else if (Pad::Pressed(Pad::KEY_DUP) && parser.outdata.items[index].count < INT16_MAX)
                parser.outdata.items[index].count += 1;
            else if (Pad::Pressed(Pad::KEY_DDOWN) && parser.outdata.items[index].count > 0)
                parser.outdata.items[index].count -= 1;

            if (parser.outdata.items[index].itemid == 0)
            {
                parser.outdata.items[index].modifier = 0;
                parser.outdata.items[index].count = 0;
            }
        }

        if (Pad::Pressed(Pad::KEY_Y)) { //item name
            app->keyboard.open("Item name");
            const char *input = app->keyboard.getValue().c_str();
            if (strcmp(input, ""))
                parser.outdata.items[index].itemid = Terraria::getItemId(input, itemslist);
        }
        else if (Pad::Pressed(Pad::KEY_B)) {//item count        
            app->keyboard.openNum("Item amount");
            parser.outdata.items[index].count = clamp(atoi(app->keyboard.getValue().c_str()), 0, INT16_MAX);
        }
        else if (Pad::Pressed(Pad::KEY_R)) { //item modifier
            app->keyboard.open("Item modifier");
            const char *input = app->keyboard.getValue().c_str();
            if (strcmp(input, ""))
                parser.outdata.items[index].modifier = Terraria::getModifierId(input, modifierlist);
        }
        //todo
//        if (Pad::Pressed(Pad::KEY_START))
  //          parser.writeFile();
    }
    else {
        int rows = NUM_INVENTORY_ROWS;
        if (selection > NUM_INVENTORY_SLOTS-1) //moved over to 4 slots
            rows = 4;
        if (Pad::Pressed(Pad::KEY_DLEFT) && selection % rows != 0) //start of row
            selection -= 1;
        else if (Pad::Pressed(Pad::KEY_DRIGHT) && selection % rows != (rows-1)) //end of row
            selection += 1;
        else if (Pad::Pressed(Pad::KEY_DUP) && selection >= rows) { //start of selection
            selection -= rows;
            scroll -= 46+INVENTORY_SLOT_SPACING;
        }
        else if (Pad::Pressed(Pad::KEY_DDOWN) && (selection + rows) < NUM_TOTAL_SLOTS) { //end of selection
            selection += rows;
            scroll += 46+INVENTORY_SLOT_SPACING;
        }
    }
    if (Pad::Pressed(Pad::KEY_X))
        editing = !editing;

    touchPosition pos = Pad::GetTouchPos();
    GFX::Rect<int> touchbar = {310, 46, 20, 230};
    if (Pad::isTouching(touchbar)) {
        int min = 46;
        int max = 230;
        scrollbar.setXY(311, clamp(pos.py, min, max));                 

        scroll = map_value(scrollbar.pos().y, min, max, 0, max_scroll);
    }

    //trash button
    if (trashButton.pressed())
        parser.outdata.items[index] = {0, 0, 0};

    //restore button
    if (restoreButton.pressed())
        parser.outdata.items[index] = parser.chardata.items[index];

}

void InventoryScene::draw(void) {
    inventorypanel.draw(app->screens->bottom);
    scrollbar.draw(app->screens->bottom);
        
    // Initial position and spacing
    int initx = 87;
    GFX::Rect<int> pos = {87, 65-scroll, 46, 46};       

    int curindex = 0;
    int curreplaceindex = 0;
    int spacing = 0;
    int offset = 0;

    for (int i = 0; i < NUM_TOTAL_SLOTS; i++) {
        if (i == NUM_INVENTORY_SLOTS) {
            spacing = 1;    
            offset = NUM_INVENTORY_SLOTS;
        }
        // Update position based on the index (i)
        if (i != 0) {
            if ((i-offset) % (NUM_INVENTORY_ROWS-spacing) == 0) {
                pos.x = initx;  // Reset to initial x
                pos.y += pos.h+INVENTORY_SLOT_SPACING; // Move down
                if (i >= NUM_INVENTORY_SLOTS) {
                    switch (i-NUM_INVENTORY_SLOTS) {
                        case 0: //coins
                            app->fontManager.setScale(1.2);
                            app->fontManager.print(app->screens->bottom, GFX::Left, pos.x-10, pos.y-15, "Coins:");
                            break;
                        case NUM_AMMO_SLOTS: //ammo
                            app->fontManager.setScale(1.2);
                            app->fontManager.print(app->screens->bottom, GFX::Left, pos.x-10, pos.y-15, "Ammo:");
                            break;
                    }
                    pos.y += pos.h+INVENTORY_SLOT_SPACING; // Move down   
                }
            } 
            else
                pos.x += pos.w+INVENTORY_SLOT_SPACING; // Move right
            
            
        }
        // Set the box based on selection and type
        if (!editing && Pad::isTouching({pos.x-pos.w/2, pos.y-pos.h/2, pos.w, pos.h}))
            selection = i;
        if (selection == i) {
            box_selected.setXY(pos.x, pos.y);
            box_selected.draw(app->screens->bottom);
            curitem = Terraria::getItem(parser.chardata.items[i].itemid, itemslist);
            curmod = Terraria::getModifier(parser.chardata.items[i].modifier, modifierlist);
            curreplaceitem = Terraria::getItem(parser.outdata.items[i].itemid, itemslist);
            curreplacemod = Terraria::getModifier(parser.outdata.items[i].modifier, modifierlist);
            curindex = Terraria::getIndex(curitem.id, parser.chardata);
            curreplaceindex = Terraria::getIndex(curreplaceitem.id, parser.outdata);

        } 
        else {
            auto& box = (i < NUM_HOTBAR_SLOTS) ? box_hotbar : box_idle;
            box.setXY(pos.x, pos.y);
            box.draw(app->screens->bottom);
        }

        //item sprite
        if (parser.outdata.items[i].itemid != 0)
        {
            GFX::Sprite2D spr = GFX::LoadSprite2D(Terraria::getSprite(parser.outdata.items[i].itemid, itemsprites), Terraria::getSpriteID(parser.outdata.items[i].itemid, itemsprites));
            spr.setXY(pos.x, pos.y);
            scaleItem(spr, 1, 40);
            spr.draw(app->screens->bottom);
        }
    }
    //hider
    GFX::Rect<int> hider = {0, 0, GFX::SCR_BTM_W, 37};
    GFX::drawRect(app->screens->bottom, hider, app->clearcol);
    hider = {0, GFX::SCR_BTM_H-2, GFX::SCR_BTM_W, 20};
    GFX::drawRect(app->screens->bottom, hider, app->clearcol);

    char str[128];
    sprintf(str, "%s%s", "Press start to save file, ", (editing ? "Press X to select item" : "Press X to edit"));

    app->fontManager.setScale(0.6);
    app->fontManager.print(app->screens->bottom, GFX::Left, 20, 10, str);

    //trash button
    trashButton.draw();

    //restore button button
    restoreButton.draw();


    //top screen
    infopanel.draw(app->screens->top);

    int yoff = 0;
    //INVENTORY
    if (editing) {
        hider = {160, 0, 240, 117};
        GFX::drawRect(app->screens->top, hider, app->clearcol);
        app->fontManager.setScale(0.6);
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "DPad L/R: change itemid\nDPad L/R + LT: change modifier\nDPad U/D: change item count\nY: type in item name\nB: type in item count\nRT: type in item modifier");
    }
    else {
        //item name
        //todo print using modifier
        app->fontManager.setScale(0.8);
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "%s%s", (parser.chardata.items[curindex].modifier != 0 ? curmod.mod.c_str() : ""), (curitem.id != 0 ? curitem.item.c_str() : "Empty"));

        //item count and modifier
        app->fontManager.setScale(0.8);
        app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "%d in inventory\nMod type: %s", parser.chardata.items[curindex].count, curmod.type.c_str());

        //item id
        app->fontManager.setScale(0.8);
        app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(%d)\nmod(%d)", curitem.id, static_cast<int>(parser.chardata.items[curindex].modifier));

        //item sprite
        if (curitem.id != 0)
        {
            GFX::Sprite2D spr = GFX::LoadSprite2D(Terraria::getSprite(curitem.id, itemsprites), Terraria::getSpriteID(curitem.id, itemsprites));
            spr.setXY(80, 63);
            scaleItem(spr, 2, 100);
            spr.draw(app->screens->top);
        }
    }
    //REPLACING
    yoff = 118;
    //item name
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "%s%s", (parser.outdata.items[curreplaceindex].modifier != 0 ? curreplacemod.mod.c_str() : ""), (curreplaceitem.id != 0 ? curreplaceitem.item.c_str() : "Empty"));
    //item count and modifier
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "%d in inventory\nMod type: %s", parser.outdata.items[curreplaceindex].count, curreplacemod.type.c_str());

    //item id
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(%d)\nmod(%d)", curreplaceitem.id, static_cast<int>(parser.outdata.items[curreplaceindex].modifier));

    //item sprite
    if (curreplaceitem.id != 0)
    {
        GFX::Sprite2D spr = GFX::LoadSprite2D(Terraria::getSprite(curreplaceitem.id, itemsprites), Terraria::getSpriteID(curreplaceitem.id, itemsprites));
        spr.setXY(80, 177);
        scaleItem(spr, 2, 100);
        spr.draw(app->screens->top);
    }

    //text
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Center, 80, 126, "Replace with:");
}

InventoryScene::~InventoryScene(void) {
	C2D_SpriteSheetFree(sprites);
    for (int i = 0; i < static_cast<int>(itemsprites.size()); i++)
	    C2D_SpriteSheetFree(itemsprites[i]);

}