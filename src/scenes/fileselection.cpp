#include "fileselection.h"
#include "../savefile.h"
#include "../pad.h"
#include "inventory.h"

constexpr const char *path_prefix = "extdata:/v2/";

SelectionScene::SelectionScene(void) {
    setSceneCol(C2D_Color32(0, 0, 0, 255));
    selection = 0;
    FsLib::Directory savedir(utf8_to_utf16(path_prefix));

    ASSERTFUNC(savedir.IsOpen(), "FAILED TO OPEN SAVE DIRECTORY");
    
    for (int i = 0; i < static_cast<int>(savedir.GetEntryCount()); i++) {
        std::string str = utf16_to_utf8(savedir[i]);

        if (str.substr(str.find_last_of(".")) == ".p")     
            filelist.push_back(str);
    }
}

void SelectionScene::update(void) {
    if (filelist.size() != 0) {
        if (Pad::Pressed(Pad::KEY_DUP) && selection > 0)
            selection -= 1;
        else if (Pad::Pressed(Pad::KEY_DDOWN) && selection < static_cast<int>(filelist.size())-1)
            selection += 1;

        std::string path = path_prefix;
        path += filelist[selection].c_str();
        if (Pad::Pressed(Pad::KEY_A))
            setScene(new InventoryScene(utf8_to_utf16(path)));
    }
}

void SelectionScene::draw(void) {
    int offs = 5;
    int y = 0;
    if (filelist.size() != 0) {
        for (int i = 0; i < static_cast<int>(filelist.size()); i++) {
            const char *sel = "";
            if (selection == i)
                sel = "> ";
            app->fontManager.print(app->screens->top, GFX::Left, offs, offs+y, "%s%s", sel, filelist[i].c_str());
            y += 30;
        }
        app->fontManager.print(app->screens->bottom, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2, "HOME - Exit\nA - Edit file\nY - Restore backup\nX - Backup");
    }
    else {
        app->fontManager.print(app->screens->top, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, "You have no savefiles!");
        app->fontManager.print(app->screens->bottom, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2, "Press HOME to exit");
    }
}

SelectionScene::~SelectionScene(void) {
    
}