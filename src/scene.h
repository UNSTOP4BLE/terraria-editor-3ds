#pragma once

class Scene {
public:
    inline Scene(void) {}
    virtual void update(void) {}
    virtual void draw(void) {}
    virtual ~Scene(void) {}
};

void setScene(Scene *scr);
void setSceneCol(uint32_t color);