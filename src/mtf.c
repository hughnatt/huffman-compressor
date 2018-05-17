#include <stdio.h>
//#include "mtf.h"
#include "string.h"
#include <assert.h>
#include "math.h"
#include "stdint.h"

/*Initialisation de la table mtf*/
void init_tab(char mtf[256]){
	for (int i = 0; i < 256; ++i)
	{
		mtf[i]=(char)(i);
	}
}

/*Affiche la table mtf*/
void afficher_tab(char tab[256]){
	for (int i = 0; i < 256; ++i){

		printf("%c ",tab[i] );
	}
	printf("\n");
}

/*Renvoie le code du caractère correspondant dans la table*/
char codage(char c, char mtf[256]){
	int i=0;
	while(c!= mtf[i]){
		i++;
	}
	return (char)i;
}


/*Fonction de décalage de la table mtf*/
void decalage(char c, char mtf[256]){

	int i =1;
	int k=1;
	char tmp0, tmp;
	tmp0 = mtf[0];
	uint8_t int_c=c;

	if((int)c <0){ // On verifie si le code du caractère n'est pas negatif = >127 en decimal dans la table ascii
		int_c = (int)c +256; //conversion
	}

	mtf[0]= mtf[int_c]; //decalage
	while(i<=int_c){
		tmp = mtf[i];
		mtf[i]=tmp0;
		tmp0=tmp;
		i++;
	}

}



void mtf_decodage(char *entree, char *sortie){
    FILE *entry = fopen(entree,"rb"); //ouverture du fichier d'entrée
    assert(entry!=NULL); //le fichier est ouvert

    FILE *output = fopen(sortie,"wb"); //ouverture du fichier de sortie
    assert(output!=NULL); //le fichier est ouvert
   
    char c1,c2;
    char mtf[256];
    init_tab(mtf);

    fscanf(entry,"%c",&c1);
 	uint8_t int_c=c1;


    while(!feof(entry)){

		if((int)c1 <0){  // On verifie si le code du caractère a decoder n'est pas negatif = >127 en decimal dans la table ascii
			int_c = (int)c1 +256;
		}
        c2=mtf[int_c]; //on recupère directement dans la table mtf
        fprintf(output, "%c", c2); //on ecrit dans le fichier de sortie
        decalage(c1, mtf); //on decale la table
        fscanf(entry,"%c",&c1); //on lit un nouveau caratère
        int_c=c1;
    }

    fclose(entry); //fermetures des fichiers
    fclose(output);

}

void mtf_codage(char *file_in, char *file_out){

    FILE *f = fopen(file_in, "rb");
    FILE *new_file = fopen(file_out, "wb");
    char mtf[256];
    char c;
    assert(f!=NULL); 
    init_tab(mtf); //initialisation de la table

    c=fgetc(f);
    while(!feof(f)){ //on parcours le fichier
        c=codage(c, mtf); // recupère le code correspondnat au caractère dans la table
        decalage(c, mtf); //on decale la table
        fputc(c,new_file); //on ecrit dans le fichier
        c=fgetc(f); //on recupère un nouveau caractère
    }
    fclose(f); //fermetures des fichiers
    fclose(new_file);
}