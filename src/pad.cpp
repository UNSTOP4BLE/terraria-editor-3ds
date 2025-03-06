#include "pad.h"

#include <cstdint>
#include <3ds.h>

namespace Pad {
    uint32_t kDown, kHeld, kUp;
    circlePosition circlePad;

    void Read(void) {
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();
        kUp = hidKeysUp();
        hidCircleRead(&circlePad);
    }

    bool Held(int key) {
        return (kHeld & BIT(key));
    }

    bool Pressed(int key) {
        return (kDown & BIT(key));
    }
}