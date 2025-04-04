#pragma once
#include "../app.h"
#include "../savefile.h"
#include "../items.h"
#include "../uibutton.h"
#include <vector>

class InventoryScene : public Scene {
public:
    InventoryScene(std::u16string path);
    void update(void);
    void draw(void);
    ~InventoryScene(void); 
private:
    void printItemInfo(int yoff);
    void changeItem(int id);
    float scaleItem(GFX::XY<int> wh, float scl, int max);

    UiButton trashButton;
    UiButton restoreButton;
    bool editing;
    Terraria::SaveFileParser parser;

    C2D_Image tex_invpanel;
    C2D_Image tex_scroll;
    C2D_Image tex_infopanel;
    C2D_Image tex_curitem;
};