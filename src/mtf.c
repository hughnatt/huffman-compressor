#include <stdio.h>
//#include "mtf.h"

void init_tab(char mtf[256]){
	for (int i = 0; i < 256; ++i)
	{
		mtf[i]=(char)(i);
	}
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
	char tmp0, tmp;
	tmp0 = mtf[0];

	mtf[0]= mtf[c];


	while(i<=(int)c){
		 tmp = mtf[i];
		 mtf[i]=tmp0;
		 tmp0=tmp;
		 i++;
	}
}

void mtf_codage(char *file_name){
	

	FILE *f = fopen(file_name, "rb");
	FILE *new_file = fopen("nvx_fichier.txt", "w");
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
		fprintf(new_file, "%c", c);
	}
	fclose(f);
	fclose(new_file);
}







void afficher_tab(char tab[256]){
	for (int i = 0; i < 256; ++i)
	{

		printf("%c ",tab[i] );
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	mtf_codage("test.txt");
	return 0;
}