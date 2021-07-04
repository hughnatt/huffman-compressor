#include "analyse.h"
#include "huffman.h"
#include "codage.h"
#include "io.h"
#include "precodage.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
volatile static void displaytab256(uint8_t tab[256])
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
#endif

int main(int argc, char *argv[])
{

    int mtf = 0, rle = 0;

    if (argc != 2 && argc != 3)
    {
        printf("Syntaxe : ./compression <filename> <precode>\n");
        printf(">>> precode : 1 pour mtf   \n");
        printf("              2 pour rle   \n");
        printf("              0 pas de precodage (default)   \n");
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

    uint64_t freq_tab[256];
    uint8_t prof_tab[256];
    uint64_t code_tab[256];

    frequences(argv[1], freq_tab);

#ifdef DEBUG
    printf("========= TABLEAU FREQUENCES ========\n");
    ecriture_tableau_freq(freq_tab);
#endif

    phtree_t hufftree = creer_htree(freq_tab);

#ifdef DEBUG
    printf("======== ARBRE HUFFMAN =======");
    afficher_arbre(hufftree, 1);
#endif

    profondeur(hufftree, prof_tab);

    #ifdef DEBUG
    printf("========== TABLEAU PROFONDEUR ==========");
    displaytab256(prof_tab);
    #endif

    detruire_htree(hufftree);

    phtree_t huffcan = arbre_canonique(prof_tab);

#ifdef DEBUG
    printf("========== ARBRE CANONIQUE ==========")
        afficher_arbre(huffcan, 1);
#endif

    correspondance(huffcan, code_tab);

#ifdef DEBUG
    printf("========== TABLEAU DE CODES =============\n");
    displaytab256(code_tab);
#endif

    char *new_name = malloc((strlen(argv[1]) + 6) * sizeof(char));
    sprintf(new_name, "%s.emdt", argv[1]);

    if (mtf)
    {
        mtf_codage(argv[1], "TEMP.MTF");
        transcodage("TEMP.MTF", new_name, code_tab, prof_tab);
    }
    else if (rle)
    {
        rle_codage(argv[1], "TEMP.RLE");
        transcodage("TEMP.RLE", new_name, code_tab, prof_tab);
    }
    else {
        transcodage(argv[1], new_name, code_tab, prof_tab);        
    }

    return 0;
}
