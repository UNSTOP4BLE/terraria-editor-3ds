#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "app.h"
#include "savefile.h"
#include "gfx.h"

namespace Terraria {

class ItemsGrid {
public:
    void init(int c, int r, GFX::XY<int> p, int w, int off) {numrows = r; numcols = c; width = w; offset = off; pos = p;}
    bool itemExists(int selection) {
        int col = selection%numcols;
        int row = selection/numcols;
        return (row >= 0 && row < numrows && col < numcols && col >= 0);
    }

    GFX::Rect<int> getItem(int selection) {
        GFX::Rect<int> r;
        r = {pos.x+((selection%numcols)*(offset+width)), pos.y+((selection/numcols)*(offset+width)), width, width};
        ASSERTFUNC(itemExists(selection), "itemsgrid: out of bounds");
        return r;
    }
    
    int numrows, numcols, width, offset;
    GFX::XY<int> pos;
};

Item getItem(int id, SaveFileParser &parser);
}
