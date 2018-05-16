#include "../include/io.h"


int main(){

    uint8_t profondeur[256] = {0};
    uint8_t code[256] = {0};

    code[65] = 0xff;
    code[66] = 0x08;
    code[67] = 0x02;

    profondeur[65] = 8;
    profondeur[66] = 4; 
    profondeur[67] = 2; 
     
    transcodage("in.txt", "out.txt", code, profondeur);


    return 0;
}