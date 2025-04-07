#pragma once
#include "../scene.h"
#include <string>

class SavingScene : public Scene {
public:
    SavingScene(std::string _msg);
    void update(void);
    void draw(void);
    ~SavingScene(void); 
private:
    int starttime;
    std::string msg;
};