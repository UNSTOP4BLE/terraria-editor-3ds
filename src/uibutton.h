#pragma once

#include "gfx.h"

class UiButton {
public:
    void init(const char *path);
    bool pressed(void);
    inline GFX::Rect<float> &pos(void) {
        return sprite.pos();
    }
    void draw(void);
    void free(void);
private:
    GFX::Sprite2D sprite;
    GFX::SpriteSheet sheet;
};