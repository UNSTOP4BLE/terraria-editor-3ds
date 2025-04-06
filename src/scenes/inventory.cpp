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
    scroll = 0;
    scrollbar = {309, 45};

    invhotbar = C2D_SpriteSheetLoad("romfs:/buttons/hotbar/hotbar.t3x");
    invstandard = C2D_SpriteSheetLoad("romfs:/buttons/standard/standard.t3x");
    invcoins = C2D_SpriteSheetLoad("romfs:/buttons/coin/coin.t3x");
    invammo = C2D_SpriteSheetLoad("romfs:/buttons/ammo/ammo.t3x");
    trashButton.init("romfs:/buttons/trash/trash.t3x");
    trashButton.pos() = {9, 44, trashButton.pos().w, trashButton.pos().h};
    
    restoreButton.init("romfs:/buttons/restore/restore.t3x");
    restoreButton.pos() = {9, 94, restoreButton.pos().w, restoreButton.pos().h};

    backButton.init("romfs:/buttons/back/back.t3x");
    backButton.pos() = {3, 208, backButton.pos().w, backButton.pos().h};
    
    tex_invpanel = GFX::loadTex("romfs:/inventory/inv.png");
    tex_scroll = GFX::loadTex("romfs:/inventory/scrollbar.png");
    tex_infopanel = GFX::loadTex("romfs:/inventory/top_panel.png");

    int off = 3;
    int offy = 0;
    int w = 46;
    invgrid.init(5, NUM_INVENTORY_SLOTS/5, {64, 42}, w, off);
    offy = 49;
    coinsgrid.init(4, NUM_COIN_SLOTS/4, {64, offy+off+invgrid.getItem(NUM_INVENTORY_SLOTS-1).y+invgrid.getItem(NUM_INVENTORY_SLOTS-1).h}, w, off);
    ammogrid.init(4, NUM_AMMO_SLOTS/4, {64, offy+off+coinsgrid.getItem(NUM_COIN_SLOTS-1).y+coinsgrid.getItem(NUM_COIN_SLOTS-1).h}, w, off);

    max_scroll = ammogrid.getItem(NUM_AMMO_SLOTS-1).y - (4*(w))-1;

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
    app->fontManager.print(app->screens->top, GFX::Left, 173, 5+yoff, "%s%s", ((mod.id != 0) ? (modifier).c_str() : ""), item.name);

    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Left, 170, 32+8+yoff, "%d in slot\nModifier type:\n%s", count, mod.type);

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

Terraria::ItemsGrid InventoryScene::getGrid(int selection, int &offset, GFX::SpriteSheet &sheet) {
    Terraria::ItemsGrid grid = invgrid;
    GFX::SpriteSheet setsheet = invstandard;
    offset = 0;
    if (selection > (NUM_INVENTORY_SLOTS+NUM_COIN_SLOTS)-1) {
        grid = ammogrid;
        setsheet = invammo;
        offset = (NUM_INVENTORY_SLOTS+NUM_COIN_SLOTS);
    }
    else if (selection > NUM_INVENTORY_SLOTS-1) {
        grid = coinsgrid;
        setsheet = invcoins;
        offset = NUM_INVENTORY_SLOTS;
    }
    else if (selection < NUM_HOTBAR_SLOTS) {
        setsheet = invhotbar;
    }
    if (sheet != 0)
        sheet = setsheet;
    return grid;
}

