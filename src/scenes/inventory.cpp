#include "inventory.h"
#include "../pad.h"
#include "fileselection.h"
#include "saving.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

InventoryScene::InventoryScene(std::u16string path) {
    setSceneCol(C2D_Color32(51, 85, 153, 255));
    editing = false;

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

    //load save
    parser.readFile(path.c_str());
}

int clamp(int d, int min, int max) {
    const int t = d < min ? min : d;
    return t > max ? max : t;
}

float map_value(float value, float input_min, float input_max, float output_min, float output_max) {
    return (value - input_min) / (input_max - input_min) * (output_max - output_min) + output_min;
}

//todo
void printItemInfo(int yoff) {
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "replacementitemname");

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "x in inventory\nMod type: x");

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(x)\nmod(x)");
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
    //exit
    if (Pad::Pressed(Pad::KEY_B))
        setScene(new SelectionScene());

    //save edited file
    if (Pad::Pressed(Pad::KEY_START)) {
        parser.writeFile(parser.inputpath.c_str()); //overwrite
        setScene(new SavingScene());
    }
}

void InventoryScene::draw(void) {
    inventorypanel.draw(app->screens->bottom);
    scrollbar.draw(app->screens->bottom);

    //hider
    GFX::drawRect(app->screens->bottom, {0, 0, GFX::SCR_BTM_W, 37}, app->clearcol);
    GFX::drawRect(app->screens->bottom, {0, GFX::SCR_BTM_H-2, GFX::SCR_BTM_W, 20}, app->clearcol);

    //help text
    char str[128];
    sprintf(str, "%s%s\nPress B to exit without saving", "Press Start to save file, ", (editing ? "Press X to select item" : "Press X to edit"));
    app->fontManager.setScale(0.5);
    app->fontManager.print(app->screens->bottom, GFX::Left, 20, 5, str);

    //buttons
    trashButton.draw();
    restoreButton.draw();

    //top screen
    infopanel.draw(app->screens->top);

    //INVENTORY
    if (editing) {
        GFX::drawRect(app->screens->top, {160, 0, 240, 117}, app->clearcol);
        app->fontManager.setScale(0.6);
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "DPad L/R: change itemid\nDPad L/R + LT: change modifier\nDPad U/D: change item count\nY: type in item name\nB: type in item count\nRT: type in item modifier");
    }
    else {
        //item name
        printItemInfo(0);
        /*
        //item sprite
        if (curitem.id != 0)
        {
            GFX::Sprite2D spr = GFX::LoadSprite2D(Terraria::getSprite(curitem.id, itemsprites), Terraria::getSpriteID(curitem.id, itemsprites));
            spr.setXY(80, 63);
            scaleItem(spr, 2, 100);
            spr.draw(app->screens->top);
        }*/
    }
    //REPLACING
    printItemInfo(118);
    /*
    //item sprite
    if (curreplaceitem.id != 0)
    {
        GFX::Sprite2D spr = GFX::LoadSprite2D(Terraria::getSprite(curreplaceitem.id, itemsprites), Terraria::getSpriteID(curreplaceitem.id, itemsprites));
        spr.setXY(80, 177);
        scaleItem(spr, 2, 100);
        spr.draw(app->screens->top);
    }*/

    //text
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Center, 80, 126, "Replace with:");
}

InventoryScene::~InventoryScene(void) {
	C2D_SpriteSheetFree(sprites);
}