#include "huffman.h"
#include <stdint.h>
#include "stdio.h"

/*
 *
 */
phtree_t arbre_canonique(uint8_t prof[256]);

/*
 *
 */
void correspondance(phtree_t t, uint8_t code[256]);

void ecriture_tableau( char racine[256]);
void delete_null_prof(uint8_t prof[256], char racine[256]);
void init_tab_freq( uint8_t freq[256]);
void tri_tableau_racine_etape1(uint8_t prof[256], char racine[256]);
void tri_tableau_racine_etape2(uint8_t prof[256], char racine[256]);
void tri_tableau_index(char racine[256], int debut, int fin);