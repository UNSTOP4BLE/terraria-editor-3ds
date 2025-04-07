#include "items.h"
#include "app.h"
#include "savefile.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <iostream>

void normalize(const char* input, char* output, size_t maxLen) {
    size_t j = 0;
    bool in_space = false;

    // Skip leading spaces
    while (*input && std::isspace(static_cast<unsigned char>(*input))) {
        ++input;
    }

    while (*input && j < maxLen - 1) {
        if (std::isspace(static_cast<unsigned char>(*input))) {
            if (!in_space) {
                output[j++] = ' ';
                in_space = true;
            }
        } else {
            output[j++] = std::tolower(static_cast<unsigned char>(*input));
            in_space = false;
        }
        ++input;
    }

    // Remove trailing space if present
    if (j > 0 && output[j - 1] == ' ') {
        --j;
    }

    output[j] = '\0';
}

namespace Terraria {
    
Item getItem(int id, SaveFileParser &parser) {
    auto it = std::find_if(parser.allitems.begin(), parser.allitems.end(), [id](const Item& item) {
        return item.id == id;
    });

    if (id == 0)
        return {0, "Empty slot"};

    if (it != parser.allitems.end())
        return *it; 
    else 
        return {0, "Unknown"};
}

Item getItemWithName(const char* name, SaveFileParser& parser) {
    if (std::strlen(name) == 0)
        return {0, "Empty slot"};

    char normalizedInput[128];
    normalize(name, normalizedInput, sizeof(normalizedInput));

    for (const auto& item : parser.allitems) {
        char normalizedItemName[128];
        normalize(item.name, normalizedItemName, sizeof(normalizedItemName));

        if (std::strcmp(normalizedInput, normalizedItemName) == 0) {
            if (item.id == 0)
                return {0, "Empty slot"};
            return item;
        }
    }

    return {0, "Unknown"};
}

Modifier getModifierWithName(const char* name, SaveFileParser& parser) {
    if (std::strlen(name) == 0)
        return parser.allmodifiers[0];

    char normalizedInput[128];
    normalize(name, normalizedInput, sizeof(normalizedInput));
    for (const auto& modifier : parser.allmodifiers) {
        char normalizedModifierName[128];
        normalize(modifier.name, normalizedModifierName, sizeof(normalizedModifierName));

        if (std::strcmp(normalizedInput, normalizedModifierName) == 0) {
            if (modifier.id == 0)
                return parser.allmodifiers[0];
            return modifier;
        }
    }

    return {0, "Unknown", ""};
}


int getTotalItemCount(int id, CharacterData &data) {
    int count = 0;
    for (int i = 0; i < NUM_TOTAL_SLOTS; i++) {
        auto item = data.items[i];
        if (item.id == id)
            count += item.count;
    }

    if (id == 0)
        count = 0;
    return count;
}

}