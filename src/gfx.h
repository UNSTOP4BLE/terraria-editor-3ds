#pragma once
#include <citro2d.h>


namespace GFX {
constexpr int SCR_TOP_W = 400;
constexpr int SCR_TOP_H = 240;
constexpr int SCR_BTM_W = 320;
constexpr int SCR_BTM_H = 240;

struct Rect {
    int x,y,w,h;
};
struct FRect {
    float x,y,w,h;
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
    void setXY(float x, float y);
    bool visible;
	FRect *pos;
	C2D_Sprite spr;
    C2D_SpriteSheet sprsheet; //sprite sheet pointer
private:
};

Sprite2D LoadSprite2D(C2D_SpriteSheet _sprsheet, int i);

void init(void);
void drawRect(C3D_RenderTarget* screen, Rect r, uint32_t col);
void clear(uint32_t col);
void flip(void);
void exit(void);

enum Align
{   
    Center,
    Left,
    Right
};

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