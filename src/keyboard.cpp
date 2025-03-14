#include "keyboard.h"

void KeyBoard::open(const char *info) {
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetInitialText(&swkbd, str);
    swkbdSetHintText(&swkbd, info);    
	swkbdInputText(&swkbd, str, sizeof(str));
}

void KeyBoard::openNum(const char *info) {
    swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 2, -1);
    swkbdSetInitialText(&swkbd, str);
    swkbdSetHintText(&swkbd, info);    
	swkbdInputText(&swkbd, str, sizeof(str));
}

std::string KeyBoard::getValue(void) {
    return str;
}