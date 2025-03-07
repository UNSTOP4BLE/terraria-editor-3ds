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
    void scale(float scale);
    void setXY(int x, int y);
    bool visible;
	Rect pos;
	C2D_Sprite spr;
    C2D_SpriteSheet sprsheet; //sprite sheet pointer
private:
};

Sprite2D LoadSprite2D(C2D_SpriteSheet _sprsheet, int i);

void init(void);
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
    void print(C3D_RenderTarget* screen, Align all, int x, int y, const char *format, ...);
    void del(void);
private:
    int getW(const char *str);
    void printMSG(C3D_RenderTarget * screen, int x, int y, const char *str, Align all);
    C2D_SpriteSheet fontsheet;
    float fontscale;
};
    

}