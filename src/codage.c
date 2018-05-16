

#include "../include/codage.h"

void init_tab( uint8_t tab[256]){
	for (int i = 0; i < 256; i++)
	{
		tab[i]=0;

	}
}
int appartient(char c, char* label){
	int present =0;
	int i=0;
	while(label[i]!='\0' && !present){
		if(label[i]== c){
			present =1;
		}
		i++;
	}
	return present;

}


uint8_t recherche(phtree_t t, char c){
	uint8_t code = 0;
	phtree_t current = t;
	while(current!=NULL && current->label[1] != '\0'){

		if(appartient(c,current->fdroit->label)){
			code = (code << 1) | 1;
			t = current->fdroit;
		}else{
			code = code << 1;
			t = current->fgauche;
		}
	}
	return code;
}

void correspondance(phtree_t t, uint8_t code[256]){ 

	char* racine = t->label;
	init_tab(code);
	int i =0;
	while(racine[i]!='\0'){
		code[racine[i]]=recherche(t,racine[i]);
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