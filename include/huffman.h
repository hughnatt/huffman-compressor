#include <stdint.h>

typedef struct t
{
    uint64_t poids;
    char label[256];
    struct t *fgauche;
    struct t *fdroit;
} htree_t, *phtree_t;

/*
 * Crée un arbre de Huffman quelconque à partir
 * du tableau des fréquences
 * @param uint64_t frequence[256]
 * @return phtree_t
 */
phtree_t creer_htree(uint64_t frequence[256]);

/*
 * Retourne la profondeur de chaque caractère
 * (taille du codage en bit) dans l'arbre t
 * @param phtree_t t
 * @param uint8_t prof[256]
 */
void profondeur(phtree_t t, uint8_t prof[256]);

/*
 *
 */
void detruire_htree(phtree_t t);

//Test