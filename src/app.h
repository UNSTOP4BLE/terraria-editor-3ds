#pragma once

#include "gfx.h"
#include "scene.h"

class Terraeditor
{
public:
    GFX::RenderScreens *screens;
    GFX::FontManager fontManager;
    Scene *currentScene;
    uint32_t clearcol;
};

extern Terraeditor *app;