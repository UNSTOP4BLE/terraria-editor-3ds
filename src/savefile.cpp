#include "savefile.h"
#include "app.h"
#include "fslib/FsLib.hpp"
#include <cstdint>
#include <cstdio>
#include <string>

std::string utf16_to_utf8(const char16_t* utf16_str) {
    std::string utf8_str;
    
    for (size_t i = 0; utf16_str[i] != 0; ++i) { // Loop until null terminator
        uint16_t code_unit = utf16_str[i];

        if (code_unit < 0x80) { 
            // 1-byte UTF-8 (ASCII)
            utf8_str += static_cast<char>(code_unit);
        } 
        else if (code_unit < 0x800) { 
            // 2-byte UTF-8
            utf8_str += static_cast<char>(0xC0 | (code_unit >> 6));
            utf8_str += static_cast<char>(0x80 | (code_unit & 0x3F));
        } 
        else if (code_unit >= 0xD800 && code_unit <= 0xDBFF) { 
            // Surrogate pair (High surrogate detected)
            uint16_t high = code_unit;
            uint16_t low = utf16_str[i + 1];

            if (low >= 0xDC00 && low <= 0xDFFF) { 
                // Convert surrogate pair to Unicode code point
                uint32_t code_point = ((high - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
                
                // 4-byte UTF-8
                utf8_str += static_cast<char>(0xF0 | (code_point >> 18));
                utf8_str += static_cast<char>(0x80 | ((code_point >> 12) & 0x3F));
                utf8_str += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                utf8_str += static_cast<char>(0x80 | (code_point & 0x3F));

                ++i; // Skip low surrogate
            }
        } 
        else { 
            // 3-byte UTF-8 (BMP characters)
            utf8_str += static_cast<char>(0xE0 | (code_unit >> 12));
            utf8_str += static_cast<char>(0x80 | ((code_unit >> 6) & 0x3F));
            utf8_str += static_cast<char>(0x80 | (code_unit & 0x3F));
        }
    }
    return utf8_str;
}

std::u16string utf8_to_utf16(const std::string& utf8_str) {
    std::u16string utf16_str;
    size_t i = 0;
    
    while (i < utf8_str.size()) {
        uint8_t byte1 = utf8_str[i];
        
        if ((byte1 & 0x80) == 0) {
            // 1 byte (ASCII)
            utf16_str.push_back(byte1);
            i++;
        } else if ((byte1 & 0xE0) == 0xC0) {
            // 2 bytes
            uint8_t byte2 = utf8_str[i + 1];
            uint16_t code_point = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
            utf16_str.push_back(code_point);
            i += 2;
        } else if ((byte1 & 0xF0) == 0xE0) {
            // 3 bytes
            uint8_t byte2 = utf8_str[i + 1];
            uint8_t byte3 = utf8_str[i + 2];
            uint16_t code_point = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
            utf16_str.push_back(code_point);
            i += 3;
        } else if ((byte1 & 0xF8) == 0xF0) {
            // 4 bytes
            uint8_t byte2 = utf8_str[i + 1];
            uint8_t byte3 = utf8_str[i + 2];
            uint8_t byte4 = utf8_str[i + 3];
            uint32_t code_point = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
            
            // Surrogate pair for code points above 0xFFFF
            code_point -= 0x10000;
            uint16_t high_surrogate = 0xD800 + (code_point >> 10);
            uint16_t low_surrogate = 0xDC00 + (code_point & 0x3FF);
            
            utf16_str.push_back(high_surrogate);
            utf16_str.push_back(low_surrogate);
            i += 4;
        } else {
            break;
        }
    }
    
    return utf16_str;
}

namespace Terraria {

void SaveFileParser::readFile(const char16_t*path) {
    FsLib::File f(path, FS_OPEN_READ);
    inputpath = utf16_to_utf8(path).c_str();

    ASSERTFUNC(f.IsOpen(), "failed to open file");

    //get file size
    filesize = f.GetSize();

    // HEADER
    f.Seek(0x2, f.Tell());
    chardata.filenamelength = f.GetCharacter();
    f.Seek(0x3, f.Tell());
    char16_t filename_utf16[13];
    f.Read(&filename_utf16, chardata.filenamelength);
    //dont work
    chardata.charname = utf16_to_utf8(filename_utf16);
    if (chardata.charname == "sviki") {
        while (1) {}
    }
    f.Seek(0x46, f.Tell());

    chardata.headersize = f.Tell();
    // ITEMS
    f.Read(&chardata.items, sizeof(chardata.items));

    outdata = chardata;
}

void SaveFileParser::writeFile(const char *path) {
    FILE *fout = fopen(path,"wb");  
    FILE *fin = fopen(inputpath.c_str(),"rb"); 

    ASSERTFUNC(fin, "failed to open file");
    ASSERTFUNC(fout, "failed to open file");

    
    uint8_t indata[filesize];

    //copy over the original file
    fread(&indata, filesize, 1, fin); 
    fwrite(indata, filesize, 1, fout);
    fclose(fin);
    fseek(fout, 0, SEEK_SET);

    // HEADER
    fseek(fout, outdata.headersize, SEEK_CUR);

    //inventory

    // ITEMS
    fwrite(&outdata.items, sizeof(outdata.items), 1, fout);

    fclose(fout);
}

}