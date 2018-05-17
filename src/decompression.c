#include "io.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[]){

    if (argc != 2){
        printf("Syntaxe : ./decompression <filename>\n");
        return -1;
    }

    decodage(argv[1],"fdecode.txt");

    return 0;
}

