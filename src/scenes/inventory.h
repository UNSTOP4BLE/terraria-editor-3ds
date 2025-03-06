#pragma once
#include "../app.h"

class InventoryScene : public Scene {
public:
    InventoryScene(void);
    void update(void);
    void draw(void);
    ~InventoryScene(void); 
private:
    GFX::SpriteSheet sprites;
    GFX::Sprite2D *box_idle;
};