#include <stdio.h>
//#include "mtf.h"
#include "string.h"
#include <assert.h>

void init_tab(char mtf[256]){
	for (int i = 0; i < 256; ++i)
	{
		mtf[i]=(char)(i);
	}
}

void afficher_tab(char tab[256]){
	for (int i = 0; i < 256; ++i){

		printf("%c ",tab[i] );
	}
	printf("\n");
}

char codage(char c, char mtf[256]){
	int i=0;
	while(c!= mtf[i]){
		i++;
	}
	return (char)i;
}

void decalage(char c, char mtf[256]){

	int i =1;
	int k=1;
	char tmp0, tmp;
	tmp0 = mtf[0];
	if((int)c == -1){
		mtf[0]=mtf[255];
		while(k<256){
			tmp = mtf[k];
			mtf[k]=tmp0;
			tmp0=tmp;
			k++;
		}
	}else{
		mtf[0]= mtf[c];
		while(i<=(int)c){

			tmp = mtf[i];
			mtf[i]=tmp0;
			tmp0=tmp;
			i++;
		}
	}
		//printf("\nTableau :\n");
		//afficher_tab(mtf);
	}



void mtf_codage(char *file_name){

	FILE *f = fopen(file_name, "rb");
	FILE *new_file = fopen("nvx_fichier_MTF.txt", "wb");
	char mtf[256];
	char c;
	if (f==NULL){
		printf("Error file\n");
		return;
	}
	init_tab(mtf);
	while(!feof(f)){
		c=fgetc(f);
		c=codage(c, mtf);
		decalage(c, mtf);
		fputc(c,new_file);
	}
	fclose(f);
	fclose(new_file);
}


void mtf_decodage(char *entree){
    FILE *entry = fopen(entree,"rb"); //ouverture du fichier d'entrée
    assert(entry!=NULL); //le fichier est ouvert

   /* char *tmp = strstr("_MTF",entree); //recherche du motif _MTF
    assert(tmp!=NULL); //le motif doit être présent

    int i=3;
    while(tmp[i]!='\0');{
        i++;
        tmp[i-4]=tmp[i];
    }*/

    FILE *output = fopen("decodage_mtf","wb"); //ouverture du fichier de sortie
    assert(output!=NULL); //le fichier est ouvert

    char c;
    char mtf[256];
    init_tab(mtf);

    fscanf(entry,"%c",&c);

    while(!feof(entry)){
        c=mtf[(int)c];
        fprintf(output, "%c", c);
        decalage(c, mtf);

        fscanf(entry,"%c",&c);
    }

    fclose(entry);
    fclose(output);

}


	int main(int argc, char const *argv[])
	{
		mtf_codage("test_MTF.txt");
		mtf_decodage("nvx_fichier_MTF.txt");
		return 0;
	}