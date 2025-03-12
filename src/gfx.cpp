#include "gfx.h"
#include "app.h"
#include <cstdarg>
#include <string>  
#include <cstring>  
#include <cstdio>

namespace GFX {

RenderScreens::RenderScreens(void) {
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

Sprite2D LoadSprite2D(C2D_SpriteSheet _sprsheet, int i) {
    Sprite2D sprite;
    sprite.sprsheet = _sprsheet;
    ASSERTFUNC(_sprsheet, "sprite sheet doesnt exist");
    int cursprite = std::min(static_cast<int>(C2D_SpriteSheetCount(sprite.sprsheet))-1, i); //dont allow using a sprite that doesnt exist
	C2D_SpriteFromSheet(&sprite.spr, sprite.sprsheet, cursprite);
	C2D_SpriteSetCenter(&sprite.spr, 0.5, 0.5);
    sprite.pos = {0, 0, static_cast<int>(sprite.spr.params.pos.w), static_cast<int>(sprite.spr.params.pos.h)};
    sprite.visible = true;
    
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

void Sprite2D::setZ(float z) {
    C2D_SpriteSetDepth(&spr, z);		
}

void Sprite2D::scale(float scale) {
    C2D_SpriteScale(&spr, scale, scale);
    pos.w = spr.params.pos.w;
    pos.h = spr.params.pos.h;
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

void FontManager::init(const char *path) {
    fontsheet = C2D_SpriteSheetLoad(path);
    fontscale = 1;
}
void FontManager::setScale(float scale) {
    fontscale = scale;
}
int FontManager::getW(const char *str) {
    int c;
    int width = 0;
    while ((c = *str++) != '\0')
    {
        //Shift and validate character
        if ((c -= 0x20) >= 0x60)
            continue;
            
        Sprite2D curchar = GFX::LoadSprite2D(fontsheet, c);
        C2D_SpriteSetCenter(&curchar.spr, 0, 0);
        curchar.scale(fontscale);
        //Add width
        width += curchar.pos.w;
    }
    return width;
}

void FontManager::print(C3D_RenderTarget* screen, Align all, int x, int y, const char *format, ...) {
    va_list list;
    
    char str[1024] = "";

    va_start(list, format);
    std::vsprintf(str, format, list);
    va_end(list);

    Sprite2D curchar = GFX::LoadSprite2D(fontsheet, 0);

    //Draw string character by character
    int c;
    int xhold = x;
    switch (all) {
        case Center:
            x -= getW(str) / 2;
            y -= (curchar.pos.h*fontscale) / 2;

            break;
        case Left:
            break;
        case Right:
            x -= getW(str);
            break;
    }

    int i = 0;      
    while ((c = str[i++]) != '\0')
    {
        curchar = GFX::LoadSprite2D(fontsheet, 0);
        curchar.scale(fontscale);

        if (c == '\n') {
            x = xhold;
            if (all == Center)
                x -= getW(str) >> 1;
            y += curchar.pos.h;
            y += 3;
            continue;
        }   
        //Shift and validate character
        if ((c -= 0x20) >= 0x60)
            continue;
        //Draw character
        curchar = GFX::LoadSprite2D(fontsheet, c);
        curchar.scale(fontscale);
        C2D_SpriteSetCenter(&curchar.spr, 0, 0);
        curchar.setXY(x, y);
        curchar.setZ(z);
        curchar.draw(screen);
        x += curchar.pos.w;
    }

    z = 0;
    setScale(1);
}

void FontManager::del(void) {
    C2D_SpriteSheetFree(fontsheet);
}

}