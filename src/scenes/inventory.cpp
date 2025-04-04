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
    trashButton.pos().x = 9;
    trashButton.pos().y = 44;
    
    restoreButton.init("romfs:/restore.t3x");
    restoreButton.pos().x = 9;
    restoreButton.pos().y = 94;
    
    tex_invpanel = GFX::loadTex("romfs:/inventory/inv.png");
    tex_scroll = GFX::loadTex("romfs:/inventory/scrollbar.png");
    tex_infopanel = GFX::loadTex("romfs:/inventory/top_panel.png");
    tex_curitem = GFX::loadTex("romfs:/inventory/top_panel.png"); //temporary

    //load save
    parser.readFile(path.c_str());
    changeItem(0);
}

static int clamp(int d, int min, int max) {
    const int t = d < min ? min : d;
    return t > max ? max : t;
}

static float map_value(float value, float input_min, float input_max, float output_min, float output_max) {
    return (value - input_min) / (input_max - input_min) * (output_max - output_min) + output_min;
}

//todo
void InventoryScene::printItemInfo(int yoff) {
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "replacementitemname");

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "x in inventory\nMod type: x");

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(x)\nmod(x)");
}

void InventoryScene::changeItem(int id) {
    GFX::freeTex(&tex_curitem);
    char itmpath[64];
    sprintf(itmpath, "romfs:/items/Item_%d.png", id);
    tex_curitem = GFX::loadTex(itmpath);
}

float InventoryScene::scaleItem(GFX::XY<int> wh, float scl, int max) {
    float maxw = std::max(wh.x, wh.y);
    return clamp(static_cast<int>(scl * maxw), 0, max) / maxw;
}

void InventoryScene::update(void) {
    if (editing) {
        if (Pad::Held(Pad::KEY_L)) {// && parser.outdata.items[index].itemid != 0) {

        }
    } else {

    }


    if (Pad::Pressed(Pad::KEY_X))
        editing = !editing;
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
    GFX::drawTexXY(tex_invpanel, app->screens->bottom, {60, 37}, 1, GFX::Left);
    GFX::drawTexXY(tex_scroll, app->screens->bottom, {309, 42}, 1, GFX::Left);

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
    GFX::drawTexXY(tex_infopanel, app->screens->top, {0, 0}, 1, GFX::Left);

    //INVENTORY
    if (editing) {
        GFX::drawRect(app->screens->top, {160, 0, 240, 117}, app->clearcol);
        app->fontManager.setScale(0.6);
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "DPad L/R: change itemid\nDPad L/R + LT: change modifier\nDPad U/D: change item count\nY: type in item name\nB: type in item count\nRT: type in item modifier");
    }
    else {
        //item info
        printItemInfo(0);
        GFX::drawTexXY(tex_curitem, app->screens->top, {80,63}, scaleItem(GFX::getTexWH(tex_curitem), 2, 100), GFX::Center);
    }
    //REPLACING
    printItemInfo(118);
    //todo replace curitem with curreplaceitem later
    GFX::drawTexXY(tex_curitem, app->screens->top, {80,177}, scaleItem(GFX::getTexWH(tex_curitem), 2, 100), GFX::Center);

    //text
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Center, 80, 126, "Replace with:");
}

InventoryScene::~InventoryScene(void) {
    trashButton.free();
    restoreButton.free();
    GFX::freeTex(&tex_invpanel);
    GFX::freeTex(&tex_scroll);
    GFX::freeTex(&tex_infopanel);
    GFX::freeTex(&tex_curitem);
}