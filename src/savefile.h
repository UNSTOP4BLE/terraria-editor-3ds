#pragma once 
#include <cstddef>
#include <vector>
#include <cstdint>
#include <string>
#include "items.h"

namespace Terraria {

struct CharacterData {
    size_t headersize;
    uint8_t filenamelength;
    char charname[13]; 
    InternalItem items[50];
    InternalItem coins[4];
    InternalItem ammo[4];
};
    

class SaveFileParser {
public:
    void readFile(const char *path);
    void writeFile(const char *path);
    
    CharacterData chardata;
   
private:
    size_t filesize; //bytes
    std::string inputpath;
};
    

}