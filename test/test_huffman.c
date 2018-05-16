#include <stdio.h>
#include <assert.h>
#include "huffman.h"


//renvoit vrai si l'arbre en entrée est une feuille
//l'arbre doit être non nulle
int feuille(phtree_t arbre){ 
	if(arbre->fgauche!=NULL){
		return 0;
	}

	if(arbre->fdroit!=NULL){
		return 0;
	}

	return 1;
}

//renvoit le nombre de feuille de l'arbre en verifiant qu'il correspond au test 2
int exploreur_test2(phtree_t arbre){

	if(feuille(arbre)){
		assert(arbre->poids==1); //une seule occurence du caractère
		assert(arbre->taille_label==1); //label de taille 1
		return 1;
	}

	assert(poids==taille_label) //chaque caractère à un poids de 1

	int result=0;

	if(arbre->fgauche!=NULL){
		result+=exploreur_test2(arbre->fgauche);
	}

	if(arbre->fdroit!=NULL){
		result+=exploreur_test2(arbre->fdroit);
	}

	return result;
}

int main(int argc, char const *argv[])
{
	uint8_t prof[256];

	/*-----Test 1----*/

	//on teste pour un tableau avec une seule lettre avec une seule occurence

	uint64_t freq_test1[256]={0};
	freq_test1[9]=1;

	phtree_t tree_result1 = creer_htree(freq_test1);

	assert(tree_result1!=NULL); //l'arbre existe
	assert(feuille(tree_result1)); //on a une seule feuille

	printf("Test 1 partie htree completed \n");

	profondeur(tree_result1,prof);

	assert(prof[9]==1);

	for(int i=0;i<256;i++){
		assert(prof[i]==0);
	}

	printf("Test 1 partie profondeur completed \n");

	/*-----Test 2----*/

	//on teste pour un tableau une seule occurence pour chaque symbole

	uint64_t freq_test2[256];
	
	for(int i=0;i<256;i++){
		freq_test2[i]=1;
	}

	phtree_t tree_result2 = creer_htree(freq_test2);

	assert(tree_result2!=NULL); //l'arbre existe

	exploreur_test2(tree_result2);

	printf("Test 2 partie htree completed \n");

	profondeur(tree_result1,prof);

	assert(prof[9]==1);

	for(int i=0;i<256;i++){
		assert(prof[i]==0);
	}

	printf("Test 1 partie profondeur completed \n");

	return 0;
}