void InventoryScene::update(void) {
    auto &item = currepitem.actualitem;
    if (editing) {
        if (Pad::Held(Pad::KEY_L) && item->id != 0) {
            int mod = item->mod;
            if (Pad::Pressed(Pad::KEY_DLEFT))
                mod -= 1;
            else if (Pad::Pressed(Pad::KEY_DRIGHT))
                mod += 1;
            item->mod = clamp(mod, 0, parser.allmodifiers.size()-1); 
            currepitem.update(item->id, item->count, item->mod, parser);
        }
        else { //items
            if (Pad::Pressed(Pad::KEY_DLEFT))
                item->id -= 1;
            else if (Pad::Pressed(Pad::KEY_DRIGHT))
                item->id += 1;
            if (Pad::Pressed(Pad::KEY_DUP))
                item->count += 1;
            else if (Pad::Pressed(Pad::KEY_DDOWN))
                item->count -= 1;

            item->id = clamp(item->id, parser.allitems[0].id, parser.allitems[parser.allitems.size()-1].id); 
            item->count = clamp(item->count, 0, INT16_MAX-1); 
            changeItem(selecteditem, item->id, true);
        }

    } else {
        int oldselection = selecteditem;
        int i = 0;
        GFX::SpriteSheet tmp = NULL;
        Terraria::ItemsGrid grid = getGrid(selecteditem, i, tmp);
      
        int curcol = (selecteditem-i)%grid.numcols;
        //input
        if (Pad::Pressed(Pad::KEY_DLEFT)) {
            if (curcol != 0)
                selecteditem --;
        }
        else if (Pad::Pressed(Pad::KEY_DDOWN)) {
            selecteditem += grid.numcols;
            if (grid.itemExists((selecteditem-i)) && (grid.getItem((selecteditem-i)).y-grid.getItem((selecteditem-i)).h)-scroll > GFX::SCR_BTM_H/2)
                scroll += grid.width+grid.offset;
        }
        else if (Pad::Pressed(Pad::KEY_DUP)) {
            selecteditem -= grid.numcols;
            if (grid.itemExists((selecteditem-i)) && (grid.getItem((selecteditem-i)).y+grid.getItem((selecteditem-i)).h)-scroll < GFX::SCR_BTM_H/2)
                scroll -= grid.width+grid.offset;
        }
        else if (Pad::Pressed(Pad::KEY_DRIGHT)) {
            if (curcol != (grid.numcols-1))
                selecteditem ++;
        }

        if (!grid.itemExists((selecteditem-i)))
            selecteditem = oldselection;
            
        //items touch selection
        for (int i = 0; i < NUM_TOTAL_SLOTS; i++) {
            int cur = 0;
            GFX::SpriteSheet tmp = NULL;
            Terraria::ItemsGrid grid = getGrid(i, cur, tmp);
            GFX::Rect<int> r = grid.getItem(i-cur);
            r.y -= scroll;
            if (Pad::isTouching(r))
                selecteditem = i;
        }
        if (selecteditem != oldselection)
            changeItem(selecteditem, parser.chardata.items[selecteditem].id, false);
    }
    //scroll
    touchPosition pos = Pad::GetTouchPos();
    GFX::Rect<int> touchbar = {309, 45, 20, 231-GFX::getTexWH(tex_scroll).y};
    int min = touchbar.y;
    int max = touchbar.h;
    if (Pad::isTouching(touchbar)) {
        scroll = map_value(pos.py, min, max, 0, max_scroll);
    }             
    scroll = clamp(scroll, 0, max_scroll);
    scrollbar.y = map_value(scroll, 0, max_scroll, min, max);  

    if (Pad::Pressed(Pad::KEY_X))
        editing = !editing;

    if (trashButton.pressed())
        changeItem(selecteditem, 0, true);

    if (restoreButton.pressed()) {
        int id = parser.chardata.items[selecteditem].id;
        currepitem.actualitem->count = curitem.actualitem->count;
        currepitem.actualitem->mod = curitem.actualitem->mod;
        changeItem(selecteditem, id, true);
    }
    //exit
    if (backButton.pressed() || Pad::Pressed(Pad::KEY_B))
        setScene(new SelectionScene());

    //save edited file
    if (Pad::Pressed(Pad::KEY_START)) {
        parser.writeFile(parser.inputpath.c_str()); //overwrite
        setScene(new SavingScene());
    }
}

