#pragma once
#include <string>
#include <3ds.h>

class KeyBoard {
public:
    void open(const char *info);
    void openNum(const char *info);
    std::string getValue(void);
private:
    SwkbdState swkbd;
    char str[256];
    SwkbdStatusData swkbdStatus;
    SwkbdLearningData swkbdLearning;

};