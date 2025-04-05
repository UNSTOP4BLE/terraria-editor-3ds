#pragma once
#include <citro2d.h>


namespace GFX {
constexpr int SCR_TOP_W = 400;
constexpr int SCR_TOP_H = 240;
constexpr int SCR_BTM_W = 320;
constexpr int SCR_BTM_H = 240;

template<typename T> struct Rect {
public:
    T x, y, w, h;
};

template<typename T> struct XY {
public:
    T x, y;
};

enum Align
{   
    Center,
    Left,
    Right
};

typedef C2D_SpriteSheet SpriteSheet;

class RenderScreens {
public:
    RenderScreens(void);
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
};

class Sprite2D {
public:
    void draw(C3D_RenderTarget* screen);
    void setZ(float z);
    void scale(float scale);
    inline Rect<float> &pos(void) {
        return *reinterpret_cast<Rect<float> *>(&spr.params.pos);
    }
    void setXY(float x, float y);
    bool visible;
	C2D_Sprite spr;
    C2D_SpriteSheet sprsheet; //sprite sheet pointer
private:
};

class Tex {
public:
    Tex() : tex{}, open(false) {}
    
    C2D_Image tex;
    bool open;
};

Tex loadTex(const char *path);
void freeTex(Tex *img);
XY<int> getTexWH(Tex &img);
void drawTexXY(Tex &img, C3D_RenderTarget *scr, XY<int> pos, float scale, Align all);
void drawTex(Tex &img, C3D_RenderTarget *scr, Rect<int> pos, float scale, Align all);
Sprite2D loadSprite2D(C2D_SpriteSheet _sprsheet, int i);

void init(void);
void drawRect(C3D_RenderTarget* screen, Rect<int> r, uint32_t col);
void clear(uint32_t col);
void flip(void);
void exit(void);

class FontManager {
public:
    void init(const char *path);
    void setScale(float scale);
    void print(C3D_RenderTarget* screen, Align all, float x, int y, const char *format, ...);
    void del(void);
    float z;
private:
    float getW(const char *str);
    C2D_SpriteSheet fontsheet;
    float fontscale;
};
    

}