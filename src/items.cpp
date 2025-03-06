#include "items.h"
#include <cstdlib>
#include <cstdio>
#include <citro2d.h>
namespace Terraria {
    
void LoadItemsList(const char *path) {
    int BUF_SIZE = 64;

    FILE *file = fopen(path, "r");

    char line[BUF_SIZE] = "";
    
    while (fgets(line, BUF_SIZE, file) != NULL) {	
        char itemid[16] = "";
        char item[64] = "";
    	for (int i = 0; i < BUF_SIZE; i++) {
            //seperate strings
            if (line[i] == '=') {
                int index = i;

                for (int j = 0; j < index; j++)
                    itemid[j] = line[j];
                //+1 to get rid of the "="
                for (int j = index; j < BUF_SIZE; j++)
                    item[j-index] = line[j+1];
                    
                //add them to a list todo
            }
    	}
    }
    
    fclose(file);
}



}
