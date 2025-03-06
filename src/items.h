#pragma once

namespace Terraria {

constexpr int NUM_INVENTORY_SLOTS = 50;
constexpr int NUM_COIN_SLOTS = 4;
constexpr int NUM_AMMO_SLOTS = 4;

class Item {
    int id;
    const char *item[64];
};
void LoadItemsList(const char *path);

}
