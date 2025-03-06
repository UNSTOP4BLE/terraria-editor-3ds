#include "gfx.h"
#include "app.h"

namespace GFX {

RenderScreens::RenderScreens(void) {
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

Sprite2D LoadSprite2D(C2D_SpriteSheet _sprsheet, int i) {
    Sprite2D sprite;
    sprite.sprsheet = _sprsheet;
    int cursprite = std::min(static_cast<int>(C2D_SpriteSheetCount(sprite.sprsheet))-1, i); //dont allow using a sprite that doesnt exist
	C2D_SpriteFromSheet(&sprite.spr, sprite.sprsheet, cursprite);
	C2D_SpriteSetCenter(&sprite.spr, 0.5, 0.5);
    sprite.pos = {0, 0, static_cast<int>(sprite.spr.params.pos.w), static_cast<int>(sprite.spr.params.pos.h)};
    return sprite;
}

void Sprite2D::draw(C3D_RenderTarget* screen) {
    spr.params.pos.x = pos.x;
    spr.params.pos.y = pos.y;
    spr.params.pos.w = pos.w;
    spr.params.pos.h = pos.h;
    if (visible)
    {
        C2D_SceneBegin(screen);
    	C2D_DrawSprite(&spr);
    }
}

void Sprite2D::setXY(int x, int y) {
    pos = {x, y, pos.w, pos.h};
}

void init(void) {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
    app->screens = new RenderScreens();
    app->clearcol = C2D_Color32(0, 255, 0, 255);

}

void drawRect(C3D_RenderTarget* screen, Rect r, uint32_t col) {
    C2D_SceneBegin(screen);
	C2D_DrawRectangle(r.x, r.y, 0, r.w, r.h, col, col, col, col);

}

void clear(uint32_t col) {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(app->screens->top, col);
	C2D_TargetClear(app->screens->bottom, col);
    C2D_SceneBegin(app->screens->top);
    C2D_SceneBegin(app->screens->bottom);
}

void flip(void) {
    C3D_FrameEnd(0);
}

void exit(void) {
	C2D_Fini();
	C3D_Fini();
	gfxExit();
    delete app->screens;
}

}