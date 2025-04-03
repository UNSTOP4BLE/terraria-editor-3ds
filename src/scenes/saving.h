#pragma once
#include "../scene.h"

class SavingScene : public Scene {
public:
    SavingScene(void);
    void update(void);
    void draw(void);
    ~SavingScene(void); 
private:
    int starttime;
};