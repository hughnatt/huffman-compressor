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

void encode_rle_prof(uint8_t profondeur[256], uint8_t profondeur_rle[512], uint16_t* taille_rle){

    uint8_t prev2 = profondeur[0];
    uint8_t prev1 = profondeur[1];

    int j=0;
    int taille;

    profondeur_rle[j++] = prev2;
    profondeur_rle[j++] = prev1;

    
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

void decode_rle_prof(uint8_t profondeur[256]){

    uint16_t taille_rle = 0;

    uint8_t prev1 = 0;
    uint8_t prev2 = 0;
    uint8_t c = 0;

    int tmp = fgetc(f_in);
    int tmp2 = fgetc(f_in);
    int j = 0;

    taille_rle = taille_rle | ((tmp & 0xFF) << 8);
    taille_rle = taille_rle | (tmp2 & 0xFF);

    prev1 = fgetc(f_in);
    profondeur[j++] = prev1;

    prev2 = fgetc(f_in);
    profondeur[j++] = prev2;

    taille_rle = taille_rle - 2;

    int i = 0;

    while(taille_rle > 0){

        c = fgetc(f_in);
        taille_rle--;

        if(prev1 == prev2){

            for( i = 0; i<c; i++){
                profondeur[j++] = prev1;
            }
            prev2 = prev1;
            prev1 = 0xFF; // Valeur impossible donc le prochain tour de boucle, on ira forcément dans le else
        }
        else{

            profondeur[j++] = c;
            prev2 = prev1;
            prev1 = c;
        }

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
    printf("Taille : %d", taille);
    printf("Encode : ");
    for (int i = 8 - taille; i < 8; i++)
    {
        printf("%d", !!((code << i) & 0x80));
    }

    nbsym++;

    //On décale le bit de poids fort vers la gauche
    code = code << (64 - taille);

    printf("\tEffective : ");
    //On lit chaque bit valide du code
    
        while ( taille > 0)
        {
            buffer = buffer << 1;
            buffer = buffer | ((code & 0x8000000000000000) >> 63); //Bit de poids fort ramené à droite
            printf("%ld",((code & 0x8000000000000000) >> 63));
            code = code << 1;
            taille--;
            restant--;
            if (restant == 0){
                writebyte();            
            }
        }
    printf("\n");
    
    //****
    //}
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

    uint8_t profondeur_rle[512] = {0};
    uint16_t taille_rle = 0;

    /*Mettre tableau de profondeur encodé en rle*/
    encode_rle_prof(profondeur, profondeur_rle, &taille_rle);
    fputc(taille_rle & 0xFF00,f_out);
    fputc(taille_rle & 0x00FF,f_out);

    int i = 0;
    uint8_t a;
    while (i < taille_rle){
        a = profondeur_rle[i];
        fputc(a,f_out);
        i++;
    }
    //uint8_t c;

    uint64_t cc;
    int taillec;

    //int getcar;
    int c = fgetc(f_in);
    while (c != EOF)
    {
        //c = (uint8_t)getcar;
        cc = code[c];
        taillec = profondeur[c] - 1;

        printf("Caractere %x (%c) -- ", c, c);
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
        for (i = 0; i < a->taille_label; i++)
        {
            printf("%x", a->label[i]);
        }
        printf(" (%d)\n\n", niveau);

        afficher_arbre(a->fgauche, niveau + 1);
    }
    return;
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

    //Lire nombre de symbole à lire (8 octets)
    uint64_t nbsym = lire_nbsym();
    printf("Symboles à lire : %ld\n", nbsym);

    //Lire la table de profondeur (256 octets)
    uint8_t profondeur[256];
    //lire_profondeur(profondeur);
    decode_rle_prof(profondeur);
    displaytab256(profondeur);

    //Créer arbre canonique
    phtree_t abr_can = arbre_canonique(profondeur);

    afficher_arbre(abr_can, 1);

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
