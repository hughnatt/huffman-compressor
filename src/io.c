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

void transcodage(char *file_in, char *file_out, uint8_t code[256], uint8_t profondeur[256])
{
    f_in = fopen(file_in, "rb");
    f_out = fopen(file_out, "wb");
    /* TODO Vérifier ouverture*/

    fputs("--------", f_out); //Place laissé pour le nombre d'octets

    /*Mettre tableau de profondeur*/
    for (int i = 0; i < 256; i++)
    {
        fputc(profondeur[i], f_out);
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
        taillec = profondeur[c] - 1;

        writecode(cc, taillec);

        //getcar = fgetc(f_in);
        c = fgetc(f_in);
    }

    padcode();

    fseek(f_out, 0, SEEK_SET); // On revient au début du fichier

    // On écrit la taille des données
    int i = 0;
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
    while (node->taille_label != 1){
        if (bit_suivant()){
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

void afficher_arbre(phtree_t a, int niveau)
{
    /*
    affichage de l'arbre a
    on l'affiche en le penchant sur sa gauche
    la partie droite (haute) se retrouve en l'air
    */

    int i;

    if (a != NULL)
    {
        afficher_arbre(a->fdroit, niveau + 1);

        for (i = 0; i < niveau; i++)
            printf("\t");
        for (i = 0; i < a->taille_label ; i++){
            printf("%c",a->label[i]);
        }
        printf(" (%d)\n\n", niveau);

        afficher_arbre(a->fgauche, niveau + 1);
    }
    return;
}

void displaytab256(uint8_t tab[256]){
    for (int k = 0; k < 256 ; k++){
        //if ( tab[k] != 0){
            printf("%d | ",k);
            for (int i = 0; i < 8; i++){
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

    //Lire nombre de symbole à lire (8 octets)
    uint64_t nbsym = lire_nbsym();
    printf("Symboles à lire : %ld\n",nbsym);

    //Lire la table de profondeur (256 octets)
    uint8_t profondeur[256];
    lire_profondeur(profondeur);
    displaytab256(profondeur);

    
    
    //Créer arbre canonique
    phtree_t abr_can = arbre_canonique(profondeur);

    afficher_arbre(abr_can,1);

    
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
}




