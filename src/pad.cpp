#include "pad.h"

#include <cstdint>

namespace Pad {
    uint32_t kDown, kHeld;
    circlePosition circlePad;
	touchPosition touch;

    void Read(void) {
		hidScanInput();
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();
        hidCircleRead(&circlePad);
		hidTouchRead(&touch);
    }

    bool Held(int key) {
        return (kHeld & BIT(key));
    }

    bool Pressed(int key) {
        return (kDown & BIT(key));
    }

    touchPosition GetTouchPos(void) {
        return touch;
    }
    bool isTouching(GFX::Rect r) {
        return (touch.px >= r.x && touch.py >= r.y && 
                touch.px <= r.x+r.w && touch.py <= r.y+r.h);
    }
}