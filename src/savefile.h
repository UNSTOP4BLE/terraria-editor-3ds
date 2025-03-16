#pragma once 
#include <cstddef>
#include <vector>
#include <cstdint>
#include <string>

#define NUM_INVENTORY_SLOTS 50
#define NUM_COIN_SLOTS 4
#define NUM_AMMO_SLOTS 4
#define NUM_TOTAL_SLOTS (NUM_INVENTORY_SLOTS+NUM_COIN_SLOTS+NUM_AMMO_SLOTS)

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
    InternalItem items[NUM_TOTAL_SLOTS];
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