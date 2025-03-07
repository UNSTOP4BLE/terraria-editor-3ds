#pragma once

namespace Terraria {

constexpr int NUM_INVENTORY_SLOTS = 50;
constexpr int NUM_INVENTORY_ROWS = 5;
constexpr int NUM_HOTBAR_SLOTS = 10;
constexpr int NUM_COIN_SLOTS = 4;
constexpr int NUM_AMMO_SLOTS = 4;
constexpr int INVENTORY_SLOT_SPACING = 49;

class Item {
    int id;
    const char *item[64];
};
void LoadItemsList(const char *path);

}
