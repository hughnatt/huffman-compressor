#include "io.h"
#include "precodage.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    int mtf = 0, rle = 0;

    if (argc != 2 && argc != 3)
    {
        printf("Syntaxe : ./decompression <filename> <precode>\n");
        printf(">>> precode : 1 pour mtf   \n");
        printf("              2 pour rle   \n");
        printf("              0 pas de precodage (default)           \n");
        return -1;
    }

    if (argc == 3)
    {
        switch (atoi(argv[2]))
        {
        case 1:
            mtf = 1;
            break;
        case 2:
            rle = 1;
            break;
        default:
            break;
        }
    }

    
    char* new_filename = malloc(strlen(argv[1] + 1) * sizeof(char));
    strcpy(new_filename,argv[1]);
    
    char* tmp = strrchr(new_filename,'.');
    *tmp = '\0';



    if (mtf)
    {
        decodage(argv[1], "TEMP.MTF");
        mtf_decodage("TEMP.MTF",new_filename);
    }
    else if (rle)
    {

        decodage(argv[1], "TEMP.RLE");
        rle_decodage("TEMP.RLE",new_filename);
    }
    else {
        decodage(argv[1], new_filename);        
    }

    return 0;
}

