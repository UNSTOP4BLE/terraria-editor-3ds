#pragma once 
#include <cstddef>
#include <vector>
#include <cstdint>
#include <string>

#define NUM_INVENTORY_SLOTS 50
#define NUM_COIN_SLOTS 4
#define NUM_AMMO_SLOTS 4
#define NUM_TOTAL_SLOTS (NUM_INVENTORY_SLOTS+NUM_COIN_SLOTS+NUM_AMMO_SLOTS)

std::string utf16_to_utf8(const char16_t* utf16_str);
std::u16string utf8_to_utf16(const std::string& utf8_str);

namespace Terraria {

struct Item {
    int id;
    char name[128];
};

struct Modifier {
    int id;
    char name[128];
    char type[32];
};
        
struct [[gnu::packed]] InternalItem {
    int16_t id;
    int16_t count;
    uint8_t mod;
};
    
struct CharacterData {
    size_t headersize;
    uint8_t filenamelength;
    std::string charname; 
    InternalItem items[NUM_TOTAL_SLOTS];
};
    
class SaveFileParser {
public:
    void init(void);
    void readFile(const char16_t*);
    void writeFile(const char *path);
    
    std::vector<Item> allitems;
    std::vector<Modifier> allmodifiers;
    CharacterData chardata; //in
    CharacterData outdata; //out
    std::string inputpath;
    size_t filesize; //bytes
};
    

}