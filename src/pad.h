#pragma once 

#include <3ds.h>
#include "gfx.h"

namespace Pad {
    constexpr int KEY_A = 0;
    constexpr int KEY_B = 1;
    constexpr int KEY_SELECT = 2;
    constexpr int KEY_START = 3;
    constexpr int KEY_DRIGHT = 4;
    constexpr int KEY_DLEFT = 5;
    constexpr int KEY_DUP = 6;
    constexpr int KEY_DDOWN = 7;
    constexpr int KEY_R = 8;
    constexpr int KEY_L = 9;
    constexpr int KEY_X = 10;
    constexpr int KEY_Y = 11;
    constexpr int KEY_ZL = 14;
    constexpr int KEY_ZR = 15;
    constexpr int KEY_TOUCH = 16;
    constexpr int KEY_CSTICK_RIGHT = 20;
    constexpr int KEY_CSTICK_LEFT = 21;
    constexpr int KEY_CSTICK_UP = 22;
    constexpr int KEY_CSTICK_DOWN = 23;
    constexpr int KEY_CPAD_RIGHT = 24;
    constexpr int KEY_CPAD_LEFT = 25;
    constexpr int KEY_CPAD_UP = 26;
    constexpr int KEY_CPAD_DOWN = 27;
    
    void Read(void);
    bool Held(int key);
    bool Pressed(int key);
    touchPosition GetTouchPos(void);
    bool isTouching(GFX::Rect<int> r);
}