#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    if (argc != 2){
        printf("Syntaxe : ./decompression <filename>\n");
        return -1;
    }

    
    char* new_filename = malloc(strlen(argv[1] + 1) * sizeof(char));
    strcpy(new_filename,argv[1]);
    
    char* tmp = strrchr(argv[1],'.');
    *tmp = '\0';

    decodage(argv[1],new_filename);

    return 0;
}

