#include "items.h"
#include <cstdlib>
#include <cstdio>
#include <citro2d.h>

namespace Terraria {
    
void LoadItemsList(const char *path, std::vector<Item> *itemslist) {
    FILE *file = fopen(path, "r");

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
                    
                //add them to a list todo
                Item item;
                item.id = atoi(itemid);
                item.item = itemname;
                itemslist->push_back(item);
            }
    	}
    }
    
    fclose(file);
}
//very slow, should rewrite
Item getItem(int itemid, std::vector<Item> &itemslist) {
    for (int i = 0; i < itemslist.size(); i++) {
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
//very slow, should rewrite
int getIndex(int itemid, CharacterData &data) {
    for (int i = 0; i < NUM_INVENTORY_SLOTS; i++) {
        if (itemid == data.items[i].itemid) {
            return i;
        }
    }
    return 0;
}



}
