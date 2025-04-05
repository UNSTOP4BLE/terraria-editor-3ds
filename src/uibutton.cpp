#include "uibutton.h"
#include "gfx.h"
#include "app.h"
#include "pad.h"

void UiButton::init(const char *path) {
    sheet = C2D_SpriteSheetLoad(path);
    sprite = GFX::loadSprite2D(sheet, pressed());
}

bool UiButton::pressed(void) {
    //very bad, hardcoded to center of the sprite
    GFX::Rect<int> r = {static_cast<int>(pos().x), static_cast<int>(pos().y), static_cast<int>(pos().w), static_cast<int>(pos().h)};
    return Pad::isTouching(r);
}

void UiButton::draw(void) {
    auto r = sprite.pos();
    sprite = GFX::loadSprite2D(sheet, pressed());
    sprite.pos() = r;
    sprite.draw(app->screens->bottom);
}

void UiButton::free(void) {
    C2D_SpriteSheetFree(sheet);
}
