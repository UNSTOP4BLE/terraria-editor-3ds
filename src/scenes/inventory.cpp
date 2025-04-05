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
    selecteditem = 0;

    trashButton.init("romfs:/trash.t3x");
    trashButton.pos() = {9, 44, trashButton.pos().w, trashButton.pos().h};
    
    restoreButton.init("romfs:/restore.t3x");
    restoreButton.pos() = {9, 94, restoreButton.pos().w, restoreButton.pos().h};
    
    tex_invpanel = GFX::loadTex("romfs:/inventory/inv.png");
    tex_scroll = GFX::loadTex("romfs:/inventory/scrollbar.png");
    tex_infopanel = GFX::loadTex("romfs:/inventory/top_panel.png");

    int off = 3;
    int w = 46;
    invgrid.init(5, NUM_INVENTORY_SLOTS/5, {64, 42}, w, off);
//    coinsgrid.init(5, NUM_COIN_SLOTS/4, {?, ?}, w, off);
  //  ammogrid.init(5, NUM_AMMO_SLOTS/4, {?, ?}, w, off);

    //load save
    parser.init();
    parser.readFile(path.c_str());

    for (int i = 0; i < NUM_TOTAL_SLOTS; i++) {
        char itmpath[64];
        int id = parser.chardata.items[i].id;
        if (id == 0)
            sprintf(itmpath, "romfs:/items/Item_empty.png");
        else
            sprintf(itmpath, "romfs:/items/Item_%d.png", Terraria::getItem(id,parser).id);
        tex_invitems[i] = GFX::loadTex(itmpath);
    }

    //initialize pointers
    changeItem(0, 0, false);
}

static int clamp(int d, int min, int max) {
    const int t = d < min ? min : d;
    return t > max ? max : t;
}

static float map_value(float value, float input_min, float input_max, float output_min, float output_max) {
    return (value - input_min) / (input_max - input_min) * (output_max - output_min) + output_min;
}

void InventoryScene::printItemInfo(int yoff, int id, Terraria::Item item, Terraria::Modifier mod, int count) {
    app->fontManager.setScale(0.8);
    std::string modifier = mod.name;
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "%s%s", (mod.id ? (modifier+" ").c_str() : ""), item.name);

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "%d in inventory\nMod type: %s", count, mod.type);

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Right, GFX::SCR_TOP_W-16, 32+8+yoff, "id(%d)\nmod(%d)", id, mod.id);
}

void InventoryScene::changeItem(int slot, int id, bool replace) {
    char itmpath[64];
    //item
    curitem.actualitem = &parser.chardata.items[slot];
    curitem.update(curitem.actualitem->id, curitem.actualitem->count, curitem.actualitem->mod, parser);
    sprintf(itmpath, "romfs:/items/Item_%d.png", curitem.item.id);
    if (curitem.actualitem->id == 0)
        sprintf(itmpath, "romfs:/items/Item_empty.png");
    GFX::freeTex(&curitem.tex);
    curitem.tex = GFX::loadTex(itmpath);
    //replace item
    currepitem.actualitem = &parser.outdata.items[slot];
    if (replace)   
        currepitem.actualitem->id = id;
    int repid = currepitem.actualitem->id;

    currepitem.update(repid, currepitem.actualitem->count, currepitem.actualitem->mod, parser);
    sprintf(itmpath, "romfs:/items/Item_%d.png", currepitem.item.id);
    if (repid == 0)
        sprintf(itmpath, "romfs:/items/Item_empty.png");
    GFX::freeTex(&currepitem.tex);
    currepitem.tex = GFX::loadTex(itmpath);
    if (replace) {
        GFX::freeTex(&tex_invitems[slot]);
        tex_invitems[slot] = GFX::loadTex(itmpath);
    }
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
        int oldselection = selecteditem;
        int curcol = selecteditem%invgrid.numcols;
        if (Pad::Pressed(Pad::KEY_DLEFT)) {// && parser.outdata.items[index].itemid != 0) {
            if (curcol != 0)
                selecteditem --;
        }
        else if (Pad::Pressed(Pad::KEY_DDOWN)) {// && parser.outdata.items[index].itemid != 0) {
            selecteditem += invgrid.numcols;
        }
        else if (Pad::Pressed(Pad::KEY_DUP)) {// && parser.outdata.items[index].itemid != 0) {
            selecteditem -= invgrid.numcols;
        }
        else if (Pad::Pressed(Pad::KEY_DRIGHT)) {// && parser.outdata.items[index].itemid != 0) {
            if (curcol != (invgrid.numcols-1))
                selecteditem ++;
        }

        if (!invgrid.itemExists(selecteditem))
            selecteditem = oldselection;
        changeItem(selecteditem, parser.chardata.items[selecteditem].id, false);
    }


    if (Pad::Pressed(Pad::KEY_X))
        editing = !editing;
    //exit
    if (Pad::Pressed(Pad::KEY_B))
        setScene(new SelectionScene());

    if (trashButton.pressed())
        changeItem(selecteditem, 0, true);

    if (restoreButton.pressed()) {
        int id = parser.chardata.items[selecteditem].id;
        changeItem(selecteditem, id, true);
    }
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

    for (int i = 0; i < NUM_INVENTORY_SLOTS; i++) {
        int item = i;
        if (i == selecteditem)
            GFX::drawRect(app->screens->bottom, invgrid.getItem(item), C2D_Color32(255, 0, 0, 255));
        else
            GFX::drawRect(app->screens->bottom, invgrid.getItem(item), C2D_Color32(0, 0, 0, 255));

        GFX::drawTexXY(tex_invitems[i], app->screens->bottom, {invgrid.getItem(item).x+invgrid.getItem(item).w/2, invgrid.getItem(item).y+invgrid.getItem(item).h/2}, scaleItem(GFX::getTexWH(curitem.tex), 1, 40), GFX::Center);
    }
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
        printItemInfo(0, curitem.actualitem->id, curitem.item, curitem.mod, curitem.actualitem->count);
        GFX::drawTexXY(curitem.tex, app->screens->top, {80,63}, scaleItem(GFX::getTexWH(curitem.tex), 2, 100), GFX::Center);
    }
    //REPLACING
    printItemInfo(118, currepitem.actualitem->id, currepitem.item, currepitem.mod, currepitem.actualitem->count);
    GFX::drawTexXY(currepitem.tex, app->screens->top, {80,177}, scaleItem(GFX::getTexWH(currepitem.tex), 2, 100), GFX::Center);

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
    GFX::freeTex(&curitem.tex);
    GFX::freeTex(&currepitem.tex);

    for (int i = 0; i < NUM_TOTAL_SLOTS; i++)
        GFX::freeTex(&tex_invitems[i]);
}