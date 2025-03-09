#pragma once
#include <cstdint>

namespace Terraria {

constexpr int NUM_INVENTORY_SLOTS = 50;
constexpr int NUM_INVENTORY_ROWS = 5;
constexpr int NUM_HOTBAR_SLOTS = 10;
constexpr int NUM_COIN_SLOTS = 4;
constexpr int NUM_AMMO_SLOTS = 4;
constexpr int INVENTORY_SLOT_SPACING = 49;

struct [[gnu::packed]] InternalItem {
    int16_t itemid;
    uint8_t count; //count of item
    uint8_t idk2; //weapon attribute maybe?
    uint8_t idk3; //no idea what this is
};

class Item {
    int id;
    const char *item[64];
};
void LoadItemsList(const char *path);

}