void InventoryScene::draw(void) {
    GFX::drawTexXY(tex_invpanel, app->screens->bottom, {60, 37}, 1, GFX::Left);
    GFX::drawTexXY(tex_scroll, app->screens->bottom, scrollbar, 1, GFX::Left);

    //items 
    for (int i = 0; i < NUM_TOTAL_SLOTS; i++) {
        Terraria::ItemsGrid grid;
        GFX::SpriteSheet sheet = invstandard;
        int cur;
        grid = getGrid(i, cur, sheet);
        cur = i-cur;
        GFX::Rect<int> r = grid.getItem(cur);
        int sprite = 0;
        r.y -= scroll;
        if (i == selecteditem)
            sprite = 2;
        if (parser.outdata.items[i].id != 0)
            sprite += 1;

        GFX::Sprite2D spr = GFX::loadSprite2D(sheet, sprite);
        spr.pos() = {static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.w), static_cast<float>(r.h)};
        spr.draw(app->screens->bottom);
        GFX::drawTexXY(tex_invitems[i], app->screens->bottom, {r.x+r.w/2, r.y+r.h/2}, scaleItem(GFX::getTexWH(tex_invitems[i]), 1, 40), GFX::Center);
    }

    //coins text
    app->fontManager.setScale(0.9);
    int y = coinsgrid.getItem(0).y-35;
    app->fontManager.print(app->screens->bottom, GFX::Left, invgrid.getItem(0).x+2, y-scroll, "Coins:");

    //ammo text
    app->fontManager.setScale(0.9);
    y = ammogrid.getItem(0).y-35;
    app->fontManager.print(app->screens->bottom, GFX::Left, invgrid.getItem(0).x+2, y-scroll, "Ammo:");

    //hider
    GFX::drawRect(app->screens->bottom, {0, 0, GFX::SCR_BTM_W, 37}, app->clearcol);
    GFX::drawRect(app->screens->bottom, {0, GFX::SCR_BTM_H-2, GFX::SCR_BTM_W, 20}, app->clearcol);

    //help text
    char str[128];
    sprintf(str, "%s%s\nPress B or tap back to exit without saving", "Press Start to save file, ", (editing ? "Press X to select item" : "Press X to edit"));
    app->fontManager.setScale(0.5);
    app->fontManager.print(app->screens->bottom, GFX::Left, 20, 5, str);

    //buttons
    trashButton.draw();
    restoreButton.draw();
    backButton.draw();

    //top screen
    GFX::drawTexXY(tex_infopanel, app->screens->top, {0, 0}, 1, GFX::Left);

    //INVENTORY
    if (editing) {
        GFX::drawRect(app->screens->top, {160, 0, 240, 117}, app->clearcol);
        app->fontManager.setScale(0.6);
        app->fontManager.print(app->screens->top, GFX::Left, 173, 5, "DPad L/R: change itemid\nDPad L/R + LT: change modifier\nDPad U/D: change item count\nY: type in item name(todo)\nB: type in item count(todo)\nRT: type in item modifier(todo)");
    }
    else {
        //item info
        printItemInfo(0, curitem.actualitem->id, curitem.item, curitem.mod, curitem.actualitem->count);
    }
    GFX::drawTexXY(curitem.tex, app->screens->top, {80,63}, scaleItem(GFX::getTexWH(curitem.tex), 2, 100), GFX::Center);
    //REPLACING
    printItemInfo(118, currepitem.actualitem->id, currepitem.item, currepitem.mod, currepitem.actualitem->count);
    GFX::drawTexXY(currepitem.tex, app->screens->top, {80,177}, scaleItem(GFX::getTexWH(currepitem.tex), 2, 100), GFX::Center);

    //text
    app->fontManager.setScale(0.8);
    app->fontManager.print(app->screens->top, GFX::Center, 80, 126, "Replace with:");
}

InventoryScene::~InventoryScene(void) {
    C2D_SpriteSheetFree(invhotbar);
    C2D_SpriteSheetFree(invstandard);
    C2D_SpriteSheetFree(invcoins);
    C2D_SpriteSheetFree(invammo);
    trashButton.free();
    restoreButton.free();
    backButton.free();
    GFX::freeTex(&tex_invpanel);
    GFX::freeTex(&tex_scroll);
    GFX::freeTex(&tex_infopanel);
    GFX::freeTex(&curitem.tex);
    GFX::freeTex(&currepitem.tex);

    for (int i = 0; i < NUM_TOTAL_SLOTS; i++)
        GFX::freeTex(&tex_invitems[i]);
}