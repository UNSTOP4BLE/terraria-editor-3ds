#pragma once

#include "gfx.h"
#include "scene.h"

class Terraeditor
{
public:
    GFX::RenderScreens *screens;
    Scene *currentScene;
    uint32_t clearcol;
	touchPosition touch;
};

extern Terraeditor *app;