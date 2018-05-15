#include <stdint.h>
#include <stdio.h>
#include <stdint.h>


uint8_t buffer;
uint8_t restant;


void transcodage(char* file_in, char* file_out, uint8_t code[256], uint8_t profondeur[256]){
    FILE* fin = fopen(file_in, "rb");
    FILE* fout = fopen(file_out,"wb");
    /* TODO Vérifier ouverture*/
    fputs("--------",fout);

    /*Mettre tableau de profondeur*/



    uint8_t c;
    
    uint8_t cc;
    int taillec;

    int getcar = 0;
    getcar = fgetc(fin);
    while (getcar != EOF){
        c = (uint8_t) getcar;
        
        cc = code[c];
        taillec = profondeur[c];


        writecode(cc,taillec);



        
        getcar = fgetc(fin);
    }

}

void writecode(uint8_t code, int taille){

    if (taille <= restant){
        buffer = buffer << taille;
        buffer = buffer | (code & ~((-1) << taille));
        restant = restant - taille;
        if (restant == 0){
            writebyte();
        }
    }
    else {
        buffer = buffer << restant;
        

    }
}


void decodage(char* file_in, char* file_out);

