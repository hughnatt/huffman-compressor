#include "codage.h"
#include "huffman.h"

#include <stdlib.h>

/**
 * Renvoie la valeur de la profondeur la plus élevée
 */
int getProfMax(uint8_t prof[256])
{
	int max = 0;
	for (int i = 0; i < 256; i++)
	{
		if (prof[i] > max)
		{
			max = prof[i];
		}
	}
	return max;
}

/**
 * Créé une feuille avec le nom label
 */
phtree_t create_leaf(uint8_t label, int value)
{
	phtree_t tmp = (phtree_t)malloc(sizeof(htree_t));
	tmp->label[0] = label;
	tmp->taille_label = 1;
	tmp->fgauche = NULL;
	tmp->fdroit = NULL;
	tmp->poids = value;
	return tmp;
}

/**
 * Remplie la file avec les noeuds de profondeurs "value", (appelle la fonction de création de feuille)
 */
void loadFile(file_fifo *f, uint8_t prof[256], int value)
{
	for (int i = 255; i >= 0; i--)
	{
		if (value == prof[i])
		{
			put_file(create_leaf((uint8_t)i, value), f);
		}
	}
}

/**
 * Crée un noeud parent à deux noeuds enfant
 */
phtree_t create_node(phtree_t node1, phtree_t node2, int value)
{
	phtree_t tmp = (phtree_t)malloc(sizeof(htree_t));

	for (int i = 0; i < node1->taille_label; i++)
	{
		tmp->label[i] = node1->label[i];
	}

	for (int i = node1->taille_label; i < node2->taille_label + node1->taille_label; i++)
	{
		tmp->label[i] = node2->label[i - node1->taille_label];
	}

	tmp->taille_label = node1->taille_label + node2->taille_label;
	tmp->fgauche = node1;
	tmp->fdroit = node2;
	tmp->poids = value;
	return tmp;
}

/**
 * Prend le tableau de profondeur et renvoie l'arbre canonique
 */
phtree_t arbre_canonique(uint8_t prof[256])
{
	int profondeur = getProfMax(prof);

	// Initialisation de la file
	file_fifo f;
	f.N = 0;
	f.q = 0;
	f.t = 0;

	phtree_t node1 = NULL;
	phtree_t node2 = NULL;

	loadFile(&f, prof, profondeur);
	node1 = get_file(&f);

	while (profondeur > 1)
	{
		node2 = get_file(&f);
		if (profondeur == 3){
			printf("-----Profondeur=3------\n");
		}
		
		put_file(create_node(node1, node2, profondeur - 1), &f);

		node1 = get_file(&f);

		if (node1->poids != profondeur)
		{
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
int put_file(phtree_t a, file_fifo *f)
{
	if (f->N >= SIZE_FILE)
		return -1;
	else
	{
		f->tab[f->q] = a;
		f->q = ((f->q) + 1) % SIZE_FILE;
		f->N++;
		return 0;
	}
}

/**
 * Récupère l'element dans la file à la position de la tête (et fait avancer la tête, donc "supprime" l'element)
 * Respect le modulo SIZE_FILE et decremente N si succès du get.
 */
phtree_t get_file(file_fifo *f)
{
	if (isEmpty(f))
		return NULL;
	else
	{
		phtree_t a = f->tab[f->t];
		f->t = (f->t + 1) % SIZE_FILE;
		f->N--;

		if (a == NULL){
			printf("WARNING get_file has returned NULL\n");
		}
		return a;
	}
}

/**
 * 1 si elle est vide, 0 sinon
 */
int isEmpty(file_fifo *f)
{
	return f->N == 0;
}

void init_tab(uint64_t tab[256])
{
	for (int i = 0; i < 256; i++)
	{
		tab[i] = 0;
	}
}

int appartient(uint8_t c, uint8_t *label, uint8_t taille_label)
{
	int present = 0;
	int i = 0;
	while (i < taille_label && !present)
	{
		if (label[i] == c)
		{
			present = 1;
		}
		i++;
	}
	return present;
}

uint64_t recherche(phtree_t t, uint8_t c)
{
	uint64_t code = 0;
	phtree_t current = t;
	while (current->fgauche != NULL)
	{

		if (appartient(c, current->fdroit->label, current->fdroit->taille_label))
		{
			code = (code << 1) | 1;
			current = current->fdroit;
		}
		else
		{
			code = code << 1;
			current = current->fgauche;
		}
	}
	return code;
}

void correspondance(phtree_t t, uint64_t code[256])
{

	uint8_t *racine = t->label;
	init_tab(code);
	int i = 0;
	while (i < t->taille_label)
	{
		code[(int) racine[i]] = recherche(t, racine[i]);
		i++;
	}

	return;
}

/*
phtree_t arbre_canonique(uint8_t prof[256]){
	
	char racine[256];
	delete_null_prof(prof, racine);
	ecriture_tableau(racine);
	printf("\n");
	tri_tableau_racine_etape1(prof, racine);
	ecriture_tableau(racine);
	printf("\n");
	tri_tableau_racine_etape2(prof,racine);
	ecriture_tableau(racine);
	printf("\n");


return NULL;
}

void tri_tableau_racine_etape1(uint8_t prof[256], char racine[256]){
	uint8_t temp =0;

	for (int i = 0; i < 256 && racine [i+1]!='\0'; i++)
	{
		for (int j = i; j < 256 && racine [j+1]!='\0'; ++j)
		{
			if(prof[racine[i]] < prof[racine[j+1]]){
			temp= racine[j+1];
			racine[j+1] = racine[i];
			racine[i] = temp;
			}
		}
		
	}

}

void tri_tableau_index(char racine[256], int debut, int fin){
	uint8_t temp =0;

	for (int i = debut; i < fin ; i++)
	{
		for (int j = i; j < fin; ++j)
		{
			if(racine[i] < racine[j+1]){
			temp= racine[j+1];
			racine[j+1] = racine[i];
			racine[i] = temp;
			}
		}
		
	}

}


void tri_tableau_racine_etape2(uint8_t prof[256], char racine[256]){
	uint8_t tmp;
	int debut, fin;

	for (int i = 0; i < 256 && racine [i]!='\0'; i++)
	{
		fin=i;
		tmp = prof[racine[i]];
		while(fin < 256 && tmp == prof[racine[fin+1]]   && racine [fin]!='\0')
		{
			fin ++;
		}
		debut =i;
		if(debut!=fin){
			tri_tableau_index(racine, debut, fin);
		}
		i=fin;

	}
}




void ecriture_tableau( char racine[256]){

	for (int i = 0; i < 256 && racine[i]!= '\0'; i++)
	{
		printf("%c  ", racine[i]);
	}
}


void delete_null_prof(uint8_t prof[256], char racine[256]){
	int k = 0;
	for (int i = 0; i < 256; i++)
	{
		if(prof[i]){
			racine[k]= i;
			k++;
		}
	}
	racine[k]= '\0';
}



void main(int argc, char const *argv[])
{
	uint8_t prof[256];
	init_tab( prof);
	prof[97]=1;
	prof[98]=6;
	prof[99]=4;
	prof[100]=2;
	prof[101]=2;
	prof[102]=2;
	arbre_canonique(prof);
	
	
}
*/
