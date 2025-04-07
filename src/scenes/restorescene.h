#pragma once
#include "../scene.h"
#include <string>
#include <vector>
#include "../fslib/FsLib.hpp"

class RestoreScene : public Scene {
public:
    RestoreScene(void);
    void update(void);
    void draw(void);
    ~RestoreScene(void); 
private:
    int selection, scroll;
    FsLib::Directory backups;
    std::vector<FsLib::Directory> subfolders;
    std::vector<std::string> backupfiles;
    std::vector<std::string> backupfullpaths;
};