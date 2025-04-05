#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "savefile.h"
#include "gfx.h"

namespace Terraria {

Item getItem(int id, SaveFileParser &parser);
}
