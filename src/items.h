#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "savefile.h"

namespace Terraria {

constexpr int NUM_INVENTORY_SLOTS = 50;
constexpr int NUM_INVENTORY_ROWS = 5;
constexpr int NUM_HOTBAR_SLOTS = 10;
constexpr int NUM_COIN_SLOTS = 4;
constexpr int NUM_AMMO_SLOTS = 4;
constexpr int INVENTORY_SLOT_SPACING = 49;

class Item {
public:
    int id;
    std::string item;
};
void LoadItemsList(const char *path, std::vector<Item> *itemslist);
Item getItem(int itemid, std::vector<Item> &itemslist);
int getIndex(int itemid, CharacterData &data);

}
