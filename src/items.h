#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "savefile.h"
#include "gfx.h"

#define NUM_INVENTORY_ROWS 5
#define NUM_HOTBAR_SLOTS 10
#define NUM_NEGATIVE_IDS 48
#define INVENTORY_SLOT_SPACING 3

namespace Terraria {

class Item {
public:
    int id;
    std::string item;
};
class Modifier {
public:
    int id;
    std::string mod;
    std::string type;
};
void LoadItemsList(const char *path, std::vector<Item> *itemslist);
void LoadModifierList(const char *path, std::vector<Modifier> *itemslist);
Item getItem(int itemid, std::vector<Item> &itemslist);
int getItemId(const char *item, std::vector<Item> &itemslist);
int getModifierId(const char *item, std::vector<Modifier> &itemslist);
Modifier getModifier(int id, std::vector<Modifier> &itemslist);
GFX::SpriteSheet getSprite(int itemid, std::vector<GFX::SpriteSheet> &spr);
int getSpriteID(int itemid, std::vector<GFX::SpriteSheet> &spr);
int getIndex(int itemid, CharacterData &data);

}
