#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdint.h>


typedef struct t
{
    uint64_t poids;
    uint8_t label[256];
    uint64_t taille_label; //nombre de char dans label
    struct t *fgauche;
    struct t *fdroit;
} htree_t, *phtree_t;

/**
 * Crée un arbre de Huffman quelconque à partir
 * du tableau des fréquences
 * @param uint64_t frequence[256]
 * @return phtree_t
 */
phtree_t creer_htree(uint64_t frequence[256]);

/**
 * Retourne la profondeur de chaque caractère
 * (taille du codage en bit) de l'arbre t
 * @param phtree_t t
 * @param uint8_t prof[256] (non initialisé)
 */
void profondeur(phtree_t t, uint8_t prof[256]);

/**
 * Desalloque un arbre
 * @param phtree_t t
 */
void detruire_htree(phtree_t t);

/**
 * Affichage d'un arbre en console
 * @param phtree_t a : Noeud racine de l'arbre
 * @param int niveau : Niveau d'origine du noeud racine
 */
void afficher_arbre(phtree_t a, int niveau);


#endif
