#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/*
 * Remplit les frequences d'apparition
 * de chaque caractère/symbole binaire
 * @param char* filename : Nom du fichier d'entrée (non ouvert en entrée)
 * @param uint64_t freq[256] : Tableau des fréquences à modifier (non initialisé)
 */
void frequences(char* filename, uint64_t freq[256]);