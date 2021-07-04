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

void encode_rle_prof(uint8_t profondeur[256])
{
    for (int i = 0; i< 256; i++){
        fputc(profondeur[i],f_out);
    }
}

void decode_rle_prof(uint8_t profondeur[256])
{
    for (int i = 0; i< 256; i++){
        profondeur[i] = fgetc(f_in);
    }
}

void writebyte()
{
    fputc(buffer, f_out);
    buffer = 0;
    restant = 8;
}

void writecode(uint64_t code, int taille)
{
#ifdef DEBUG
    printf("Taille : %d", taille);
    printf("Encode : ");
    for (int i = 8 - taille; i < 8; i++)
    {
        printf("%d", !!((code << i) & 0x80));
    }
#endif

    nbsym++;

    //On décale le bit de poids fort vers la gauche
    code = code << (64 - taille);

#ifdef DEBUG
    printf("\tEffective : ");
#endif

    //On lit chaque bit valide du code

    while (taille > 0)
    {
        buffer = buffer << 1;
        buffer = buffer | ((code & 0x8000000000000000) >> 63); //Bit de poids fort ramené à droite
#ifdef DEBUG
        printf("%ld", ((code & 0x8000000000000000) >> 63));
#endif
        code = code << 1;
        taille--;
        restant--;
        if (restant == 0)
        {
            writebyte();
        }
    }
#ifdef DEBUG
    printf("\n");
#endif
}

void padcode()
{
    if (restant != 8)
    {
        buffer = buffer << restant;
        writebyte();
    }
}

void transcodage(char *file_in, char *file_out, uint64_t code[256], uint8_t profondeur[256])
{
    f_in = fopen(file_in, "rb");
    f_out = fopen(file_out, "wb");
    /* TODO Vérifier ouverture*/

    fputs("--------", f_out); //Place laissé pour le nombre d'octets


    /*Mettre tableau de profondeur encodé en rle*/
    encode_rle_prof(profondeur);

    uint64_t cc;
    int taillec, i;


    int c = fgetc(f_in);
    while (c != EOF)
    {

        cc = code[c];
        taillec = profondeur[c] - 1;

#ifdef DEBUG
        printf("Caractere %x (%c) -- ", c, c);
#endif

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
        c = fgetc(f_in);
        restant = 7;
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
    while (node->taille_label != 1)
    {
        if (bit_suivant())
        {
            node = node->fdroit;
        }
        else
        {
            node = node->fgauche;
        }
    }

    return node->label[0];
}

void lire_profondeur(uint8_t profondeur[256])
{
    for (int i = 0; i < 256; i++)
    {
        profondeur[i] = fgetc(f_in);
    }
}

uint64_t lire_nbsym()
{
    uint64_t nb = 0;
    uint64_t tmp;

    int i = 7;
    while (i >= 0)
    {
        tmp = 0;
        tmp = fgetc(f_in);
        tmp = tmp << (56 - i * 8);
        nb = nb | tmp;
        i--;
    }
    return nb;
}

void displaytab256(uint8_t tab[256])
{
    for (int k = 0; k < 256; k++)
    {
        //if ( tab[k] != 0){
        printf("%d | ", k);
        for (int i = 0; i < 8; i++)
        {
            printf("%d", !!((tab[k] << i) & 0x80));
        }
        printf(" \n");
        //}
    }
    printf("\n");
}

void decodage(char *file_in, char *file_out)
{
    f_in = fopen(file_in, "rb");
    f_out = fopen(file_out, "wb");

    if (f_in == NULL || f_out == NULL)
    {
        printf("FATAL ERROR, Erreur d'ouverture de fichier\n%s\n%s", file_in, file_out);
        exit(-1);
    }
    //Lire nombre de symbole à lire (8 octets)
    uint64_t nbsym = lire_nbsym();
    //printf("Symboles à lire : %ld\n", nbsym);

    //Lire la table de profondeur (256 octets)
    uint8_t profondeur[256];
    //lire_profondeur(profondeur);
    decode_rle_prof(profondeur);

    #ifdef DEBUG
    displaytab256(profondeur);
    #endif

    //Créer arbre canonique
    phtree_t abr_can = arbre_canonique(profondeur);

#ifdef DEBUG
    afficher_arbre(abr_can, 1);
#endif

    //Lecture des symboles

    restant = 0;
    int cpt = 0;
    uint8_t c;
    while (cpt < nbsym)
    {
        c = lire_sym(abr_can);
        fputc(c, f_out);
        cpt++;
    }

    fclose(f_in);
    fclose(f_out);
}
