#pragma once
#include "../app.h"
#include "../savefile.h"
#include "../items.h"
#include <vector>

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
    std::vector<Terraria::Modifier> modifierlist;
    Terraria::Item curitem;
    Terraria::Item curreplaceitem;
    Terraria::Modifier curmod;
    Terraria::Modifier curreplacemod;
    bool editing;
    GFX::SpriteSheet sprites;
    std::vector<GFX::SpriteSheet> itemsprites;
    GFX::Sprite2D box_idle;
    GFX::Sprite2D box_hotbar;
    GFX::Sprite2D box_selected;
    GFX::Sprite2D inventorypanel;
    GFX::Sprite2D scrollbar;
    GFX::Sprite2D infopanel;
    Terraria::SaveFileParser parser;
};