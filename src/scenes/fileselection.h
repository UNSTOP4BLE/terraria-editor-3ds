#pragma once
#include "../app.h"
#include "../fslib/FsLib.hpp"
#include <vector>

class SelectionScene : public Scene {
public:
    SelectionScene(void);
    void update(void);
    void draw(void);
    ~SelectionScene(void); 
private:
    std::vector<std::string> filelist;
    int selection;
};