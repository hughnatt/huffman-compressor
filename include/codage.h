#include "huffman.h"
#include <stdint.h>

#define SIZE_FILE 256

/*
 *
 */
phtree_t arbre_canonique(uint8_t prof[256]);

/*
 *
 */
void correspondance(phtree_t t, uint8_t code[256]);




/* file (tableau) */

typedef struct {
	int N, t, q;
	phtree_t tab[SIZE_FILE];
} file_fifo;

// Insère un element dans la file a la position de la queue, puis incrémente q de +1, en faisant attention au modulo SIZE_FILE
// incrémente aussi N de 1 (si l'insertion est possible bien sur)
int put_file(phtree_t a, file_fifo* f);

// Récupère l'element dans la file à la position de la tête (et fait avancer la tête, donc "supprime" l'element)
// Respect le modulo SIZE_FILE et decremente N si succès du get.
phtree_t get_file(file_fifo* f);

// 1 si elle est vide, 0 sinon
int isEmpty(file_fifo* f);

