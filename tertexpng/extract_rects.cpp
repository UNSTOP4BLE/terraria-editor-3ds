#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cassert>

struct Rect {
    int16_t x;
    int16_t padding1;
    int16_t y;
    int16_t padding2;
    int16_t w;
    int16_t padding3;
    int16_t h;
    int16_t padding4;
};

struct Sprite {
    char texname[64];
    char name[64];
    Rect r;
};

int main(void) {
    FILE *fin = fopen("Terraria.pck", "rb");
    assert(fin);

    FILE *fout = fopen("sprites.txt", "w");
    assert(fout);

    char curtex[64] = {0};
    std::vector<Sprite> sprites;

    while (true) {
        Sprite spr;
        memset(&spr, 0, sizeof(Sprite));

        int c = fgetc(fin);
        if (c == EOF) break;

        if (c == 0 && !sprites.empty()) {
            int texlen = fgetc(fin);
            if (texlen == EOF) break;
            if (texlen > 63) texlen = 63;  // safety check

            memset(curtex, 0, sizeof(curtex));
            for (int j = 0; j < texlen; j++) {
                int ch = fgetc(fin);
                if (ch == EOF) break;
                curtex[j] = static_cast<char>(ch);
            }
            curtex[texlen] = '\0';

            c = fgetc(fin);  // name length
            if (c == EOF) break;
        } else if (sprites.empty()) {
            int texlen = c;
            if (texlen > 63) texlen = 63;

            memset(curtex, 0, sizeof(curtex));
            for (int j = 0; j < texlen; j++) {
                int ch = fgetc(fin);
                if (ch == EOF) break;
                curtex[j] = static_cast<char>(ch);
            }
            curtex[texlen] = '\0';

            c = fgetc(fin);  // name length
            if (c == EOF) break;
        }

        int namelen = c;
        if (namelen > 63) namelen = 63;

        memset(spr.name, 0, sizeof(spr.name));
        for (int j = 0; j < namelen; j++) {
            int ch = fgetc(fin);
            if (ch == EOF) break;
            spr.name[j] = static_cast<char>(ch);
        }
        spr.name[namelen] = '\0';

        strcpy(spr.texname, curtex);

        Rect r;
        size_t read = fread(&r, sizeof(Rect), 1, fin);
        if (read != 1) break;

        spr.r = r;
        sprites.push_back(spr);
    }

    for (const auto& spr : sprites) {
        fprintf(fout, "%s,%s,%d,%d,%d,%d\n",
            spr.texname,
            spr.name,
            spr.r.x, spr.r.y, spr.r.w, spr.r.h);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
