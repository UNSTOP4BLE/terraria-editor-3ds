#include "items.h"
#include "app.h"
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <iostream>


// Function to remove spaces from a string
std::string removeSpaces(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    return result;
}

// Function to convert string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool compareStrings(const std::string& str1, const std::string& str2) {
    std::string modifiedStr1 = toLowerCase(removeSpaces(str1));
    std::string modifiedStr2 = toLowerCase(removeSpaces(str2));

    return modifiedStr1 == modifiedStr2;
}

namespace Terraria {
    
void LoadItemsList(const char *path, std::vector<Item> *itemslist) {
    FILE *file = fopen(path, "r");
    ASSERTFUNC(file, "failed to open file");

    int BUF_SIZE = 64;

    char line[BUF_SIZE] = "";
    while (fgets(line, BUF_SIZE, file) != NULL) {	
        char itemid[16] = "";
        char itemname[64] = "";
    	for (int i = 0; i < BUF_SIZE; i++) {
            //seperate strings
            if (line[i] == '=') {
                int index = i;

                for (int j = 0; j < index; j++)
                    itemid[j] = line[j];
                //+1 to get rid of the "="
                for (int j = index; j < BUF_SIZE; j++)
                    itemname[j-index] = line[j+1];
                    
                Item item;
                item.id = atoi(itemid);
                item.item = itemname;
                itemslist->push_back(item);
            }
    	}
    }
    
    fclose(file);
}

void LoadModifierList(const char *path, std::vector<Modifier> *itemslist) {
    FILE *file = fopen(path, "r");
    ASSERTFUNC(file, "failed to open file");
    int BUF_SIZE = 64;

    char line[BUF_SIZE] = "";
    while (fgets(line, BUF_SIZE, file) != NULL) {	
        char id[16] = "";
        char name[64] = "";
    	for (int i = 0; i < BUF_SIZE; i++) {
            //seperate strings
            if (line[i] == '=') {
                int index = i;

                for (int j = 0; j < index; j++)
                    id[j] = line[j];
                //+1 to get rid of the "="
                for (int j = index; j < BUF_SIZE; j++)
                    name[j-index] = line[j+1];
                            
                char *modifiername = strtok(name, " ");
                char *type = strtok(NULL, "");
                                
                Modifier mod;
                mod.id = atoi(id);
                modifiername[strcspn(modifiername, "\n")] = '\0';
                mod.mod = modifiername;
                mod.mod += " ";
                mod.type = type;
                itemslist->push_back(mod);
            }
    	}
    }
    
    fclose(file);
}

//very slow, should rewrite
Item getItem(int itemid, std::vector<Item> &itemslist) {
    for (int i = 0; i < static_cast<int>(itemslist.size()); i++) {
        if (itemid == itemslist[i].id) {
            Item item;
            item.id = itemid;
            item.item = itemslist[i].item;
            return item;
        }
    }
    //item doesnt exist
    Item item = {0, "Unknown"};
    return item;
}

int getItemId(const char *item, std::vector<Item> &itemslist) {
    //convert to lowercase
    for (int i = 0; i < static_cast<int>(itemslist.size()); i++) {
        if (compareStrings(itemslist[i].item, item)) {
            return itemslist[i].id;
        }
    }
    //doesnt exist
    return 0;
}

int getModifierId(const char *item, std::vector<Modifier> &itemslist) {
    //convert to lowercase
    std::string item_l = item;
    std::transform(item_l.begin(), item_l.end(), item_l.begin(), [](unsigned char c){ return std::tolower(c); });

    for (int i = 0; i < static_cast<int>(itemslist.size()); i++) {
        if (compareStrings(itemslist[i].mod, item)) {
            return itemslist[i].id;
        }
    }
    //doesnt exist
    return 0;
}

Modifier getModifier(int id, std::vector<Modifier> &itemslist) {
    for (int i = 0; i < static_cast<int>(itemslist.size()); i++) {
        if (id == itemslist[i].id) {
            Modifier item;
            item.id = id;
            item.mod = itemslist[i].mod;
            item.type = itemslist[i].type;
            return item;
        }
    }
    //item doesnt exist
    Modifier item = {0, "Unknown", "NULL"};
    return item;
}

GFX::SpriteSheet getSprite(int itemid, std::vector<GFX::SpriteSheet> &spr) {
    int amount=0;
    for (int i = 0; i < static_cast<int>(spr.size()); i++) {
        auto curspr = spr[i];
        int sprcount = C2D_SpriteSheetCount(curspr);
        if (itemid-amount <= sprcount)
            return curspr;
        amount += sprcount;
    }
    return spr[0];
}

int getSpriteID(int itemid, std::vector<GFX::SpriteSheet> &spr) {
    int amount=0;
    int id = 0;
    for (int i = 0; i < static_cast<int>(spr.size()); i++) {
        id=itemid;
        if (i == 0)
            id = itemid + NUM_NEGATIVE_IDS;
        auto curspr = spr[i];
        int sprcount = C2D_SpriteSheetCount(curspr);
        if (id-amount <= sprcount && id-amount > 0)
            return id-amount;
        amount += sprcount;
    }
    return 0;
}

int getIndex(int itemid, CharacterData &data) {
    for (int i = 0; i < NUM_INVENTORY_SLOTS; i++) {
        if (itemid == data.items[i].itemid) {
            return i;
        }
    }
    return 0;
}



}
