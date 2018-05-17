#include <stdio.h>
#include "analyse.h"

//test_analyse_1 : contient un seul caractère 'a'
//test_analyse_2 : contient une seule occurence de chaque lettre minuscule
// et majuscule
//test_analyse_3 : contient 3 occurences successives de chaque valeurs possibles.
// Il est remplie par la fonction de test
//test_analyse_4 : fichier vide

int main(int argc, char const *argv[])
{
	uint64_t freq[256];

	/*-----Test 1----*/

	//on teste l'initialisation et un fichier avec une seule lettre

	for (int i = 0; i < 256; ++i){
		freq[i]=9;
	}

	frequences("test_analyse_1.txt",freq);

	for(int i=0;i<256;i++){
		if(i=='a'){
			if(freq[i]!=1){
				printf("Test 1 : 'a' non détecté\n");
				return 1;
			}
		}else{
			if(freq[i]!=0){
				if(freq[i]==9){
					printf("Test 1 : problème d'initialisation en %i \n",i);
				}else{
					printf("Test 1 : problème de détection en %i \n",i);
				}
				return 1;
			}
		}
	}

	printf("Test 1 completed \n");

	/*-----Test 2----*/

	//on teste la détection des lettres

	frequences("test_analyse_2.txt",freq);

	for(int i=0;i<256;i++){
		if((i>='a')&&(i<='z')){
			if(freq[i]!=1){
				printf("Test 2 : minuscule %c mal détectée\n",(char) i);
				return 1;
			}
		}else if((i>='A')&&(i<='Z')){
			if(freq[i]!=1){
				printf("Test 2 : majuscule %c mal détectée\n",(char) i);
				return 1;
			}
		}else{
			if(freq[i]!=0){
				printf("Test 2 : problème de détection en %c \n",(char) i);
				return 1;
			}
		}
	}

	printf("Test 2 completed \n");

	/*-----Test 3----*/

	//on teste la détection de toutes les valeurs sur 3 occurences successives

	FILE *file = fopen("test_analyse_3.txt","w");

	if(file==NULL){
		printf("Test 3 : problème d'ouverture de fichier\n");
		return 2;
	}

	for (int i = 0; i < 256; i++){
		for(int j=0; j < 3;j++){
			fprintf(file, "%c",(char)i );
		}
	}

	fclose(file);

	frequences("test_analyse_3.txt",freq);

	for(int i=0;i<256;i++){
		if(freq[i]!=3){
			printf("Test 3 : problème de détection en %i \n",i);
			return 1;
		}
	}

	printf("Test 3 completed \n");

	/*-----Test 4----*/

	//on teste sur un fichier vide

	frequences("test_analyse_4.txt",freq);

	for(int i=0;i<256;i++){
		if(freq[i]!=0){
			printf("Test 4 : problème de détection en %i \n",i);
			return 1;
		}
	}

	printf("Test 4 completed \n");

	return 0;
}