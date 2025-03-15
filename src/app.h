#pragma once

#include "gfx.h"
#include "scene.h"
#include "keyboard.h"

class Terraeditor
{
public:
    GFX::RenderScreens *screens;
    GFX::FontManager fontManager;
    KeyBoard keyboard;
    Scene *currentScene;
    uint32_t clearcol;
    double deltatime;
    int elapsed;
};

void ErrMSG(const char *filename, const char *function, int line, const char *expr, const char *msg);
#define ASSERTFUNC(expr, msg) ((expr) ? ((void) 0) : ErrMSG(__FILE__, __func__, __LINE__, #expr, msg))

extern Terraeditor *app;