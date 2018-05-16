#include "../include/codage.h"

tmp = get_file(&f);
put_file(a,&f);

/**
 * Renvoie la valeur de la profondeur la plus élevée
 */
int getProfMax(uint8_t prof[256]) {
  int max = 0;
  for(int i=0; i<256; i++) {
    if(prof[i] > max) {
      max = prof[i];
    }
  }
  return max;
}

/**
 * Remplie la file avec les noeuds de profondeurs "value", (appelle la fonction de création de feuille)
 */
void loadFile(file_fifo &f, uint8_t prof[256], int value) {
  for(int i=255; i>=0; i++) {
    if(value == prof[i]) {
      put_file(create_leaf((char)i, value),f);
    }
  }
}

/**
 * Créé une feuille avec le nom label
 */
phtree_t create_leaf(char label, int value) {
  phtree_t tmp = (phtree_t) malloc(sizeof(htree));
  tmp->label[0] = label;
  tmp->taille_label = 1;
  tmp->fgauche = NULL;
  tmp->fdroite = NULL;
  tmp->poids = value;
  return tmp;
}

/**
 * Crée un noeud parent à deux noeuds enfant
 */
phtree_t create_node(phtree_t node1, phtree_t node2, int value) {
  phtree_t tmp = (phtree_t) malloc(sizeof(htree));

  for (int i = 0; i < node1->taille_label; i++) {
      tmp->label[i] = node1->label[i];
  }

  for (int i = node1->taille_label; i < node2->taille_label; i++) {
      tmp->label[i] = node2->label[i];
  }

  tmp->taille_label = node1->taille_label + node2->taille_label;
  tmp->fgauche = node1;
  tmp->fdroite = node2;
  tmp->poids = value;
  return tmp;
}

/**
 * Prend le tableau de profondeur et renvoie l'arbre canonique
 */
phtree_t arbre_canonique(uint8_t prof[256]) {
  int profondeur = getProfMax(prof);

  // Initialisation de la file
  file_fifo f;
  f.N = 0;
  f.q = 0;
  f.t = 0;

  phtree_t node1 = NULL;
  phtree_t node2 = NULL;

  node1 = get_file(&f);

  while(profondeur > 1) {

    node2 = get_file(&f);

    put_file(create_node(node1, node2, profondeur-1),&f);

    node1 = get_file(&f);

    if(node1->poids != profondeur) {
      profondeur = node1->poids;
      loadFile(&f, prof, profondeur);
    }
  }

  return node1;
}



/** 
 * Insère un element dans la file a la position de la queue, puis incrémente q de +1, en faisant attention au modulo SIZE_FILE
 * incrémente aussi N de 1 (si l'insertion est possible bien sur)
 */
int put_file(phtree_t a, file_fifo* f) {
  if(f->N >= SIZE_FILE)
    return -1;
  else {
    f->tab[f->q] = a;
    f->q = ((f->q)+1)%SIZE_FILE;
    f->N++;
    return 0;
  }
}

/**
 * Récupère l'element dans la file à la position de la tête (et fait avancer la tête, donc "supprime" l'element)
 * Respect le modulo SIZE_FILE et decremente N si succès du get.
 */
phtree_t get_file(file_fifo* f) {
  if(isEmpty(f))
    return NULL;
  else{
    phtree_t a = f->tab[f->t];
    f->t = (f->t+1)%SIZE_FILE;
    f->N--;
    return a;
  }
}

/**
 * 1 si elle est vide, 0 sinon
 */
int isEmpty(file_fifo* f) {
  return f->N == 0;
}