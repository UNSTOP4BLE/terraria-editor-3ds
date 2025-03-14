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
    sprite.spr.params.pos = {0, 0, sprite.spr.params.pos.w, sprite.spr.params.pos.h};
	C2D_SpriteSetCenter(&sprite.spr, 0.5, 0.5);
    sprite.visible = true;
    
    return sprite;
}

void Sprite2D::draw(C3D_RenderTarget* screen) {
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
    pos().w *= scale;
    pos().h *= scale;
}

void Sprite2D::setXY(float x, float y) {
    pos().x = x; 
    pos().y = y;
}

void init(void) {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
    app->screens = new RenderScreens();
    app->clearcol = C2D_Color32(0, 255, 0, 255);

}

void drawRect(C3D_RenderTarget* screen, Rect<int> r, uint32_t col) {
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
float FontManager::getW(const char *str) {
    int c;
    float width = 0;
    float maxwidth = 0;
    while ((c = *str++) != '\0')
    {
        //Shift and validate character
        if (c == '\n') {
            if (width > maxwidth)
                maxwidth = width;
            width = 0;
            continue;
        }

        if ((c -= 0x20) >= 0x60)
            continue;
            
        Sprite2D curchar = GFX::LoadSprite2D(fontsheet, c);
        C2D_SpriteSetCenter(&curchar.spr, 0, 0);
        curchar.scale(fontscale);
        //add width
        width += curchar.pos().w;
    }
    return static_cast<float>(std::max(width, maxwidth));
}

void FontManager::print(C3D_RenderTarget* screen, Align all, float x, int y, const char *format, ...) {
    va_list list;
    char str[1024] = "";

    va_start(list, format);
    std::vsprintf(str, format, list);
    va_end(list);

    Sprite2D curchar = GFX::LoadSprite2D(fontsheet, 0);

    float strw = getW(str);
    //Draw string character by character
    int c;
    float xhold = x;
    switch (all) {
        case Center:
            x -= strw / 2;
            y -= (curchar.pos().h / 2);
            break;
        case Left:
            break;
        case Right:
            x -= strw;
            break;
    }

    int scrw = 0;
    if (screen == app->screens->top)
        scrw = SCR_TOP_W;
    else
        scrw = SCR_BTM_W;
    float maxscl = fontscale;
    auto available = scrw - x;
    
    float newscale = (available / strw);
    if (newscale < 1.0f) {
        fontscale = std::min(newscale*maxscl, maxscl);
    }

    int i = 0;      
    while ((c = str[i++]) != '\0') {
        if (c == '\n') {
            x = xhold;
            if (all == Center)
                x -= strw / 2;
            else if (all == Right)
                x -= strw;
            y += curchar.pos().h;
            continue;
        }   
        //Shift and validate character
        if ((c -= 0x20) >= 0x60)
            continue;
        //Draw character
        curchar = GFX::LoadSprite2D(fontsheet, c);
        C2D_SpriteSetCenter(&curchar.spr, 0, 0);
        curchar.setXY(x, y);
        curchar.scale(fontscale);
        curchar.setZ(z);
        curchar.draw(screen);
        x += curchar.pos().w;
    }

    z = 0;
    setScale(1);
}

void FontManager::del(void) {
    C2D_SpriteSheetFree(fontsheet);
}

}