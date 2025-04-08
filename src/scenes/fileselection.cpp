#include "fileselection.h"
#include "../savefile.h"
#include "../backup.h"
#include "../pad.h"
#include "inventory.h"
#include "saving.h"
#include "restorescene.h"

SelectionScene::SelectionScene(void) {
    setSceneCol(C2D_Color32(0, 0, 0, 255));
    selection = 0;
    FsLib::Directory savedir;
    if (FsLib::DirectoryExists(utf8_to_utf16(EXTDATA_PATH))) {
        savedir.Open(utf8_to_utf16(EXTDATA_PATH));
        ASSERTFUNC(savedir.IsOpen(), "FAILED TO OPEN SAVE DIRECTORY");
    }

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

        std::string path = EXTDATA_PATH;
        path += filelist[selection].c_str();
        std::string backuppath = AUTOBACKUP_PATH + filelist[selection] + ".bak";
        if (Pad::Pressed(Pad::KEY_Y))
            setScene(new RestoreScene());
        else if (Pad::Pressed(Pad::KEY_A))
            setScene(new InventoryScene(utf8_to_utf16(path)));
        else if (Pad::Pressed(Pad::KEY_X)) {
            if (confirmScreen()) {
                writeBackup(BACKUP_PATH, utf8_to_utf16(path), filelist[selection].c_str());
                setScene(new SavingScene(("Backup success: \n" + std::string(BACKUP_PATH) + filelist[selection].c_str()).c_str()));
            }
        }
        else if (Pad::Pressed(Pad::KEY_B)) 
        {
            if (FsLib::FileExists(utf8_to_utf16(backuppath))) {
                if (confirmScreen()) {
                    restoreBackup(backuppath);
                    setScene(new SavingScene(("Restore backup success: \n" + std::string(BACKUP_PATH) + filelist[selection].c_str()).c_str()));
                }
            }
            else 
                setScene(new SavingScene(" You have no autobackups,\nthey are made automatically\n  by editing the save file"));
        }
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
        app->fontManager.print(app->screens->bottom, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2-20, "HOME - Exit\nA - Edit file\nY - Restore backup\nX - Backup\nB - Restore autobackup");
    }
    else {
        app->fontManager.print(app->screens->top, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, "You have no savefiles!");
        app->fontManager.print(app->screens->bottom, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2, "Press HOME to exit");
    }
}

SelectionScene::~SelectionScene(void) {
    
}