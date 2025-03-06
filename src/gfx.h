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
    Sprite2D(C2D_SpriteSheet _sprsheet, int i);
    void draw(C3D_RenderTarget* screen);
    void setXY(int x, int y);
    bool visible;
	Rect pos;
private:
	C2D_Sprite spr;
    C2D_SpriteSheet sprsheet; //sprite sheet pointer
};


void init(void);
void clear(uint32_t col);
void flip(void);
void exit(void);

}