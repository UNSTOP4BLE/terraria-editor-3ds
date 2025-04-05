#include "items.h"
#include "app.h"
#include "savefile.h"
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <iostream>

namespace Terraria {
    
Item getItem(int id, SaveFileParser &parser) {
    // Search for the item with the specified id
    auto it = std::find_if(parser.allitems.begin(), parser.allitems.end(), [id](const Item& item) {
        return item.id == id;
    });

    // If the item is found, return it, otherwise return a default item or handle error
    if (it != parser.allitems.end())
        return *it;  // Return the found item
    else 
        return Item();  // Return a default (empty) item as a fallback todo
}

}