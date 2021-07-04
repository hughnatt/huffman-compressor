#include <stdint.h>
#include <stdio.h>

void rle_codage(char* file_in, char* file_out) {
    FILE* f_in = fopen(file_in, "rb");
    FILE* f_out = fopen(file_out, "wb");

    uint8_t last1, last2;
    int byte, last2valide, cpt;

    // Lecture du premier octet
    byte = fgetc(f_in);
    if (byte == EOF) {
        return;
    }
    fputc(byte, f_out);
    last2 = byte;
    last2valide = 1;

    // Lecture du deuxième octet
    byte = fgetc(f_in);
    if (byte == EOF) {
        return;
    }
    fputc(byte, f_out);
    last1 = byte;


    while (byte != EOF) {
        if (last1 == last2 && last2valide) {  // 2 caractères identiques à la suite
            
            cpt = 0;
            while ((byte != EOF) && (byte == last1) && (cpt < 256)) {
                byte = fgetc(f_in);
                cpt++;
            }
            fputc(cpt - 1, f_out);

            printf("%x\n",byte);
            
            if ((byte != EOF)) {
                fputc(byte, f_out);
            }
            last2valide = 0;
            last1 = byte;

        } else {
            byte = fgetc(f_in);

            if (byte == EOF){
                break;
            }

            fputc(byte, f_out);
            last2 = last1;
            last2valide = 1;            
            last1 = byte;
        }
    }
    fclose(f_in);
    fclose(f_out);
}

void rle_decodage(char* file_in, char* file_out) {

    FILE* f_in = fopen(file_in, "rb");
    FILE* f_out = fopen(file_out, "wb");

    if((f_in == NULL) | (f_out == NULL)){
        printf("Erreur de fichier\n");
    }


    uint8_t last1, last2;
    int byte, cpt, last2valide;

    // Lecture du premier octet
    byte = fgetc(f_in);
    printf("Decodage\n");

    if (byte == EOF) {
        return;
    }
    fputc(byte, f_out);
    last2 = byte;
    last2valide = 0;

    // Lecture du deuxième octet
    byte = fgetc(f_in);
    if (byte == EOF) {
        return;
    }
    fputc(byte, f_out);
    last1 = byte;


    while (byte != EOF) {
        if ((last1 == last2) && (last2valide<=0))  {  // 2 caractères identiques à la suite
            printf("Répétition de deux car\n");
            
            cpt = fgetc(f_in);
            if (cpt == EOF){
                break;
            }
            printf("On boucle %d fois\n",cpt);
            
            while (cpt > 0) {
                fputc(last1, f_out);
                cpt--;
            }

            last2valide = 2;
        } else {

            byte = fgetc(f_in);
            printf("Lu : %x\n",byte);
            if (byte == EOF){
                break;
            }
            fputc(byte, f_out);
            last2 = last1;
            last1 = byte;
            last2valide--;                        
        }
    }

    fclose(f_in);
    fclose(f_out);
}