#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "codage.h"

uint8_t buffer = 0;
uint8_t restant = 8;

uint64_t nbsym = 0;

FILE *f_in;
FILE *f_out;

void writebyte()
{
    fputc(buffer, f_out);
    buffer = 0;
    restant = 8;
}

void writecode(uint8_t code, int taille)
{
    nbsym++;
    if (taille <= restant)
    {
        buffer = buffer << taille;
        buffer = buffer | (code & ~((-1) << taille));
        restant = restant - taille;
        if (restant == 0)
        {
            writebyte();
        }
    }
    else
    {
        //buffer = buffer << restant;
        printf("%d", taille);
        //****
        code = code << (8 - taille);
        while (restant != 0)
        {
            buffer = buffer << 1;
            buffer = buffer | ((code & 0x80) >> 7); //Bit de poids fort ramené à droite
            code = code << 1;
            taille--;
            restant--;
        }

        writebyte();

        while (taille != 0)
        {
            buffer = buffer << 1;
            buffer = buffer | ((code & 0x80) >> 7); //Bit de poids fort ramené à droite
            code = code << 1;
            restant--;
            taille--;
        }
        //****
    }
}

void padcode()
{
    if (restant != 8)
    {
        buffer = buffer << restant;
        writebyte();
    }
}

void rle_prof(uint8_t profondeur[256], uint8_t profondeur_rle[512], uint8_t* taille_rle){

    char prev2 = profondeur[0];
    char prev1 = profondeur[1];

    int j=0;
    int taille;

    profondeur_rle[j++] = prev2;
    profondeur_rle[j++] = prev1;

    int k;
    for(k=0; k<256; k++){
        printf("%d %d\n", k, profondeur[k]);
    }
    
    for(int i=2; i<256; i++){

        if(prev1 == prev2){

            taille = 0;

            while(profondeur[i] == prev1){

            prev2 = prev1;
            prev1 = profondeur[i];

            taille++;
            i++;

           }
           profondeur_rle[j++] = taille;       
        }
        
        profondeur_rle[j++] = profondeur[i];
        prev2 = prev1;
        prev1 = profondeur[i];

    }
    *taille_rle = j-1;
}

void transcodage(char *file_in, char *file_out, uint8_t code[256], uint8_t profondeur[256])
{
    f_in = fopen(file_in, "rb");
    f_out = fopen(file_out, "wb");
    /* TODO Vérifier ouverture*/

    fputs("--------", f_out); //Place laissé pour le nombre d'octets

    /*Mettre tableau de profondeur*/
    uint8_t taille_rle;
    uint8_t profondeur_rle[512];

    
    rle_prof(profondeur, profondeur_rle, &taille_rle);
    fputc(taille_rle,f_out);

    int i = 0;
    char a;
    while (i < taille_rle){
        a = profondeur_rle[i];
        fputc(a,f_out);
        i++;
    }

    //uint8_t c;

    uint8_t cc;
    int taillec;

    //int getcar;
    int c = fgetc(f_in);
    while (c != EOF)
    {
        //c = (uint8_t)getcar;
        cc = code[c];
        taillec = profondeur[c];

        writecode(cc, taillec);

        //getcar = fgetc(f_in);
        c = fgetc(f_in);
    }

    padcode();

    fseek(f_out, 0, SEEK_SET); // On revient au début du fichier

    // On écrit la taille des données
    i = 0;
    while (i < 8)
    {
        fputc(nbsym, f_out);
        nbsym = nbsym >> 8;
        i++;
    }

    fclose(f_out);
    fclose(f_in);
}


uint8_t bit_suivant()
{
    static uint8_t c;

    //Lit un octet
    if (restant == 0)
    {
        c = getc(f_in);
    }
    else
    {
        restant--;
        c = c << 1;
    }
    return c & 0x80;
}

uint8_t lire_sym(phtree_t arbre_canonique)
{
    phtree_t node = arbre_canonique;
    while (node->taille_label != 1){
        if (bit_suivant){
            node = node->fdroit;
        } else {
            node = node->fgauche;
        }
    }

    return node->label[0];
}


void lire_profondeur(uint8_t profondeur[256]){
    for (int i = 0; i < 256; i++)
    {
        profondeur[i] = fgetc(f_in);
    }
}

uint64_t lire_nbsym()
{
    uint64_t nb;
    uint64_t tmp;
    tmp = 0;
    tmp = fgetc(f_out);

    int i = 0;
    while (i < 8)
    {

        tmp = tmp << (56 - i * 8);
        nb = nb | tmp;

        i++;

        tmp = 0;
        tmp = fgetc(f_out);
    }
}

void decodage(char *file_in, char *file_out)
{
    f_in = fopen(file_in, "rb");
    f_out = fopen(file_out, "wb");

    //Lire nombre de symbole à lire (8 octets)
    uint64_t nbsym = lire_nbsym();

    //Lire la table de profondeur (256 octets)
    uint8_t profondeur[256];
    lire_profondeur(profondeur);
    //Créer arbre canonique
    //phtree_t abr_can = arbre_canonique(profondeur);

    //Lecture des symboles
    restant = 0;
    int cpt = 0;
    uint8_t c;
    while (cpt < nbsym)
    {
        //c = lire_sym(abr_can);
        fputc(c, f_out);
        cpt++;
    }
}




