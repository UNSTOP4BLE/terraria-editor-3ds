#pragma once

#include <cstdint>

class Scene {
public:
    inline Scene(void) {}
    virtual void update(void) {}
    virtual void draw(void) {}
    virtual ~Scene(void) {}
};

class SceneManager {
public:
    Scene *cur;
    Scene *next;
};
    
void setScene(Scene *scn);
void setSceneCol(uint32_t color);
void checkSwapScene(void);