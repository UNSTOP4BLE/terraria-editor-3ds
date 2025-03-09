#include "savefile.h"
#include "app.h"
#include <cstdint>
#include <cstdio>

namespace Terraria {

void SaveFileParser::readFile(const char *path) {
    FILE *f = fopen(path,"rb");  
    inputpath = path;

    ASSERTFUNC(f, "failed to open file");

    //get file size
    fseek(f, 0, SEEK_END);
    filesize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // HEADER
    fseek(f, 0x2, SEEK_CUR);
    chardata.filenamelength = fgetc(f);
    fseek(f, 0x3, SEEK_CUR);
    uint8_t filename_utf16[chardata.filenamelength];
    fread(&filename_utf16, chardata.filenamelength*2, 1, f);
//    chardata.charname = converttoutf8(filename_utf16);
    fseek(f, 0x46, SEEK_CUR);

    chardata.headersize = ftell(f);
    // ITEMS
    fread(&chardata.items, sizeof(chardata.items), 1, f);

    // COINS
    fread(&chardata.coins, sizeof(chardata.coins), 1, f);
    // AMMO
    fread(&chardata.ammo, sizeof(chardata.ammo), 1, f);
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
    fseek(fout, chardata.headersize, SEEK_CUR);

    //inventory

    // ITEMS
    fwrite(&chardata.items, sizeof(chardata.items), 1, fout);
    // COINS
    fwrite(&chardata.coins, sizeof(chardata.coins), 1, fout);
    // AMMO
    fwrite(&chardata.ammo, sizeof(chardata.ammo), 1, fout);

    fclose(fout);
}

}