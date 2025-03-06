#pragma once

#include "gfx.h"

class Terraeditor
{
public:
    GFX::RenderScreens *screens;
    uint32_t clearcol;
	touchPosition touch;
};

extern Terraeditor *app;