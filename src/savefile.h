#pragma once 
#include <cstddef>
#include <vector>
#include <cstdint>
#include <string>

namespace Terraria {

struct [[gnu::packed]] InternalItem {
    int16_t itemid;
    int16_t count;
    uint8_t modifier;
};
    
struct CharacterData {
    size_t headersize;
    uint8_t filenamelength;
    char charname[13]; 
    InternalItem items[50+4+4];
};
    
class SaveFileParser {
public:
    void readFile(const char *path);
    void writeFile(const char *path);
    
    CharacterData chardata; //in
    CharacterData outdata; //out
   
private:
    size_t filesize; //bytes
    std::string inputpath;
};
    

}