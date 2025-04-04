#define STB_IMAGE_IMPLEMENTATION

#include "gfx.h"
#include "app.h"
#include "stb_image.h"
#include <cstdarg>
#include <cmath>

namespace GFX {

RenderScreens::RenderScreens(void) {
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

static int pow2(int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return (v >= 64 ? v : 64);
}

static int revertPow2(int v) {
    if (v <= 1) return 0; 

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return (v + 1) >> 1;
}

  
static void convertImage(C3D_Tex *tex, Tex3DS_SubTexture *subtex, uint8_t *buf, int size, int width, int height, GPU_TEXCOLOR format) {
    // RGBA -> ABGR
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < height; col++) {
            uint32_t z = (row + col * width) * 4;

            uint8_t r = *(uint8_t *)(buf + z);
            uint8_t g = *(uint8_t *)(buf + z + 1);
            uint8_t b = *(uint8_t *)(buf + z + 2);
            uint8_t a = *(uint8_t *)(buf + z + 3);

            *(buf + z) = a;
            *(buf + z + 1) = b;
            *(buf + z + 2) = g;
            *(buf + z + 3) = r;
        }
    }  
    int w_pow2 = pow2(width);
    int h_pow2 = pow2(height);

    subtex->width = width;
    subtex->height = height;
    subtex->left = 0.0f;
    subtex->top = 1.0f;
    subtex->right = (width / (float)w_pow2);
    subtex->bottom = 1.0 - (height / (float)h_pow2);

    C3D_TexInit(tex, w_pow2, h_pow2, format);
    C3D_TexSetFilter(tex, GPU_NEAREST, GPU_NEAREST);

    int pixel_size = (size / width / height);

    memset(tex->data, 0, tex->size);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int dst_pos =
            ((((y >> 3) * (w_pow2 >> 3) + (x >> 3)) << 6) +
            ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
            ((x & 4) << 2) | ((y & 4) << 3))) *
            pixel_size;
            int src_pos = (y * width + x) * pixel_size;

            memcpy(&(static_cast<uint8_t *>(tex->data))[dst_pos],
            &(static_cast<uint8_t *>(buf))[src_pos], pixel_size);
        }
    }

    C3D_TexFlush(tex);

    tex->border = 0x00000000;
    C3D_TexSetWrap(tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
    linearFree(buf);
}

template <typename T>
static XY<T> alignPos(Rect<T> pos, Align all) {
    switch (all) {
        case Center:
            pos.x -= pos.w / 2;
            pos.y -= pos.h / 2;
            break;
        case Left:
            break;
        case Right:
            pos.x -= pos.w;
            break;
    }
    return XY<T>{pos.x, pos.y};
}

C2D_Image loadTex(const char *path) {
    C2D_Image img;
    int w, h, c;
    uint8_t *buffer = static_cast<uint8_t *>(stbi_load(path, &w, &h, &c, 4));
      
    ASSERTFUNC(buffer, "failed to load image");
    ASSERTFUNC(!(w > 1024 || h > 1024), "loaded image too big");
          
    C3D_Tex *tex = new C3D_Tex;
    Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture;
    convertImage(tex, subtex, buffer, (w * h * 4), w, h, GPU_RGBA8);
    img.tex = tex;
    img.subtex = subtex;
    stbi_image_free(buffer);
    return img;
}

void freeTex(C2D_Image *img) {
    delete img->tex;
    delete img->subtex;
}

XY<int> getTexWH(C2D_Image &img) {
    return {revertPow2(img.tex->width), revertPow2(img.tex->height)};
}

void drawTexXY(C2D_Image &img, C3D_RenderTarget *scr, XY<int> pos, float scale, Align all) {
    Rect<int> srect = {pos.x, pos.y, static_cast<int>(getTexWH(img).x*scale), static_cast<int>(getTexWH(img).y*scale)};
    XY<int> dpos = alignPos(srect, all);

    C2D_SceneBegin(scr);
    C2D_DrawImageAt(img, dpos.x, dpos.y, 0, nullptr, scale, scale);
}

void drawTex(C2D_Image &img, C3D_RenderTarget *scr, Rect<int> pos, float scale, Align all) {
    XY<float> newscale = {static_cast<float>(pos.w) / getTexWH(img).x, static_cast<float>(pos.h)/getTexWH(img).y};
    newscale.x *= scale;
    newscale.y *= scale;

    Rect<int> srect = {pos.x, pos.y, static_cast<int>(getTexWH(img).x*newscale.x), static_cast<int>(getTexWH(img).y*newscale.y)};
    XY<int> dpos = alignPos(srect, all);

    C2D_SceneBegin(scr);
    C2D_DrawImageAt(img, dpos.x, dpos.y, 0, nullptr, newscale.x, newscale.y);
}

Sprite2D loadSprite2D(C2D_SpriteSheet _sprsheet, int i) {
    Sprite2D sprite;
    sprite.sprsheet = _sprsheet;
    ASSERTFUNC(_sprsheet, "sprite sheet doesnt exist");
    int cursprite = std::min(static_cast<int>(C2D_SpriteSheetCount(sprite.sprsheet))-1, i); //dont allow using a sprite that doesnt exist
	C2D_SpriteFromSheet(&sprite.spr, sprite.sprsheet, cursprite);
    sprite.spr.params.pos = {0, 0, sprite.spr.params.pos.w, sprite.spr.params.pos.h};
	C2D_SpriteSetCenter(&sprite.spr, 0, 0);
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
            
        Sprite2D curchar = GFX::loadSprite2D(fontsheet, c);
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

    Sprite2D curchar = GFX::loadSprite2D(fontsheet, 0);

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
        curchar = GFX::loadSprite2D(fontsheet, c);
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