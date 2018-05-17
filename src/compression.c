#include "analyse.h"
#include "huffman.h"
#include "codage.h"
#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
volatile static void displaytab256(uint8_t tab[256]){
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
#endif



int main(int argc, char* argv[]){

    if (argc != 2){
        printf("Syntaxe : ./compression <filename>\n");
        return -1;
    }

    uint64_t freq_tab[256];
    uint8_t prof_tab[256];
    uint64_t code_tab[256];

    frequences(argv[1],freq_tab);
    
    #ifdef DEBUG
        printf("========= TABLEAU FREQUENCES ========\n"); 
        ecriture_tableau_freq(freq_tab);
    #endif

    phtree_t hufftree = creer_htree(freq_tab);

    #ifdef DEBUG
    printf("======== ARBRE HUFFMAN =======");
    afficher_arbre(hufftree,1);    
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
    afficher_arbre(huffcan,1);
    #endif

    correspondance(huffcan,code_tab);

    #ifdef DEBUG
    printf("========== TABLEAU DE CODES =============\n");
    displaytab256(code_tab);
    #endif



    char* new_name = malloc((strlen(argv[1]) + 6) * sizeof(char));
    sprintf(new_name,"%s.emdt",argv[1]);


    transcodage(argv[1],new_name,code_tab,prof_tab);
    

    return 0;
}

