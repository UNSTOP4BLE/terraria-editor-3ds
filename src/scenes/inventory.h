#pragma once
#include "../app.h"
#include "../savefile.h"
#include "../items.h"
#include "../uibutton.h"
#include <vector>

class InventoryScene : public Scene {
public:
    InventoryScene(std::u16string path);
    void update(void);
    void draw(void);
    ~InventoryScene(void); 
private:
    void printItemInfo(int yoff, int id, Terraria::Item item, Terraria::Modifier mod, Terraria::CharacterData &data);
    void changeItem(int slot, int id, bool replace);
    float scaleItem(GFX::XY<int> wh, float scl, int max);
    Terraria::ItemsGrid getGrid(int selection, int &offset, GFX::SpriteSheet &sheet);


    GFX::SpriteSheet invhotbar, invstandard, invcoins, invammo;
    UiButton trashButton;
    UiButton restoreButton;
    UiButton backButton;
    bool editing;
    int selecteditem;
    int scroll;
    float pushdelaytime;
    int max_scroll;
    GFX::XY<int> scrollbar;
    Terraria::SaveFileParser parser;
    Terraria::ItemsGrid invgrid;
    Terraria::ItemsGrid coinsgrid;
    Terraria::ItemsGrid ammogrid;

    GFX::Tex tex_invpanel;
    GFX::Tex tex_scroll;
    GFX::Tex tex_infopanel;
    GFX::Tex tex_invitems[NUM_TOTAL_SLOTS];
    struct {
        GFX::Tex tex;
        Terraria::Item item;
        Terraria::InternalItem *actualitem;
        Terraria::Modifier mod;
        inline void update(int id, int count, int modid, Terraria::SaveFileParser &parser) {
            item = Terraria::getItem(id, parser);
            actualitem->id = id;
            if (item.id == 0) {
                count = 0;
                modid = 0;
            }
            actualitem->count = count;
            actualitem->mod = modid;
            mod = parser.allmodifiers[modid];
        }
    } curitem, currepitem;
};