#include "items.h"
#include "app.h"
#include "savefile.h"
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <iostream>

namespace Terraria {
    
Item getItem(int id, SaveFileParser &parser) {
    auto it = std::find_if(parser.allitems.begin(), parser.allitems.end(), [id](const Item& item) {
        return item.id == id;
    });

    if (id == 0)
        return {0, "Empty slot"};

    if (it != parser.allitems.end())
        return *it; 
    else 
        return {0, "Unknown"};
}

}