#include "../include/huffman.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//_______________________ Partie non déclarée _______________________

//Structure liste chainée
typedef struct c {
    phtree_t elem;
    struct c* next;
} list_t, *plist_t;

/*
 * Crée une feuille
 * @param uint64_t poids
 * @param char label[256]
 * @result phtree : la feuille crée
 */
phtree_t creer_feuille(uint64_t poids, char label) {
    phtree_t result = malloc(sizeof(htree_t));
    
    result->poids = poids;
    result->label[0] = label;
    result->taille_label = 1;
    result->fgauche = NULL;
    result->fdroit = NULL;
    
    return result;
}

/*
 * Crée un noeud père à partir de deux noeuds fils
 * @param phtree_t n1 : noeud fils gauche
 * @param phtree_t n2 : noeud fils droit
 * @return phtree_t
 */
phtree_t creer_noeud_parent(phtree_t n1, phtree_t n2) {
    // Création du nouveau noeud
    phtree_t tree = malloc(sizeof(htree_t));

    // Ajout des fils
    tree->fgauche = n1;
    tree->fdroit = n2;

    // Remplissage label du père
    for (int i = 0; i < n1->taille_label; i++) {
        tree->label[i] = n1->label[i];
    }

    for (int i = n1->taille_label; i < n2->taille_label; i++) {
        tree->label[i] = n2->label[i];
    }

    // Calcul taille_label
    tree->taille_label = n1->taille_label + n2->taille_label;
    
    // Calcul poids
    tree->poids = n1->poids + n2->poids;
    
    //Vérification
    assert( tree->poids > 0 );
    assert( tree->taille_label > 0 );
    assert( tree->fdroit != NULL );
    assert( tree->fgauche != NULL );

    return tree;
}

/*
 * Ajoute un arbre à une liste croissante
 * @param uint64_t poids
 * @param char label[256]
 * @result phtree : la feuille crée
 */
plist_t ajouter_membre(plist_t list, phtree_t membre) {
    plist_t new = malloc(sizeof(list_t));
    new->elem = membre;
    plist_t tmp = list, prev = NULL;

    while (tmp != NULL) {
        if (tmp->elem->poids < membre->poids) {
            prev = tmp;
            tmp = tmp->next;
        } else {
            if (prev == NULL) {
                new->next = list;
                list = new;
            } else {
                prev->next = new;
                new->next = tmp;
            }
            break;
        }
    }

    if (tmp == NULL) {
        prev->next = new;
        new->next = NULL;
    }

    return list;
}

inline plist_t supprimer_plus_petit_membre(plist_t l) { return l->next; }

plist_t conversion_list(uint64_t frequence[256]) {
    plist_t result = NULL;  // on initialise une liste

    for (int i = 0; i < 256; i++) {  // pour chaque caractère

        if (frequence[i] == 0) {  // si la fréquence est nulle
            continue;             // on ne fait rien
        }

        // sinon on créer une feuille pour ce caractère
        phtree_t feuille = creer_feuille(frequence[i], (char)i);

        // on met la feuille à une position adaptée dans la liste
        result = ajouter_membre(result, feuille);
    }
}

//_______________________ Partie déclarée _______________________

/*
 * Crée un arbre de Huffman quelconque à partir
 * du tableau des fréquences
 * @param uint64_t frequence[256]
 * @return phtree_t : NULL si tableau de fréquence vide
 */
phtree_t creer_htree(uint64_t frequence[256]) {
    plist_t liste = conversion_list(frequence);

    phtree_t arbre;

    if (liste == NULL) {
        return NULL;
    }

    while (liste->next != NULL) {
        arbre = creer_noeud_parent(liste->elem, liste->next->elem);

        ajouter_membre(liste, arbre);

        supprimer_plus_petit_membre(liste);
        supprimer_plus_petit_membre(liste);
    }

    return arbre;
}


void recur_profondeur(phtree_t t, uint8_t prof[256], uint64_t profondeur){
	assert(t->taille_label>0);
	
	if(t->taille_label==1){
		prof[t->label[0]]=profondeur;
	}else{
		recur_profondeur(t->fgauche,prof,profondeur+1);
		recur_profondeur(t->fdroit,prof,profondeur+1);
	}

	return;
}


/*
 * Retourne la profondeur de chaque caractère
 * (taille du codage en bit) dans l'arbre t
 * @param phtree_t t
 * @param uint8_t prof[256]
 */
void profondeur(phtree_t t, uint8_t prof[256]){
	for(int i=0;i<256;i++){
		prof[i]=0;
	}

	if(t==NULL){
		printf("Arbre vide, fonction profondeur non applicable\n");
	}

	recur_profondeur(t,prof,1);
}

/* Détruit l'arbre de Huffman
 * @param phtree_t t : racine de l'arbre
 */
void detruire_htree(phtree_t t) {
    if (t) {
        detruire_htree(t->fdroit);
        detruire_htree(t->fgauche);
        free(t);
    }
}