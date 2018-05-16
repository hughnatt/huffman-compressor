#include "../include/codage.h"

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



void correspondance(phtree_t t, uint8_t code[256]){ 
	return;
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

void init_tab( uint8_t tab[256]){
	for (int i = 0; i < 256; i++)
	{
		tab[i]=0;

	}
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
