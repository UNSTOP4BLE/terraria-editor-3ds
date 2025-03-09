#pragma once
#include "../app.h"
#include "../savefile.h"
#include "../items.h"

class InventoryScene : public Scene {
public:
    InventoryScene(void);
    void update(void);
    void draw(void);
    ~InventoryScene(void); 
private:
    int selection;
    int scroll;
    std::vector<Terraria::Item> itemslist;
    GFX::SpriteSheet sprites;
    GFX::Sprite2D box_idle;
    GFX::Sprite2D box_hotbar;
    GFX::Sprite2D box_selected;
    GFX::Sprite2D inventorypanel;
    GFX::Sprite2D scrollbar;
    GFX::Sprite2D infopanel;
    Terraria::SaveFileParser parser;
};