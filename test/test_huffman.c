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

	assert(arbre->poids==arbre->taille_label); //chaque caractère à un poids de 1

	int result=0;

	if(arbre->fgauche!=NULL){
		result+=exploreur_test2(arbre->fgauche);
	}

	if(arbre->fdroit!=NULL){
		result+=exploreur_test2(arbre->fdroit);
	}

	return result;
}

void exploreur_test3(phtree_t arbre){

	uint64_t objectif=0;

	for(int i=0;i<arbre->taille_label;i++){
		switch(arbre->label[i]){
			case 'd':
				objectif+=2;
				break;
			case 'b':
				objectif+=3;
				break;
			case 'c':
				objectif+=4;
				break;
			case 'a':
				objectif+=5;
				break;
			case 'f':
				objectif+=7;
				break;
			case 'e':
				objectif+=18;
				break;
		}
	}

	assert(objectif==arbre->poids);

	if(arbre->fdroit!=NULL){
		exploreur_test3(arbre->fdroit);
	}

	if(arbre->fgauche!=NULL){
		exploreur_test3(arbre->fgauche);
	}

	return;

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

	for(int i=0;i<256;i++){
		if(i==9){
			assert(prof[9]==1);
		}else{
			assert(prof[i]==0);
		}
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

	assert(256==exploreur_test2(tree_result2));

	printf("Test 2 partie htree completed \n");

	profondeur(tree_result1,prof);

	uint8_t prof_ref=0;

	for(int i=0;i<256;i++){
		if(prof[i]!=0){
			if(prof_ref==0){
				prof_ref=prof[i];
			}
			assert(prof[i]==prof_ref);
		}
	}

	printf("Test 2 partie profondeur completed \n");

	/*-----Test 3----*/

	//on teste avec un exemple du diapo
	//contenu du fichier : d*2 b*3 c*4 a*5 f*7 e*18

	uint64_t freq_test3[256];
	
	for(int i=0;i<256;i++){
		freq_test3[i]=0;
	}

	freq_test3[(int)'d']=2;
	freq_test3[(int)'b']=3;
	freq_test3[(int)'c']=4;
	freq_test3[(int)'a']=5;
	freq_test3[(int)'f']=7;
	freq_test3[(int)'e']=18;

	phtree_t tree_result3 = creer_htree(freq_test3);

	assert(tree_result3!=NULL); //l'arbre existe
	assert(tree_result3->taille_label==6); //il y a 8 symboles différents
	assert(tree_result3->poids==39); //il y a 39 occurrences
	
	exploreur_test3(tree_result3);

	printf("Test 3 partie htree completed \n");

	profondeur(tree_result3,prof);

	assert(prof[(int)'a']==4);
	assert(prof[(int)'b']==5);
	assert(prof[(int)'c']==4);
	assert(prof[(int)'d']==5);
	assert(prof[(int)'e']==2);
	assert(prof[(int)'f']==4);

	printf("Test 3 partie profondeur completed \n");

	return 0;
}