#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include "precodage.h"

int compare_file(char *fichier1,char*fichier2){
	FILE* f1=fopen(fichier1,"rb");
	assert(f1!=NULL);
	FILE* f2=fopen(fichier2,"rb");
	assert(f2!=NULL);

	char c1,c2;

	fscanf(f1,"%c",&c1);
	fscanf(f2,"%c",&c2);

	while((!feof(f1))&&(!feof(f2))){
		if(c1!=c2){
			fclose(f1);
			fclose(f2);
			return 0;
		}
		fscanf(f1,"%c",&c1);
		fscanf(f2,"%c",&c2);
	}

	if(!(feof(f1)&&feof(f2))){
		fclose(f1);
		fclose(f2);
		return 0;
	}

	fclose(f1);
	fclose(f2);
	return 1;
}

int main(int argc, char const *argv[])
{
	/*------Test 1----- */
	mtf_codage("test_mtf_1.txt", "test_MTF_1.txt");
	mtf_decodage("test_MTF_1.txt", "test_mtf_1_result.txt");
	assert(compare_file("test_mtf_1.txt","test_mtf_1_result.txt"));

	/*------Test 2----- */
	FILE *f2=fopen("test_mtf_2.txt","wb");
	assert(f2!=NULL);
	for(int i = 255; i>=0; i--){
		fprintf(f2, "%i",i );
	}
	fclose(f2);

	mtf_codage("test_mtf_2.txt", "test_MTF_2.txt");
	mtf_decodage("test_MTF_2.txt", "test_mtf_2_result.txt");
	assert(compare_file("test_mtf_2.txt","test_mtf_2_result.txt"));
	return 0;
}