#include "restorescene.h"
#include "../app.h"
#include "fileselection.h"
#include "../pad.h"
#include "../savefile.h"
#include "../backup.h"
#include "saving.h"

RestoreScene::RestoreScene(void) {
    setSceneCol(C2D_Color32(0, 0, 0, 255));
    selection = 0;
    scroll = -60;
    backups.Open(utf8_to_utf16(BACKUP_PATH));
    subfolders.resize(static_cast<int>(backups.GetEntryCount()));
    //subfolder
    for (int i = 0; i < static_cast<int>(backups.GetEntryCount()); i++) {
        subfolders[i].Open(utf8_to_utf16(BACKUP_PATH + utf16_to_utf8(backups[i])));
        //files in subfolder
        for (int j = 0; j < static_cast<int>(subfolders[i].GetEntryCount()); j++) {
            std::string name = utf16_to_utf8(subfolders[i][j]);
            backupfiles.push_back(name);
            backupfullpaths.push_back(BACKUP_PATH + utf16_to_utf8(backups[i]) + "/" +  name);
        }
    } 
}

void RestoreScene::update(void) {
    if (Pad::Pressed(Pad::KEY_B))
        setScene(new SelectionScene());
    if (backupfiles.size() != 0) {
        if (Pad::Pressed(Pad::KEY_DUP) && selection > 0) {
            selection -= 1;
            scroll -= 30;
        }
        else if (Pad::Pressed(Pad::KEY_DDOWN) && selection < static_cast<int>(backupfiles.size())-1) {
            selection += 1;
            scroll += 30;
        }
        else if (Pad::Pressed(Pad::KEY_X)) { //todo add are you sure screen
            if (confirmScreen()) {
                deleteBackup(backupfullpaths[selection].c_str());
                setScene(new RestoreScene());
            }
        }
        else if (Pad::Pressed(Pad::KEY_A)) {
            if (confirmScreen()) {
                restoreBackup(backupfullpaths[selection].c_str());
                setScene(new SavingScene("Restored:\n" + backupfullpaths[selection]));
            }
        }
    }
}

void RestoreScene::draw(void) { 
    if (backupfiles.size() != 0) { //folder
        int offs = 5;
        int y = 0;
    
        app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Left, offs, offs+y-scroll, "%s:", BACKUP_PATH);
        //subfolder
        for (int i = 0; i < static_cast<int>(backups.GetEntryCount()); i++) {
            y += 30;
            app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Left, offs, offs+y-scroll, "    %s:", utf16_to_utf8(backups[i]).c_str());
  
            //files in subfolder
            for (int j = 0; j < static_cast<int>(subfolders[i].GetEntryCount()); j++) {
                y += 30;
                std::string name = utf16_to_utf8(subfolders[i][j]);
                const char *sel = "";
                if (!strcmp(name.c_str(), backupfiles[selection].c_str())) {
                    sel = "> "; 
                }
                app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Left, offs, offs+y-scroll, "        %s%s", sel, name.c_str());
            }
        } 
        app->fontManager.print(app->screens->bottom, GFX::SCR_BTM_W, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2-20, "HOME - Exit\nX - Delete backup\nA - Restore backup\nB - Go back");
    }
    else {
        auto dir = utf8_to_utf16(BACKUP_PATH);
        if (FsLib::DirectoryExists(dir))
            FsLib::DeleteDirectoryRecursively(dir);
        app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, "You have no backups!");
        app->fontManager.print(app->screens->bottom, GFX::SCR_BTM_W, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2, "Press B to go back");
    }
}

RestoreScene::~RestoreScene(void) {
    
}