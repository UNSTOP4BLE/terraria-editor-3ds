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
    UiButton trashButton;
    UiButton restoreButton;
    bool editing;
    GFX::SpriteSheet sprites;
    GFX::Sprite2D box_idle;
    GFX::Sprite2D box_hotbar;
    GFX::Sprite2D box_selected;
    GFX::Sprite2D inventorypanel;
    GFX::Sprite2D scrollbar;
    GFX::Sprite2D infopanel;
    Terraria::SaveFileParser parser;
};